#include "Calcul_temps.h"

int Proc_temps_restitution(Processus p){
    /**
     * Calcul du temps de restitution d'un processus
     * @param p : processus
     * @return le temps total passé par le processus dans le système
     */
    return p.temps_fin - p.arrive;
}

int Proc_temps_attente(Processus p){
    /**
     * Calcul du temps d'attente d'un processus
     * @param p : processus
     * @return le temps total passé par le processus dans le système en état prêt
     */
    return Proc_temps_restitution(p) - p.temps_CPU - p.temps_ES;
}

int Proc_temps_reponse(Processus p){
    /**
     * Calcul du temps de réponse d'un processus
     * @param p : processus
     * @return le temps d'attente du processus depuis son arrivée dans le système jusqu'au premier passage à l'état "en_cours" 
     */
    return p.temps_premier_CPU - p.arrive;
}

float Taux_occupation(int temps_CPU_actif, int temps_total){
    /**
     * Calcul du taux d'occupation du CPU par les processus
     * @param temps_CPU_actif : temps où le CPU est resté occupé
     * @param temps_total : temps total de l'ordonnancement
     * @return taux d'occupation du CPU par rapport au temps total d'ordonnancement en pourcentage
     */
    return ((float)temps_CPU_actif / temps_total)*100;
}

float restitution_moyen(Liste liste){
    /**
     * Calcul du temps de restitution moyen de l'ordonnancement
     * @param liste : la liste des processus
     * @return le temps de restitution moyen
     */
    int restitution_total = 0;
    int nbr_Proc = 0;
    Liste l = liste;
    while (!estVideListe(l)){
        restitution_total += Proc_temps_restitution(l->proc);
        l = l->suivant;
        nbr_Proc ++;
    }
    if (nbr_Proc){
        return (float)restitution_total / nbr_Proc;
    }
    return 0;
}
float reponse_moyen(Liste liste){
    /**
     * Calcul du temps de reponse moyen de l'ordonnancement
     * @param liste : la liste des processus
     * @return le temps de reponse moyen
     */
    int reponse_total = 0;
    int nbr_Proc = 0;
    Liste l = liste;
    while (!estVideListe(l)){
        reponse_total += Proc_temps_reponse(l->proc);
        l = l->suivant;
        nbr_Proc ++;
    }
    if (nbr_Proc){
        return (float)reponse_total / nbr_Proc;
    }
    return 0;

}
float attente_moyen(Liste liste){
    /**
     * Calcul du temps de attente moyen de l'ordonnancement
     * @param liste : la liste des processus
     * @return le temps de attente moyen
     */
    int attente_total = 0;
    int nbr_Proc = 0;
    Liste l = liste;
    while (!estVideListe(l)){
        attente_total += Proc_temps_attente(l->proc);
        l = l->suivant;
        nbr_Proc ++;
    }
    if (nbr_Proc){
        return (float)attente_total / nbr_Proc;
    }
    return 0;
}
