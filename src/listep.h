#include "ProcStruct.h"

typedef struct celluleP{
    Processus *p;
    struct celluleP *suiv;
}*ListeP;

ListeP inserQueueP(ListeP l, Processus *p){
    ListeP n = malloc(sizeof(struct celluleP));
    n->p = p;
    n->suiv = NULL;

    if(!l) return n;

    ListeP cur = l;
    while(cur->suiv) cur = cur->suiv;
    cur->suiv = n;
    return l;
}

Processus* popTeteP(ListeP *l){
    if(!*l) return NULL;
    ListeP tmp = *l;
    Processus *p = tmp->p;
    *l = tmp->suiv;
    free(tmp);
    return p;
}
