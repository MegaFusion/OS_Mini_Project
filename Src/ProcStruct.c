#include "ProcStruct.h"

Cycle initCycle(){
    /**
     * Initialise un Cycle
     * @return un Cycle vide
     */
    return NULL;
}
int estVideCycle(Cycle c){
    /**
     * Verifie si un cycle est vide
     * @param c : le cycle à verifier
     * @return  : 1 si vide, 0 sinon 
     */
    return (c == NULL);
}
int get_DureeCycle(Cycle c){
    /**
     * Renvoie la durée stockée dans une cellule
     * @param c : pointeur vers la cellule
     * @return la durée
     */
    if (estVideCycle(c)){
        return 0;
    }
    return c->duree;
}
Cycle suivantCycle(Cycle c){
    /**
     * Renvoie la cellule suivante
     * @param c : la cellule courante
     * @return la cellule suivante
     */
    if (!estVideCycle(c))
        return c->suivant;
    return NULL;
}
Cycle inserTeteDureeCycle(Cycle c, int temps){
    /**
     * Insertion en tête d'un cycle
     * @param c     : pointeur sur la cellule du cycle
     * @param temps : la durée contenu dans la cellule
     * @return Le cycle mis à jour
     */
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
    /**
     * Insertion en queue du cycle donné
     * @param c     : pointeur sur la cellule du cycle
     * @param temps : la durée contenu dans la cellule
     * @return Le cycle mis à jour
     */
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
    /**
     * Supprime la tête du cycle donnée
     * @param c : adresse vers cycle
     */
    Cycle cc;
    if (estVideCycle(*c)){
        printf("erreur suppression : le cycle est deja vide\n");
        exit(1);
    }
    cc = *c;
    *c = suivantCycle(*c);
    free(cc);
}

Cycle get_ES(Processus p){
    /**
     * Renvoie le cycle E/S d'un processus
     */
    return p.ES;
}
Cycle get_CPU(Processus p){
    /**
     * Renvoie le cycle CPU d'un processus
     */
    return p.CPU;
}

Cycle creerCycle(int durees[], int taille) {
    /**
     * Initialise un tableau de temps CPU ou ES pour un processus
     * @param durees : tableau de temps
     * @return un Cycle CPU ou ES
     */
    Cycle c = initCycle();
    if (durees == NULL || taille == 0)
        return c;
    for (int i = 0; i < taille; i++) {
        c = inserDureeCycle(c, durees[i]);
    }
    return c;
}

Processus initProc(int pid, int arrive, int* tab_CPU, int taille_CPU, int* tab_ES, int taille_ES){
    /**
     * Initialise un Processus
     * @param pid : le numéro de Processus
     * @param arrive : le temps d'arrivée dans le système
     * @param tab_ES : tableau de temps ES
     * @param tab_CPU : tableau de temps CPU
     * @return Un processus initialisé
     */
    Processus p;
    p.arrive = arrive;
    p.pid = pid;
    p.CPU = creerCycle(tab_CPU,taille_CPU);
    p.ES = creerCycle(tab_ES,taille_ES);
    p.etat = 0;
    p.temps_fin = 0;
    p.temps_premier_CPU = 0;
    p.temps_premier_CPU_verif = 0;

    p.temps_CPU = 0;
    for (int i = 0; i < taille_CPU; i++)
        p.temps_CPU += tab_CPU[i];

    p.temps_ES = 0;
    for (int i = 0; i < taille_ES; i++)
        p.temps_ES += tab_ES[i];
    
    return p;
}

int non_arrive(Processus p){
    /**
     * Verifie si un processus n'est pas encore arrivé dans le système
     */
    return (p.etat == 0);
}
int est_pret(Processus p){
    /**
     * Verifie si un processus est prêt
     */
    return (p.etat == 1);
}
int est_en_cours(Processus p){
    /**
     * Verifie si un processus est en_cours
     */
    return (p.etat == 2);
}
int est_bloque(Processus p){
    /**
     * Verifie si un processus est bloqué
     */
    return (p.etat == 3);
}
int est_termine(Processus p){
    /**
     * Verifie si un processus est terminé
     */
    return (p.etat == 4);
}
