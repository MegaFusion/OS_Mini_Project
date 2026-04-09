#include <unistd.h>
#include "File.h"

Liste chercher_premier_pret(Liste l){
    /**
     * Parcourt une liste de processus et renvoie le premier processus prêt
     * @param l : La liste de processus
     * @return Si un processus prêt trouvé, renvoie ce dernier. NULL sinon
     */
    Liste ll = l;
    while (ll != NULL){
        if (est_pret(ll->proc))
            return ll;
        ll = ll->suivant;
    }
    return NULL;
}
int tous_termines_file(Liste l){
    /**
     * Verifie si tous les processus d'une liste sont tous terminés
     * @param l : Liste de processus
     * @return 1 si oui, 0 sinon
     */
    Liste ll = l;
    while (!estVideListe(ll)){
        if (!est_termine(ll->proc)){
            return 0;
        }
        ll = ll->suivant;
    }
    return 1;
}


int FIFO(Liste liste){
    /**
     * Ordonnancement FIFO d'une liste de processus.
     * @param liste : la liste de processus
     * @return 0 si tout s'est bien passé
     */
    int temps = 0;
    int pid;
    Liste l;
    Liste courant = NULL;
    int cpu_libre = 1;

    while (!tous_termines_file(liste)){

        l = liste;

        while (l != NULL){
            if (l->proc.arrive <= temps && non_arrive(l->proc)){
                printf("Processus %d arrivé dans le système\n",l->proc.pid);
                l->proc.etat = 1;
            }

            if (est_bloque(l->proc) && !estVideCycle(l->proc.ES)){
                l->proc.ES->duree--;

                if (get_DureeCycle(l->proc.ES) == 0){
                    supTeteCycle(&l->proc.ES);
                    l->proc.etat = 1;
                }
            }

            l = l->suivant;
        }

        if (cpu_libre){

            courant = chercher_premier_pret(liste);

            if (courant != NULL){
                courant->proc.etat = 2;
                pid = courant->proc.pid;
                cpu_libre = 0;
                printf("Processus %d lancé\n", courant->proc.pid);
            }
        }

        if (!cpu_libre && courant != NULL){

            printf("Processus %d en cours\t temps : %d\n",
                   courant->proc.pid, temps);

            if (!estVideCycle(courant->proc.CPU)){
                courant->proc.CPU->duree--;
            }

            if (get_DureeCycle(courant->proc.CPU) == 0){

                supTeteCycle(&courant->proc.CPU);

                if (!estVideCycle(courant->proc.ES)){
                    courant->proc.etat = 3;
                    printf("Processus %d bloqué\n", courant->proc.pid);
                }
                else{
                    courant->proc.etat = 4;
                    printf("Processus %d terminé\n", courant->proc.pid);
                    supElement(liste, pid);
                }

                cpu_libre = 1;
                courant = NULL;
            }
        }

        temps++;
        sleep(1);
    }

    printf("Tous les processus sont terminés\n");
    return 0;
}


