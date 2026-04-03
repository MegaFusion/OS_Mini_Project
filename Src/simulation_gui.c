#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simulation_gui.h"
#include "Liste.h"
#include "ProcStruct.h"
#include "Calcul_temps.h"
#include "SJF.h"
#include "SJRF.h"
#include "ordonnancement.h"

/**
 * Duplique une chaine de caractères
 * @param s : chaine à copier
 * @return nouvelle chaine allouée
 */
static char *dupliquer_chaine(const char *s) {
    char *copie = malloc(strlen(s) + 1);
    if (!copie) {
        fprintf(stderr, "Erreur allocation memoire\n");
        exit(1);
    }
    strcpy(copie, s);
    return copie;
}

/**
 * Compte le nombre de processus dans une liste
 * @param l : liste des processus
 * @return nombre de processus
 */
static int compter_processus(Liste l) {
    int n = 0;
    while (l != NULL) {
        n++;
        l = l->suivant;
    }
    return n;
}

/**
 * Libère toute la mémoire associée au résultat de simulation
 * @param r : résultat à libérer
 */
void liberer_resultat(SimulationResultat *r) {
    if (!r) return;

    if (r->cellules) {
        for (int i = 0; i < r->nb_lignes; i++) {
            if (r->cellules[i]) {
                for (int j = 0; j < r->nb_colonnes; j++) {
                    free(r->cellules[i][j]);
                }
                free(r->cellules[i]);
            }
        }
        free(r->cellules);
    }

    if (r->entetes) {
        for (int i = 0; i < r->nb_colonnes; i++) {
            free(r->entetes[i]);
        }
        free(r->entetes);
    }

    free(r->pid);
    free(r->restitution);
    free(r->attente);
    free(r->reponse);
    free(r);
}

/**
 * Initialise une structure de résultat vide
 * @param liste : liste des processus
 * @return structure initialisée
 */
static SimulationResultat *creer_resultat_vide(Liste liste) {
    SimulationResultat *r = calloc(1, sizeof(SimulationResultat));
    if (!r) {
        fprintf(stderr, "Erreur allocation resultat\n");
        exit(1);
    }

    r->nb_processus = compter_processus(liste);
    r->nb_colonnes = r->nb_processus + 1;
    r->nb_lignes = 0;

    r->cellules = NULL;
    r->entetes = malloc(sizeof(char *) * r->nb_colonnes);
    r->pid = malloc(sizeof(int) * r->nb_processus);
    r->restitution = malloc(sizeof(int) * r->nb_processus);
    r->attente = malloc(sizeof(int) * r->nb_processus);
    r->reponse = malloc(sizeof(int) * r->nb_processus);

    if (!r->entetes || !r->pid || !r->restitution || !r->attente || !r->reponse) {
        fprintf(stderr, "Erreur allocation tableaux resultat\n");
        exit(1);
    }

    r->entetes[0] = dupliquer_chaine("Temps");

    Liste l = liste;
    int i = 0;
    while (l != NULL) {
        char nom[32];
        snprintf(nom, sizeof(nom), "P%d", l->proc.pid);
        r->entetes[i + 1] = dupliquer_chaine(nom);
        r->pid[i] = l->proc.pid;
        i++;
        l = l->suivant;
    }

    return r;
}

/**
 * Convertit l'état d'un processus en texte affichable
 * @param etat : entier représentant l'état
 * @return chaine correspondante
 */
static const char *etat_vers_texte(int etat) {
    switch (etat) {
        case 0: return "---";
        case 1: return "PRT";
        case 2: return "CPU";
        case 3: return "BLQ";
        case 4: return "FIN";
        case 5: return "";
        case 6: return "PRT";
        default: return "???";
    }
}

/**
 * Ajoute une ligne dans le tableau des résultats
 * @param r : résultat
 * @param liste : état actuel des processus
 * @param temps : temps courant
 */
