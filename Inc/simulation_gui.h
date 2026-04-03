#ifndef SIMULATION_GUI_H
#define SIMULATION_GUI_H

#include "Liste.h"

/**
 * Structure contenant les résultats de la simulation
 */
typedef struct {
    int nb_colonnes;
    int nb_lignes;
    char ***cellules;
    char **entetes;

    int nb_processus;
    int *pid;
    int *restitution;
    int *attente;
    int *reponse;

    float restitution_moyenne;
    float attente_moyenne;
    float reponse_moyenne;
    float occupation_cpu;
} SimulationResultat;

SimulationResultat *simuler_gui(Liste *liste, int algo, int quantum);
void liberer_resultat(SimulationResultat *r);

#endif