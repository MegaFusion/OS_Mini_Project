
#ifndef PROCSTRUCT_H
#define PROCSTRUCT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct cellule{
    /**
     * Structure d'un cycle contenant une liste de durée
     * @var duree   : temps
     * @var suivant : cellule suivante
     */
    int duree;
    struct cellule* suivant;
}*Cycle;


typedef struct {
    /**
     * Structure d'un processus
     * @var pid     : numéro du processus
     * @var arrive  : temps d'arrivé dans le système
     * @var ES      : Cycle entrées/sorties
     * @var CPU     : Cycle CPU
     * @var etat    : état actuel du processus
     */
    int pid;
    int arrive;
    Cycle ES;
    Cycle CPU;
    int etat;   /* état du processus - 0 : non arrivé
                                     - 1 : pret
                                     - 2 : en_cours
                                     - 3 : bloqué
                                     - 4 : terminé
                                     - 5 : terminé ( pour l'affichage )
                                     - 6 : prêt mais en attente après quantum épuisé
                */
    int temps_fin;
    int temps_CPU;
    int temps_ES;
    int temps_premier_CPU;
    int temps_premier_CPU_verif;    /* Pour verifier si le tp_CPU a déja été affecté */

}Processus;

Cycle initCycle();
int estVideCycle(Cycle c);
int get_DureeCycle(Cycle c);
Cycle suivantCycle(Cycle c);
Cycle inserTeteDureeCycle(Cycle c, int temps);
Cycle inserDureeCycle(Cycle c,int temps);
void supTeteCycle(Cycle *c);
Cycle get_ES(Processus p);
Cycle get_CPU(Processus p);
Cycle creerCycle(int durees[], int taille);
int non_arrive(Processus p);
int est_pret(Processus p);
int est_en_cours(Processus p);
int est_bloque(Processus p);
int est_termine(Processus p);
Processus initProc(int pid, int arrive, int* tab_CPU,int taille_CPU, int* tab_ES, int taille_ES);

#endif