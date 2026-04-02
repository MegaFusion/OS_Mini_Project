#include "lecture_fichier.h"
#include <string.h>

Liste lire_processus(const char *chemin){
    /**
     * Lit un fichier de processus et retourne une liste de processus
     * Format : pid arrive CPU... / ES...
     * @param chemin : chemin vers le fichier
     * @return la liste des processus lus
     */
    FILE *f = fopen(chemin, "r");
    Processus p;

    if (f == NULL){
        printf("Erreur : impossible d'ouvrir le fichier %s\n", chemin);
        exit(1);
    }

    Liste liste = initListe();
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), f)){

        /* ignorer les commentaires et lignes vides */
        if (ligne[0] == '#' || ligne[0] == '\n')
            continue;

        int pid, arrive;
        int tab_CPU[40];
        int tab_ES[40];
        int taille_CPU = 0;
        int taille_ES  = 0;

        /* lecture pid et arrive */
        char *tok = strtok(ligne, " \t");
        if (tok == NULL) continue;
        pid = atoi(tok);

        tok = strtok(NULL, " \t");
        if (tok == NULL) continue;
        arrive = atoi(tok);
        
        /* lecture tableau CPU jusqu'au '/' */
        int section_ES = 0;
        while ((tok = strtok(NULL, " \t\n")) != NULL){
            if (strcmp(tok, "/") == 0){
                section_ES = 1;
                continue;
            }
            if (!section_ES){
                tab_CPU[taille_CPU] = atoi(tok);
                taille_CPU++;
            }
            else{
                tab_ES[taille_ES] = atoi(tok);
                taille_ES++;
            }
        }

        p = initProc(pid, arrive, taille_CPU  > 0 ? tab_CPU  : NULL, taille_CPU, taille_ES > 0 ? tab_ES : NULL, taille_ES);

        liste = inserProcListe(liste, p);
    }

    fclose(f);
    return liste;
}
