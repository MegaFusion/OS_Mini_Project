#include "ordonnancement.h"

/* Codes couleurs ANSI */
#define NORMAL   "\033[0m"
#define TOUT    "\033[1m"
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
    printf("%-14s", "Temps");
    Liste l = liste;
    while (l != NULL){
        printf(" P%-5d", l->proc.pid);
        l = l->suivant;
    }
    printf("\n");
    printf("----------");
    l = liste;
    while (l != NULL){
        printf("--------");
        l = l->suivant;
    }
    printf("\n");
}

void ecrire_entete(FILE *csv, Liste liste){
    /**
     * Écrit l'en-tête du tableau dans le fichier CSV
     * @param csv   : fichier CSV
     * @param liste : la liste des processus
     */
    fprintf(csv, "Temps");
    Liste l = liste;
    while (l != NULL){
        fprintf(csv, "\tP%d", l->proc.pid);
        l = l->suivant;
    }
    fprintf(csv, "\n");
}

Liste chercher_premier_pret(Liste l){
    /**
     * Parcourt une liste de processus et renvoie le premier processus prêt
     * @param l : La liste de processus
     * @return Si un processus prêt trouvé et prioritaire, renvoie ce dernier. NULL sinon
     */
    Liste ll = l;
    Liste non_prioritaire = NULL;
    while (ll != NULL){
        if (ll->proc.etat == 6){
            non_prioritaire = ll;
        }
        if (est_pret(ll->proc))
            return ll;
        ll = ll->suivant;
    }
    return non_prioritaire;
}

void afficher_ligne(Liste liste, int temps){
    /**
     * Affiche une ligne des informations de l'ordonnancement en fonction du temps
     * @param liste : la liste des processus
     * @param temps : temps de l'ordonnancement
     */
    printf("%-14d", temps);
    Liste l = liste;
    while (l != NULL){
        switch (l->proc.etat){
            case 0: printf(" " BLANC "%-6s" NORMAL, "---"); break;
            case 1: printf(" " JAUNE "%-6s" NORMAL, "PRT"); break;
            case 2: printf(" " BLEU "%-6s" NORMAL,  "CPU"); break;
            case 3: printf(" " VIOLET "%-6s" NORMAL, "BLQ"); break;
            case 4: printf(" " VERT "%-6s" NORMAL,   "FIN"); break;
            case 5: printf(" %-6s", "   "); break;
            case 6: printf(" " JAUNE "%-6s" NORMAL, "PRT"); break;
            default:printf(" " ROUGE "%-6s", "???"); break;
        }
        l = l->suivant;
    }
    printf("\n");
}

void ecrire_ligne(FILE *csv, Liste liste, int temps){
    /**
     * Écrit une ligne de l'ordonnancement dans le fichier CSV
     * @param csv   : fichier CSV
     * @param liste : la liste des processus
     * @param temps : temps de l'ordonnancement
     */
    fprintf(csv, "%d", temps);
    Liste l = liste;
    while (l != NULL){
        switch (l->proc.etat){
            case 0: fprintf(csv, "\t---"); break;
            case 1: fprintf(csv, "\tPRT"); break;
            case 2: fprintf(csv, "\tCPU"); break;
            case 3: fprintf(csv, "\tBLQ"); break;
            case 4: fprintf(csv, "\tFIN"); break;
            case 5: fprintf(csv, "\t");    break;
            case 6: fprintf(csv, "\tPRT"); break;
            default:fprintf(csv, "\t???"); break;
        }
        l = l->suivant;
    }
    fprintf(csv, "\n");
}

void afficher_restitution(Liste liste){
    /**
     * Afficher le temps de restitution d'un processus
     * @param liste : la liste de processus
     */
    Liste l = liste;
    printf("----------");
    while (l != NULL){
        printf("--------");
        l = l->suivant;
    }
    printf("\n");
    printf(BLEU "%-14s" NORMAL, "Restitution");
    l = liste;
    while (!estVideListe(l)){
        printf(" %-6d", Proc_temps_restitution(l->proc));
        l = l->suivant;
    }
    printf("\n");
}

void afficher_attente(Liste liste){
    /**
     * Afficher le temps d'attente d'un processus
     * @param liste : la liste de processus
     */
    Liste l = liste;
    printf("----------");
    while (l != NULL){
        printf("--------");
        l = l->suivant;
    }
    printf("\n");
    printf(BLEU "%-14s" NORMAL, "Attente");
    l = liste;
    while (!estVideListe(l)){
        printf(" %-6d", Proc_temps_attente(l->proc));
        l = l->suivant;
    }
    printf("\n");
}

