#ifndef ORDONNANCEMENT_H
#define ORDONNANCEMENT_H
 
#include <unistd.h>
#include "Liste.h"
#include "FIFO.h"
#include "SJF.h"
#include "SJRF.h"

#define ALGO_FIFO 0
#define ALGO_SJF  1
#define ALGO_SJRF 2
#define ALGO_RR   3
 
void afficher_entete(Liste liste);
void afficher_ligne(Liste liste, int temps);
Liste trier_par_arrivee(Liste l);
Liste chercher_premier_pret(Liste l);
int tous_termines_file(Liste l);
int ordonnancer(Liste *liste, int algo, int quantum);

#endif