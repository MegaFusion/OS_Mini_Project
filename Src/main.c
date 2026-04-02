#include "ordonnancement.h"
#include "lecture_fichier.h"
#include <string.h>

void afficher_aide(){
    /**
     * Afficher le texte d'aide pour l'utilisation en invité de commande après l'option -h
     */
    printf("Usage : ./main -a <ALGO> [options]\n\n");
    printf("Algorithmes :\n");
    printf("  -a FIFO          Ordonnancement FIFO\n");
    printf("  -a SJF           Ordonnancement SJF\n");
    printf("  -a SJRF          Ordonnancement SJRF\n");
    printf("  -a RR            Ordonnancement RR\n\n");
    printf("Options :\n");
    printf("  -q <quantum>     Quantum pour RR (par défaut : 2)\n");
    printf("  -f <fichier>     Charger les processus depuis un fichier\n");
    printf("  -csv             Sauvegarder les resultats en fichier CSV\n");
    printf("  -h               Afficher cette aide\n\n");
    printf("Exemple :\n");
    printf(" ./main -a RR -q 3 -f processus.txt -csv\n\n");
}

Liste creer_liste_defaut(){
    /**
     * Crée une liste de processus par défaut pour les tests
     * @return la liste des processus
     */
    Liste liste = initListe();

    int ES1[]  = {2};
    int CPU1[] = {3, 2};
    int ES2[]  = {3};
    int CPU2[] = {1, 3};
    int ES3[]  = {1};
    int CPU3[] = {2, 3};

    liste = inserProcListe(liste, initProc(1, 0, ES1, 1, CPU1, 2));
    liste = inserProcListe(liste, initProc(2, 1, ES2, 1, CPU2, 2));
    liste = inserProcListe(liste, initProc(3, 2, ES3, 1, CPU3, 2));

    return liste;
}

int main(int argc, char *argv[]){

    int   algo        = -1;
    int   quantum     = 2;
    int   export_csv  = 0;
    char *fichier     = NULL;

    /* Parsing des arguments */
    for (int i = 1; i < argc; i++){

        if (strcmp(argv[i], "-h") == 0){
            afficher_aide();
            return 0;
        }
        else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc){
            i++;
            if      (strcmp(argv[i], "FIFO") == 0) algo = ALGO_FIFO;
            else if (strcmp(argv[i], "SJF")  == 0) algo = ALGO_SJF;
            else if (strcmp(argv[i], "SJRF") == 0) algo = ALGO_SJRF;
            else if (strcmp(argv[i], "RR")   == 0) algo = ALGO_RR;
            else {
                printf("Erreur : algorithme '%s' inconnu\n", argv[i]);
                printf("Algorithmes disponibles : FIFO, SJF, SJRF, RR\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc){
            i++;
            quantum = atoi(argv[i]);
            if (quantum <= 0){
                printf("Erreur : le quantum doit être > 0\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc){
            i++;
            fichier = argv[i];
        }
        else if (strcmp(argv[i], "-csv") == 0){
            export_csv = 1;
        }
        else {
            printf("Erreur : argument '%s' inconnu\n", argv[i]);
            printf("Utilisez -h pour l'aide\n");
            return 1;
        }
    }

    /* Vérification algo */
    if (algo == -1){
        printf("Erreur : algorithme non spécifié\n");
        printf("Utilisez -h pour l'aide\n");
        return 1;
    }

    /* Vérification quantum pour RR */
    if (algo == ALGO_RR && quantum == 2){
        printf("Info : quantum par défaut = 2\n\n");
    }

    /* Chargement des processus */
    Liste liste;
    if (fichier != NULL){
        printf("Chargement des processus depuis '%s'\n\n", fichier);
        liste = lire_processus(fichier);
    }
    else{
        printf("Utilisation des processus par défaut\n\n");
        liste = creer_liste_defaut();
    }

    /* Lancement de l'ordonnancement */
    ordonnancer(&liste, algo, quantum, export_csv);

    return 0;
}