void afficher_reponse(Liste liste){
    /**
     * Afficher le temps de reponse d'un processus
     * @param liste : la liste de processus
     */
    Liste l = liste;
    printf("----------");
    while (l != NULL){
        printf("--------");
        l = l->suivant;
    }
    printf("\n");
    printf(BLEU "%-14s" NORMAL, "Reponse");
    l = liste;
    while (!estVideListe(l)){
        printf(" %-6d", Proc_temps_reponse(l->proc));
        l = l->suivant;
    }
    printf("\n");
}

void afficher_restitution_moyen(Liste liste){
    /**
     * Afficher le temps de restitution moyen d'un ordonnancement
     * @param liste : la liste de processus
     */
    Liste l = liste;
    printf("----------");
    while (l != NULL){
        printf("--------");
        l = l->suivant;
    }
    printf("\n");
    printf(VERT "%-14s" NORMAL, "Restitution_M");
    printf(" %.2f \n", restitution_moyen(liste));
}

void afficher_attente_moyen(Liste liste){
    /**
     * Afficher le temps d'attente moyen d'un ordonnancement
     * @param liste : la liste de processus
     */
    Liste l = liste;
    printf("----------");
    while (l != NULL){
        printf("--------");
        l = l->suivant;
    }
    printf("\n");
    printf(VERT "%-14s" NORMAL, "Attente_M");
    printf(" %.2f \n", attente_moyen(liste));
}

void afficher_reponse_moyen(Liste liste){
    /**
     * Afficher le temps de reponse moyen d'un ordonnancement
     * @param liste : la liste de processus
     */
    Liste l = liste;
    printf("----------");
    while (l != NULL){
        printf("--------");
        l = l->suivant;
    }
    printf("\n");
    printf(VERT "%-14s" NORMAL, "Reponse_M");
    printf(" %.2f \n", reponse_moyen(liste));
}

void afficher_occupation(int temps_CPU_actif, int temps_total, Liste liste){
    /**
     * Afficher le temps d'occupation moyen du système durant un ordonnancement
     * @param liste : la liste de processus
     * @param temps_CPU_actif : temps où le CPU est resté occupé
     * @param temps_total : temps total de l'ordonnancement
     */
    Liste l = liste;
    printf("----------");
    while (l != NULL){
        printf("--------");
        l = l->suivant;
    }
    printf("\n");
    printf(VERT "%-14s" NORMAL, "T_occupation");
    printf(" %.2f %%\n", (float)Taux_occupation(temps_CPU_actif, temps_total));
}

