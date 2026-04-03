#include "Liste.h"

int estVideListe(Liste l){
    /**
     * Verifie si la liste est vide, renvoie Vrai si vide ,Faux sinon
     * @param l : La Liste à vérifier
     * @return 1 ou 0
     */
    return l == NULL;
}

Liste initListe(){
    /**
     * Initialise une liste vide
     * @return une liste vide (NULL)
     */
    return NULL;
}

Processus procListe(Liste l){
    /**
     * Renvoie le processus de la cellule en paramètre
     * @param l : pointeur sur la cellule
     * @return le processus courant
     */
    return l->proc;
}

Liste suivantListe(Liste l){
    /**
     * Renvoie la cellule suivante de la cellule en paramètre
     * @param l : pointeur sur la cellule
     * @return la cellule suivante
     */
    return l->suivant;
}

void supTeteListe(Liste *l){
    /**
     * Supprime la tete de la liste, passage par adresse
     * @param l : adresse de la liste de processus
     */
    Liste ll;
    if (estVideListe(*l)){
        printf("la liste est deja vide\n");
        exit(0);
    }
    ll = *l;
    *l = suivantListe(*l);
    free(ll);
}

Liste inserTeteListe(Liste l, Processus p){
    /**
     * Ajoute une cellule à la tête de la liste donnée
     * @param l : pointeur sur la liste
     * @param p : le processus contenu dans la cellule
     * @return La liste mis à jour
     */
    Liste tmp = (Liste)malloc (sizeof(struct celluleListe));
    if (tmp == NULL){
        printf("Erreur d'allocation liste\n");
        exit(1);
    }
    tmp->proc = p;
    tmp->suivant = l;
    l = tmp;
    return l;
}
Liste inserProcListe(Liste l,Processus p){
    /**
     * Insertition en queue d'une liste
     * @param l : la Liste de processus
     * @param p : le processus à ajouter
     * @return La liste mis à jour
     */
    Liste ll = l;
    if (estVideListe(l)){
        return inserTeteListe(l,p);
    }
    while (!estVideListe(ll->suivant)){
        ll = ll->suivant;
    }
    ll->suivant = inserTeteListe(ll->suivant,p);
    return l;
}

int supElement(Liste* l,int pid){
    /**
     * Supprime une cellule dans une liste de processus
     * @param l     : La liste de processus
     * @param pid   : numéro du processus
     * @return Une verification : 1 si le processus est trouvé, 0 sinon
     */
    if (estVideListe(*l)){
        return 0;
    }
    if ((*l)->proc.pid == pid){
        supTeteListe(l);
        return 1;
    }
    else{
        return supElement(&(*l)->suivant,pid);
    }
}