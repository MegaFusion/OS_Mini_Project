#include "ordonnancement.h"

/* Codes couleurs ANSI */
#define NORMAL   "\033[0m"
#define GRAS    "\033[1m"
#define VERT "\033[1;32m"
#define JAUNE "\033[1;33m"
#define ROUGE "\033[1;31m"
#define VIOLET "\033[1;35m"
#define BLEU "\033[1;34m"
#define BLANC "\033[1;37m"

void afficher_entete(Liste liste){
    /**
     * Affiche l'en tête du tableau
     * @param liste : la liste des processus
     */
    printf("%-8s", "Temps");
    Liste l = liste;
    while (l != NULL){
        printf(" P%-5d", l->proc.pid);
        l = l->suivant;
    }
    printf("\n");
    printf("--------");
    l = liste;
    while (l != NULL){
        printf("--------");
        l = l->suivant;
    }
    printf("\n");
}

Liste chercher_premier_pret(Liste l){
    /**
     * Parcourt une liste de processus et renvoie le premier processus prêt
     * @param l : La liste de processus
     * @return Si un processus prêt trouvé, renvoie ce dernier. NULL sinon
     */
    Liste ll = l;
    while (ll != NULL){
        if (est_pret(ll->proc))
            return ll;
        ll = ll->suivant;
    }
    return NULL;
}

void afficher_ligne(Liste liste, int temps){
    /**
     * Affiche une ligne des informations de l'ordonnancement en fonction du temps
     * @param liste : la liste des processus
     * @param temps : temps de l'ordonnancement
     */
    printf("%-8d", temps);
    Liste l = liste;
    while (l != NULL){
        switch (l->proc.etat){
            case 0: printf(" " BLANC "%-6s" NORMAL, "---"); break;
            case 1: printf(" " JAUNE "%-6s" NORMAL, "PRT"); break;
            case 2: printf(" " BLEU "%-6s" NORMAL, "CPU"); break;
            case 3: printf(" " VIOLET "%-6s" NORMAL,  "BLQ"); break;
            case 4: printf(" " VERT "%-6s" NORMAL,  "FIN"); break;
            case 5: printf(" %-6s", "   "); break;                 /* colonne vide */
            default:printf(" " ROUGE "%-6s", "???");        break;
        }
        l = l->suivant;
    }
    printf("\n");
}

Liste trier_par_arrivee(Liste l){
    /**
     * Trie une liste de processus par ordre d'arrivée
     * @param l : la liste à trier
     * @return la liste triée
     */
    Liste triee = NULL;
    Liste courant = l;

    while (courant != NULL){
        Liste suivant = courant->suivant;
        Processus p = courant->proc;

        if (triee == NULL || p.arrive < triee->proc.arrive){
            courant->suivant = triee;
            triee = courant;
        }
        else{
            Liste tmp = triee;
            while (tmp->suivant != NULL && tmp->suivant->proc.arrive <= p.arrive){
                tmp = tmp->suivant;
            }
            courant->suivant = tmp->suivant;
            tmp->suivant = courant;
        }
        courant = suivant;
    }
    return triee;
}

int tous_termines_file(Liste l){
    /**
     * Verifie si tous les processus d'une liste sont tous terminés
     * @param l : Liste de processus
     * @return 1 si oui, 0 sinon
     */
    Liste ll = l;
    while (!estVideListe(ll)){
        if (!est_termine(ll->proc) && ll->proc.etat != 5){
            return 0;
        }
        ll = ll->suivant;
    }
    return 1;
}

