#include "SJRF.h"

Liste chercher_sjrf(Liste l){
    /**
     * Parcourt une liste de processus et renvoie le processus prêt ou en cours avec le plus petit temps d'éxecution,
     * prendre en priorité les nouveaux arrivés si le temps d'éxecution est plus petit
     * @param l : La liste de processus
     * @return Le processus avec le temps d'éxecution le plus court, NULL sinon
     */
    Liste ll = l;
    Liste meilleur = NULL;

    while (ll != NULL){
        if (est_pret(ll->proc) || est_en_cours(ll->proc)){
            if (meilleur == NULL ||
                get_DureeCycle(ll->proc.CPU) < get_DureeCycle(meilleur->proc.CPU)){
                meilleur = ll;
            }
        }
        ll = ll->suivant;
    }
    return meilleur;
}