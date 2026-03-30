#include "ordonnancement.h"


int main() {

    Liste liste = initListe();

    Processus p1,p2,p3;
    int ES1[] = {2};
    int CPU1[] = {2,2};

    int ES2[] = {3};
    int CPU2[] = {2,3};

    int ES3[] = {4,2,2};
    int CPU3[] = {2,2,1,3};

    p1 = initProc(1, 3, CPU1, 2, ES1, 1);
    p2 = initProc(2, 0, CPU2, 2, ES2, 1);
    p3 = initProc(3, 1, CPU3, 4, ES3, 3);

    liste = inserProcListe(liste,p1);
    liste = inserProcListe(liste,p2);
    liste = inserProcListe(liste,p3);

    ordonnancer(&liste, 1, 0);
    printf("\n");

    return 0;
}