int ordonnancer(Liste *liste, int algo, int quantum){
    /**
     * Ordonnancement d'une liste de processus selon l'algorithme choisi.
     * @param liste   : adresse de la liste de processus
     * @param algo    : algorithme (ALGO_FIFO=0, ALGO_SJF=1, ALGO_SJRF=2, ALGO_RR=3)
     * @param quantum : durée du quantum en ms pour RR
     * @return 0 si tout s'est bien passé
     */
    int temps = 0;
    int ticks_restants = 0;   /* compteur du quantum pour RR */
    Liste l;
    Liste courant = NULL;
    int cpu_libre = 1;
 
    const char *noms[] = {"FIFO", "SJF", "SJRF", "RR"};
 
    *liste = trier_par_arrivee(*liste);
 
    printf("=== Ordonnancement %s ===\n\n", noms[algo]);
    if (algo == ALGO_RR)
        printf("Quantum : %d\n\n", quantum);
 
    printf("Legende : "
           BLEU "CPU" NORMAL " = en cours  "
           JAUNE "PRT" NORMAL " = pret  "
           VIOLET "BLQ" NORMAL " = bloque  "
           VERT "FIN" NORMAL " = termine  "
           BLANC "---" NORMAL " = non arrive\n\n");
 
    afficher_entete(*liste);
 
    while (!tous_termines_file(*liste)){
 
        l = *liste;
 
        /* traitement des processus arrivés et bloqués, changement d'état */
        while (l != NULL){
            if (l->proc.arrive <= temps && non_arrive(l->proc)){
                l->proc.etat = 1;
            }
 
            if (est_bloque(l->proc) && !estVideCycle(l->proc.ES)){
                l->proc.ES->duree--;
 
                if (get_DureeCycle(l->proc.ES) == 0){
                    supTeteCycle(&l->proc.ES);
                    l->proc.etat = 1;
                }
            }
 
            l = l->suivant;
        }
 
        /* traitement du processus en cours, changement d'état */
        if (!cpu_libre && courant != NULL){
 
            if (!estVideCycle(courant->proc.CPU)){
                courant->proc.CPU->duree--;
            }
 
            ticks_restants--;
 
            if (get_DureeCycle(courant->proc.CPU) == 0){
                supTeteCycle(&courant->proc.CPU);
 
                if (!estVideCycle(courant->proc.ES)){
                    courant->proc.etat = 3;
                }
                else{
                    courant->proc.etat = 4;
                    courant = NULL;
                }
 
                cpu_libre = 1;
                courant = NULL;
                ticks_restants = 0;
            }
            else if (algo == ALGO_RR && ticks_restants == 0){
                courant->proc.etat = 1;
                cpu_libre = 1;
                courant = NULL;
            }
        }
 
        /* préemption SJRF : remettre le courant à PRT si un plus court est prêt */
        if (algo == ALGO_SJRF && !cpu_libre && courant != NULL){
            Liste plus_court = chercher_sjrf(*liste);
 
            if (plus_court != NULL && plus_court != courant &&
                get_DureeCycle(plus_court->proc.CPU) < get_DureeCycle(courant->proc.CPU)){
 
                courant->proc.etat = 1;
                cpu_libre = 1;
                courant = NULL;
            }
        }
 
        /* prendre le prochain processus selon l'algorithme */
        if (cpu_libre){
            if      (algo == ALGO_FIFO) courant = chercher_premier_pret(*liste);
            else if (algo == ALGO_SJF)  courant = chercher_sjf(*liste);
            else if (algo == ALGO_SJRF) courant = chercher_sjrf(*liste);
            else                        courant = chercher_premier_pret(*liste);
 
            if (courant != NULL){
                courant->proc.etat = 2;
                cpu_libre = 0;
                ticks_restants = quantum;  /* initialise le quantum (ignoré hors RR) */
            }
        }
 
        /* affichage */
        afficher_ligne(*liste, temps);
 
        l = *liste;
 
        /* Passage état 4 (FIN) en état 5 */
        while (l != NULL){
            if (l->proc.etat == 4){
                l->proc.etat = 5;
            }
            l = l->suivant;
        }
 
        temps++;
        sleep(1);
    }
 
    printf("--------\n");
    printf("Tous les processus sont terminés !\n\n");
    return 0;
}