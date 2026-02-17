#include <stdio.h>
#include <stdlib.h>

typedef struct cellule{
    int duree;
    struct cellule* suivant;
}*Cycle;

Cycle initCycle(){
    return NULL;
}
int estVideCycle(Cycle c){
    return (c == NULL);
}
int get_DureeCycle(Cycle c){
    if (estVideCycle(c)){
        return 0;
    }
    return c->duree;
}
Cycle suivantCycle(Cycle c){
    return c->suivant;
}
Cycle inserTeteDureeCycle(Cycle c, int temps){
    Cycle tmp = malloc (sizeof(struct cellule));
    if (tmp == NULL){
        printf("Erreur d'allocation\n");
        exit(1);
    }
    tmp->duree = temps;
    tmp->suivant = c;
    c = tmp;
    return c;
}
Cycle inserDureeCycle(Cycle c,int temps){
    Cycle cc = c;
    if (estVideCycle(c)){
        return inserTeteDureeCycle(c,temps);
    }
    while (!estVideCycle(cc->suivant)){
        cc = cc->suivant;
    }
    cc->suivant = inserTeteDureeCycle(cc->suivant,temps);
    return c;
}

void supTeteCycle(Cycle *c){
    Cycle cc;
    if (estVideCycle(*c)){
        printf("le cycle est deja vide\n");
        exit(1);
    }
    cc = *c;
    *c = suivantCycle(*c);
    free(cc);
}

typedef struct {
    int arrive;
    Cycle ES;     /* Durée E/S */
    Cycle CPU;    /* Durée CPU */
    int etat;   /* état du processus - 0 : non arrivé
                                     - 1 : pret
                                     - 2 : en_cours
                                     - 3 : bloqué
                                     - 4 : terminé  

    */

}Processus;

Cycle get_ES(Processus p){
    return p.ES;
}
Cycle get_CPU(Processus p){
    return p.CPU;
}
int non_arrive(Processus p){
    return (p.etat == 0);
}
int est_pret(Processus p){
    return (p.etat == 1);
}
int est_en_cours(Processus p){
    return (p.etat == 2);
}
int est_bloque(Processus p){
    return (p.etat == 3);
}
int est_termine(Processus p){
    return (p.etat == 4);
}