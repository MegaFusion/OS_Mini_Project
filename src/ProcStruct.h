
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
    return c->suivant;
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
        printf("le cycle est deja vide\n");
        exit(1);
    }
    cc = *c;
    *c = suivantCycle(*c);
    free(cc);
}

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

    */

}Processus;

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