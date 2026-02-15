#include <unistd.h>
#include "ProcStruct.h"

int tous_termines(Processus* tab, int n){
    for(int i = 0; i < n; i++){
        if(!est_termine(tab[i]))
            return 0;
    }
    return 1;
}

int chercher_premier_pret(Processus* tab, int n){
    for(int i = 0; i < n; i++){
        if(est_pret(tab[i]))
            return i;
    }
    return -1;
}

int FIFO(Processus* tab, int nbProcs){
    int temps = 0;
    int i;
    int proc = -1;

    while (!tous_termines(tab,nbProcs)){
        for (i=0;i<nbProcs;i++){    // Active ceux qui sont arrivés
            if (tab[i].arrive <= temps && non_arrive(tab[i])){
                tab[i].etat = 1;
            }
        }

        if(proc == -1){     // Prends le prochain Processus prêt selon l'ordre du tab
            proc = chercher_premier_pret(tab,nbProcs);
            if (proc != -1){
                printf("Processus %d prêt à être lancé\n",proc+1);
                tab[proc].etat = 2;
            }
        }

        if(proc != -1){     // Gestion des état des processus
            printf("Processus en cours : %d\t temps : %d\n",proc+1,temps);
            get_CPU(tab[proc])->duree--;

            if(get_CPU(tab[proc])->duree == 0){

                supTeteCycle(&tab[proc].CPU);

                if(get_ES(tab[proc]) != NULL){
                    printf("Processus %d bloqué\n",proc+1);
                    tab[proc].etat = 3;
                }
                else{
                    printf("Processus %d terminé\n",proc+1);
                    tab[proc].etat = 4;
                }

                proc = -1;
            }
        }

        for (i=0;i<nbProcs;i++){    // Avance les E/S
            if(est_bloque(tab[i])){
                printf("Processus %d en E/S \t ES : %d\n",i+1,get_DureeCycle(get_ES(tab[i])));
                tab[i].ES->duree--;

                if (get_DureeCycle(get_ES(tab[i])) == 0){
                    supTeteCycle(&tab[i].ES);
                    tab[i].etat = 1;
                }
            }
        }
        temps++;
        sleep(5);
    }
    printf("Tous les processus sont traités ! temps : %d\n",temps);
    return 0;
}