static void ajouter_ligne_resultat(SimulationResultat *r, Liste liste, int temps) {
    r->cellules = realloc(r->cellules, sizeof(char **) * (r->nb_lignes + 1));
    if (!r->cellules) {
        fprintf(stderr, "Erreur realloc\n");
        exit(1);
    }

    r->cellules[r->nb_lignes] = malloc(sizeof(char *) * r->nb_colonnes);
    if (!r->cellules[r->nb_lignes]) {
        fprintf(stderr, "Erreur allocation ligne\n");
        exit(1);
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%d", temps);
    r->cellules[r->nb_lignes][0] = dupliquer_chaine(buffer);

    Liste l = liste;
    int col = 1;
    while (l != NULL) {
        r->cellules[r->nb_lignes][col] = dupliquer_chaine(etat_vers_texte(l->proc.etat));
        col++;
        l = l->suivant;
    }

    r->nb_lignes++;
}

/**
 * Trie les processus par ordre d'arrivée
 * @param l : liste non triée
 * @return liste triée
 */
static Liste trier_par_arrivee_gui(Liste l) {
    Liste triee = NULL;
    Liste courant = l;

    while (courant != NULL) {
        Liste suivant = courant->suivant;
        Processus p = courant->proc;

        if (triee == NULL || p.arrive < triee->proc.arrive) {
            courant->suivant = triee;
            triee = courant;
        } else {
            Liste tmp = triee;
            while (tmp->suivant != NULL && tmp->suivant->proc.arrive <= p.arrive) {
                tmp = tmp->suivant;
            }
            courant->suivant = tmp->suivant;
            tmp->suivant = courant;
        }
        courant = suivant;
    }

    return triee;
}

/**
 * Vérifie si tous les processus sont terminés
 * @param l : liste des processus
 * @return 1 si tous terminés, 0 sinon
 */
static int tous_termines_gui(Liste l) {
    while (!estVideListe(l)) {
        if (!est_termine(l->proc) && l->proc.etat != 5) {
            return 0;
        }
        l = l->suivant;
    }
    return 1;
}

/**
 * Cherche le premier processus prêt
 * @param l : liste des processus
 * @return processus trouvé ou NULL
 */
static Liste chercher_premier_pret_gui(Liste l) {
    Liste ll = l;
    Liste non_prioritaire = NULL;

    while (ll != NULL) {
        if (ll->proc.etat == 6) {
            non_prioritaire = ll;
        }
        if (est_pret(ll->proc)) {
            return ll;
        }
        ll = ll->suivant;
    }

    return non_prioritaire;
}

/**
 * Lance la simulation d'ordonnancement version GUI
 * @param liste : liste des processus
 * @param algo : algorithme choisi
 * @param quantum : quantum pour RR
 * @return structure contenant tous les résultats
 */
SimulationResultat *simuler_gui(Liste *liste, int algo, int quantum) {
    int temps = 0;
    int ticks_restants = 0;
    int cpu_libre = 1;
    int temps_CPU_actif = 0;
    Liste l;
    Liste courant = NULL;

    *liste = trier_par_arrivee_gui(*liste);

    SimulationResultat *res = creer_resultat_vide(*liste);

    while (!tous_termines_gui(*liste)) {
        l = *liste;

        while (l != NULL) {
            if (l->proc.arrive <= temps && non_arrive(l->proc)) {
                l->proc.etat = 1;
            }

            if (est_bloque(l->proc) && !estVideCycle(l->proc.ES)) {
                l->proc.ES->duree--;

                if (get_DureeCycle(l->proc.ES) == 0) {
                    supTeteCycle(&l->proc.ES);
                    l->proc.etat = 1;
                }
            }

            l = l->suivant;
        }

        if (!cpu_libre && courant != NULL) {
            temps_CPU_actif++;

            if (!estVideCycle(courant->proc.CPU)) {
                courant->proc.CPU->duree--;
            }

            ticks_restants--;

            if (get_DureeCycle(courant->proc.CPU) == 0) {
                supTeteCycle(&courant->proc.CPU);

                if (!estVideCycle(courant->proc.ES)) {
                    courant->proc.etat = 3;
                } else {
                    courant->proc.etat = 4;
                    courant->proc.temps_fin = temps;
                }

                cpu_libre = 1;
                courant = NULL;
                ticks_restants = 0;
            } else if (algo == ALGO_RR && ticks_restants == 0) {
                courant->proc.etat = 6;
                cpu_libre = 1;
                courant = NULL;
            }
        }

        if (algo == ALGO_SJRF && !cpu_libre && courant != NULL) {
            Liste plus_court = chercher_sjrf(*liste);

            if (plus_court != NULL &&
                plus_court != courant &&
                get_DureeCycle(plus_court->proc.CPU) < get_DureeCycle(courant->proc.CPU)) {
                courant->proc.etat = 1;
                cpu_libre = 1;
                courant = NULL;
            }
        }

        if (cpu_libre) {
            if (algo == ALGO_FIFO) {
                courant = chercher_premier_pret_gui(*liste);
            } else if (algo == ALGO_SJF) {
                courant = chercher_sjf(*liste);
            } else if (algo == ALGO_SJRF) {
                courant = chercher_sjrf(*liste);
            } else {
                courant = chercher_premier_pret_gui(*liste);
            }

            if (courant != NULL) {
                courant->proc.etat = 2;
                cpu_libre = 0;
                ticks_restants = quantum;

                if (!courant->proc.temps_premier_CPU_verif) {
                    courant->proc.temps_premier_CPU = temps;
                    courant->proc.temps_premier_CPU_verif = 1;
                }
            }
        }

        ajouter_ligne_resultat(res, *liste, temps);

        l = *liste;
        while (l != NULL) {
            if (l->proc.etat == 4) {
                l->proc.etat = 5;
            }
            if (l->proc.etat == 6) {
                l->proc.etat = 1;
            }
            l = l->suivant;
        }

        temps++;
    }

    l = *liste;
    for (int i = 0; i < res->nb_processus; i++) {
        res->restitution[i] = Proc_temps_restitution(l->proc);
        res->reponse[i] = Proc_temps_reponse(l->proc);
        res->attente[i] = Proc_temps_attente(l->proc);
        l = l->suivant;
    }

    res->restitution_moyenne = restitution_moyen(*liste);
    res->reponse_moyenne = reponse_moyen(*liste);
    res->attente_moyenne = attente_moyen(*liste);
    res->occupation_cpu = Taux_occupation(temps_CPU_actif, temps);

    return res;
}