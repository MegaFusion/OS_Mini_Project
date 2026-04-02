#ifndef CALCUL_TEMPS_H
#define CALCUL_TEMPS_H

#include "ProcStruct.h"
#include "Liste.h"

int Proc_temps_restitution(Processus p);
int Proc_temps_attente(Processus p);
int Proc_temps_reponse(Processus p);
int restitution_moyen(Liste l);
int attente_moyen(Liste liste);
int reponse_moyen(Liste liste);
float Taux_occupation(int temps_CPU_actif, int temps_total);

#endif