#include <stdio.h>

#include "csv_export.h"

/**
 * Exporte les résultats de simulation dans un fichier CSV
 * @param chemin : chemin du fichier
 * @param r : résultat à exporter
 */
void exporter_csv_gui(const char *chemin, SimulationResultat *r) {
    if (!r) return;

    FILE *f = fopen(chemin, "w");
    if (!f) return;

    for (int j = 0; j < r->nb_colonnes; j++) {
        fprintf(f, "%s", r->entetes[j]);
        if (j < r->nb_colonnes - 1) fprintf(f, "\t");
    }
    fprintf(f, "\n");

    for (int i = 0; i < r->nb_lignes; i++) {
        for (int j = 0; j < r->nb_colonnes; j++) {
            fprintf(f, "%s", r->cellules[i][j]);
            if (j < r->nb_colonnes - 1) fprintf(f, "\t");
        }
        fprintf(f, "\n");
    }

    fprintf(f, "\n");
    fprintf(f, "Indicateur");
    for (int i = 0; i < r->nb_processus; i++) {
        fprintf(f, "\tP%d", r->pid[i]);
    }
    fprintf(f, "\n");

    fprintf(f, "Restitution");
    for (int i = 0; i < r->nb_processus; i++) {
        fprintf(f, "\t%d", r->restitution[i]);
    }
    fprintf(f, "\n");

    fprintf(f, "Attente");
    for (int i = 0; i < r->nb_processus; i++) {
        fprintf(f, "\t%d", r->attente[i]);
    }
    fprintf(f, "\n");

    fprintf(f, "Reponse");
    for (int i = 0; i < r->nb_processus; i++) {
        fprintf(f, "\t%d", r->reponse[i]);
    }
    fprintf(f, "\n");

    fprintf(f, "Restitution_M\t%.2f\n", r->restitution_moyenne);
    fprintf(f, "Attente_M\t%.2f\n", r->attente_moyenne);
    fprintf(f, "Reponse_M\t%.2f\n", r->reponse_moyenne);
    fprintf(f, "T_occupation\t%.2f\n", r->occupation_cpu);

    fclose(f);
}