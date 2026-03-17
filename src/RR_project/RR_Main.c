// main_RR.c
#include <stdio.h>
#include <stdlib.h>

#include "RR.h"
int main(){
    int i;
    Processus p1,p2;
    Processus tab[2];
    p1.arrive = 0;
    p2.arrive = 1;
    p1.etat = p2.etat = 0;
    p1.CPU = p2.CPU = initCycle();
    p1.ES = p2.ES = initCycle();

    for(i=1;i<4;i++){
        p1.CPU = inserDureeCycle(p1.CPU,i);
        p2.CPU = inserDureeCycle(p2.CPU,i+1);
    }

    inserDureeCycle(p1.ES,2);
    inserDureeCycle(p1.ES,3);
    inserDureeCycle(p2.ES,2);
    inserDureeCycle(p2.ES,3);

    tab[0] = p1;
    tab[1] = p2;
    RR(tab, 2, 2);
    
    return 0;
}


