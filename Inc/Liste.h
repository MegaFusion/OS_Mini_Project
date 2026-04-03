#ifndef LISTE_H
#define LISTE_H

#include "ProcStruct.h"

typedef struct celluleListe {
    /**
     * Structure d'une liste chainée
     * @var proc : processus stocké 
     * @var suivant : cellule suivante
     */

    Processus proc;
    struct celluleListe* suivant;
}*Liste;

int estVideListe(Liste l);
Liste initListe();
Processus procListe(Liste l);
Liste suivantListe(Liste l);
void supTeteListe(Liste *l);
Liste inserTeteListe(Liste l, Processus p);
Liste inserProcListe(Liste l,Processus p);
int supElement(Liste* l,int pid);

#endif