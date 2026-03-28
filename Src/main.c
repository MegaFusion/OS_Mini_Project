#include "ordonnancement.h"

/**
 * Crée un cycle à partir d'un tableau de durées
 * Exemple : {3, 2, 0} crée un cycle avec 2 durées : 3 puis 2
 * (le 0 marque la fin du tableau)
 */
Cycle creerCycle(int durees[]) {
    Cycle c = initCycle();
    int i = 0;
    while (durees[i] != 0) {
        c = inserDureeCycle(c, durees[i]);
        i++;
    }
    return c;
}

Liste creer_liste(){
    /**
     * Crée et retourne une liste de 3 processus de test
     * @return la liste des processus
     */
    Liste liste = initListe();

    /*
     * Processus 1 :
     *   - Arrive à t=0
     *   - CPU : 3 unités, puis 2 unités
     *   - E/S : 2 unités (entre les deux bursts CPU)
     */
    Processus p1;
    p1.pid    = 1;
    p1.arrive = 0;
    p1.etat   = 0;
    int cpu1[] = {3, 2, 0};
    int es1[]  = {2, 0};
    p1.CPU = creerCycle(cpu1);
    p1.ES  = creerCycle(es1);

    /*
     * Processus 2 :
     *   - Arrive à t=1
     *   - CPU : 5 unités (pas d'E/S)
     */
    Processus p2;
    p2.pid    = 2;
    p2.arrive = 1;
    p2.etat   = 0;
    int cpu2[] = {5, 0};
    int es2[]  = {0};
    p2.CPU = creerCycle(cpu2);
    p2.ES  = creerCycle(es2);

    /*
     * Processus 3 :
     *   - Arrive à t=2
     *   - CPU : 2 unités, puis 3 unités
     *   - E/S : 1 unité (entre les deux bursts CPU)
     */
    Processus p3;
    p3.pid    = 3;
    p3.arrive = 2;
    p3.etat   = 0;
    int cpu3[] = {2, 3, 0};
    int es3[]  = {1, 0};
    p3.CPU = creerCycle(cpu3);
    p3.ES  = creerCycle(es3);

    liste = inserProcListe(liste, p1);
    liste = inserProcListe(liste, p2);
    liste = inserProcListe(liste, p3);

    return liste;
}

int main() {

    Liste liste;

    printf("\n");

    liste = creer_liste();
    ordonnancer(&liste, ALGO_FIFO, 0);

    liste = creer_liste();
    ordonnancer(&liste, ALGO_SJF, 0);

    liste = creer_liste();
    ordonnancer(&liste, ALGO_SJRF, 0);

    liste = creer_liste();
    ordonnancer(&liste, ALGO_RR, 2);

    return 0;
}