void ecrire_indicateurs(FILE *csv, Liste liste,
                         int temps_CPU_actif, int temps_total){
    /**
     * Écrit les indicateurs de performance dans le fichier CSV
     * @param csv             : fichier CSV
     * @param liste           : la liste des processus
     * @param temps_CPU_actif : temps total où le CPU est resté occupé
     * @param temps_total     : temps total de l'ordonnancement
     */
    Liste l;

    /* ligne vide de séparation */
    fprintf(csv, "\n");

    /* en-tête indicateurs */
    fprintf(csv, "Indicateur");
    l = liste;
    while (l != NULL){
        fprintf(csv, "\tP%d", l->proc.pid);
        l = l->suivant;
    }
    fprintf(csv, "\n");

    /* restitution */
    fprintf(csv, "Restitution");
    l = liste;
    while (!estVideListe(l)){
        fprintf(csv, "\t%d", Proc_temps_restitution(l->proc));
        l = l->suivant;
    }
    fprintf(csv, "\n");

    /* attente */
    fprintf(csv, "Attente");
    l = liste;
    while (!estVideListe(l)){
        fprintf(csv, "\t%d", Proc_temps_attente(l->proc));
        l = l->suivant;
    }
    fprintf(csv, "\n");

    /* reponse */
    fprintf(csv, "Reponse");
    l = liste;
    while (!estVideListe(l)){
        fprintf(csv, "\t%d", Proc_temps_reponse(l->proc));
        l = l->suivant;
    }
    fprintf(csv, "\n");

    /* t_restitution moyen */
    fprintf(csv, "t_restitution_moyen");
    l = liste;
        fprintf(csv, "\t%.2f", restitution_moyen(l));
    fprintf(csv, "\n");

    /* t_attente moyen */
    fprintf(csv, "t_attente_moyen");
    l = liste;
        fprintf(csv, "\t%.2f", attente_moyen(l));
    fprintf(csv, "\n");

    /* t_reponse moyen */
    fprintf(csv, "t_reponse_moyen");
    l = liste;
        fprintf(csv, "\t%.2f", reponse_moyen(l));
    fprintf(csv, "\n");

    /* taux occupation */
    fprintf(csv, "T_occupation\t%.2f\n",
            (double)Taux_occupation(temps_CPU_actif, temps_total));
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

int ordonnancer(Liste *liste, int algo, int quantum, int export_csv){
    /**
     * Ordonnancement d'une liste de processus selon l'algorithme choisi.
     * @param liste      : adresse de la liste de processus
     * @param algo       : algorithme (ALGO_FIFO=0, ALGO_SJF=1, ALGO_SJRF=2, ALGO_RR=3)
     * @param quantum    : durée du quantum pour RR (ignoré sinon)
     * @param export_csv : 1 pour sauvegarder en CSV, 0 sinon
     * @return 0 si tout s'est bien passé
     */
    int temps = 0;
    int ticks_restants  = 0;
    Liste l;
    Liste courant       = NULL;
    int cpu_libre       = 1;
    int temps_CPU_actif = 0;

    const char *noms[] = {"FIFO", "SJF", "SJRF", "RR"};

    *liste = trier_par_arrivee(*liste);

    /* Ouverture fichier CSV */
    FILE *csv = NULL;
    if (export_csv){
        char nom_fichier[50];
        sprintf(nom_fichier, "ordonnancement_%s.csv", noms[algo]);
        csv = fopen(nom_fichier, "w");
        if (csv == NULL){
            printf("Erreur : impossible de créer le fichier CSV\n");
            return -1;
        }
        fprintf(csv, "=== Ordonnancement %s ===\n\n", noms[algo]);
        if (algo == ALGO_RR)
            fprintf(csv, "Quantum\t%d\n\n", quantum);
        ecrire_entete(csv, *liste);
    }

    printf("=== Ordonnancement %s ===\n\n", noms[algo]);
    if (algo == ALGO_RR)
        printf("Quantum : %d\n\n", quantum);

    printf("Legende : "
           BLEU "CPU" NORMAL " = en_cours  "
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
            temps_CPU_actif++;

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
                    courant->proc.etat  = 4;
                    courant->proc.temps_fin = temps;
                    courant = NULL;
                }

                cpu_libre      = 1;
                courant        = NULL;
                ticks_restants = 0;
            }
            else if (algo == ALGO_RR && ticks_restants == 0){
                courant->proc.etat = 6;
                cpu_libre          = 1;
                courant            = NULL;
            }
        }

        /* préemption SJRF */
        if (algo == ALGO_SJRF && !cpu_libre && courant != NULL){
            Liste plus_court = chercher_sjrf(*liste);

            if (plus_court != NULL && plus_court != courant && get_DureeCycle(plus_court->proc.CPU) < get_DureeCycle(courant->proc.CPU)){

                courant->proc.etat = 1;
                cpu_libre          = 1;
                courant            = NULL;
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
                cpu_libre          = 0;
                ticks_restants     = quantum;

                if (!courant->proc.temps_premier_CPU_verif){
                    courant->proc.temps_premier_CPU      = temps;
                    courant->proc.temps_premier_CPU_verif = 1;
                }
            }
        }

        afficher_ligne(*liste, temps);

        if (export_csv)
            ecrire_ligne(csv, *liste, temps);

        l = *liste;

        /*  Passage état 4 (FIN) en état 5
            Passage état 6 (attente RR) en état 1
        */
        while (l != NULL){
            if (l->proc.etat == 4){
                l->proc.etat = 5;
            }
            if (l->proc.etat == 6){
                l->proc.etat = 1;
            }
            l = l->suivant;
        }

        temps++;
        sleep(1);
    }

    l = *liste;
    afficher_restitution(l);
    l = *liste;
    afficher_reponse(l);
    l = *liste;
    afficher_attente(l);
    l = *liste;
    afficher_restitution_moyen(l);
    l = *liste;
    afficher_reponse_moyen(l);
    l = *liste;
    afficher_attente_moyen(l);
    l = *liste;
    afficher_occupation(temps_CPU_actif, temps, l);

    if (export_csv){
        ecrire_indicateurs(csv, *liste, temps_CPU_actif, temps);
        fclose(csv);
        printf("\nRésultats sauvegardés dans ordonnancement_%s.csv\n",
               noms[algo]);
    }

    printf("\n==========\n\n");
    printf("Tous les processus sont terminés !\n\n");
    return 0;
}