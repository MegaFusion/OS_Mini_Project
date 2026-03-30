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
