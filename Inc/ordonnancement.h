#ifndef ORDONNANCEMENT_H
#define ORDONNANCEMENT_H
 
#include <unistd.h>
#include "Liste.h"
#include "SJF.h"
#include "SJRF.h"
#include "Calcul_temps.h"

#define ALGO_FIFO 0
#define ALGO_SJF  1
#define ALGO_SJRF 2
#define ALGO_RR   3
 
void afficher_entete(Liste liste);
void afficher_ligne(Liste liste, int temps);
Liste trier_par_arrivee(Liste l);
Liste chercher_premier_pret(Liste l);
int tous_termines_file(Liste l);
void afficher_restitution(Liste l);
void afficher_attente(Liste l);
void afficher_reponse(Liste l);
void afficher_restitution_moyen(Liste liste);
void afficher_attente_moyen(Liste liste);
void afficher_reponse_moyen(Liste liste);
void afficher_occupation(int temps_CPU_actif, int temps_total, Liste liste);
int ordonnancer(Liste *liste, int algo, int quantum, int csv);

#endif