#include "SJF.h"
#include "listep.h"
#include <stdio.h>
#include <stdlib.h>

int peek_cpu(Processus *p){
    return get_DureeCycle(p->CPU);
}

void charger_cpu(Processus *p){
    if(p->cpu_rest == 0)
        p->cpu_rest = peek_cpu(p);
}

void charger_io(Processus *p){
    if(p->io_rest == 0)
        p->io_rest = get_DureeCycle(p->ES);
}

void ajouter_arrives(Processus tab[], int n, int t, ListeP *pret){
    for(int i=0;i<n;i++){
        if(tab[i].etat == 0 && tab[i].arrive == t){
            tab[i].etat = 1;
            *pret = inserQueueP(*pret, &tab[i]);
        }
    }
}

Processus* min_sjf(ListeP *pret){
    if(!*pret) return NULL;

    ListeP cur = *pret;
    ListeP prev = NULL;
    ListeP meilleurPrev = NULL;
    ListeP meilleur = cur;

    int min = peek_cpu(cur->p);

    while(cur){
        int d = peek_cpu(cur->p);
        if(d < min){
            min = d;
            meilleur = cur;
            meilleurPrev = prev;
        }
        prev = cur;
        cur = cur->suiv;
    }

    if(!meilleurPrev)
        *pret = meilleur->suiv;
    else
        meilleurPrev->suiv = meilleur->suiv;

    Processus *res = meilleur->p;
    free(meilleur);
    return res;
}

void avancer_io(ListeP *bloques, ListeP *pret){
    ListeP cur = *bloques;
    ListeP prev = NULL;

    while(cur){
        Processus *p = cur->p;
        charger_io(p);
        p->io_rest--;

        if(p->io_rest == 0){
            supTeteCycle(&p->ES);
            p->etat = 1;

            ListeP tmp = cur;
            if(!prev) *bloques = cur->suiv;
            else prev->suiv = cur->suiv;
            cur = cur->suiv;
            free(tmp);

            *pret = inserQueueP(*pret, p);
        } else {
            prev = cur;
            cur = cur->suiv;
        }
    }
}

void SJF(Processus tab[], int n){
    ListeP pret = NULL, bloques = NULL;
    Processus *courant = NULL;
    int t = 0;

    while(1){
        int fini = 1;
        for(int i=0;i<n;i++)
            if(tab[i].etat != 4) fini = 0;
        if(fini) break;

        ajouter_arrives(tab,n,t,&pret);
        avancer_io(&bloques,&pret);

        if(!courant){
            courant = min_sjf(&pret);
            if(courant){
                courant->etat = 2;
                charger_cpu(courant);
            }
        }

        if(courant){
            courant->cpu_rest--;
            if(courant->cpu_rest == 0){
                supTeteCycle(&courant->CPU);
                if(!estVideCycle(courant->ES)){
                    courant->etat = 3;
                    courant->io_rest = 0;
                    bloques = inserQueueP(bloques,courant);
                } else {
                    courant->etat = 4;
                }
                courant = NULL;
            }
        }

        t++;
    }

    printf("SJF terminé en %d ms\n", t);
}
