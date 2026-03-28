#include "RR.h"

Liste chercher_rr(Liste l){
    /**
     * Parcourt une liste de processus et renvoie le premier processus prêt
     * (même logique que FIFO — le RR se distingue par la gestion du quantum)
     * @param l : La liste de processus
     * @return Le premier processus prêt trouvé, NULL sinon
     */
    Liste ll = l;
    while (ll != NULL){
        if (est_pret(ll->proc))
            return ll;
        ll = ll->suivant;
    }
    return NULL;
}