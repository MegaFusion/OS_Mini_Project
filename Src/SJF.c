#include "SJF.h"

Liste chercher_sjf(Liste l){
    /**
     * Parcourt une liste de processus et renvoie le processus prêt avec le plus petit temps d'éxecution
     * @param l : La liste de processus
     * @return Le processus prêt avec le temps d'éxecution le plus court, NULL sinon
     */
    Liste ll = l;
    Liste meilleur = NULL;

    while (ll != NULL){
        if (est_pret(ll->proc)){
            if (meilleur == NULL ||
                get_DureeCycle(ll->proc.CPU) < get_DureeCycle(meilleur->proc.CPU)){
                meilleur = ll;
            }
        }
        ll = ll->suivant;
    }
    return meilleur;
}