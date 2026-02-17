#include <stdio.h>
#include <stdlib.h>

#include "FIFO.h"

int main(int argc,char* argv){
    int i;
    Processus p1,p2;
    Liste liste = initListe();
    p1.pid = 1;
    p2.pid = 2;
    p1.arrive = 0;
    p2.arrive = 1;
    p1.etat = p2.etat = 0;
    p1.CPU = p2.CPU = initCycle();
    p1.ES = p2.ES = initCycle();

    for(i=1;i<4;i++){
        p1.CPU = inserDureeCycle(p1.CPU,i);
        p2.CPU = inserDureeCycle(p2.CPU,i+1);
    }

    p1.ES = inserDureeCycle(p1.ES,2);
    p1.ES = inserDureeCycle(p1.ES,3);
    p2.ES = inserDureeCycle(p2.ES,2);
    p2.ES = inserDureeCycle(p2.ES,3);

    liste = inserProcListe(liste,p1);
    liste = inserProcListe(liste,p2);

    FIFO(liste);
}
