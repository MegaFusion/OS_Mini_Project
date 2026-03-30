#ifndef CALCUL_TEMPS_H
#define CALCUL_TEMPS_H

#include "ProcStruct.h"

int Proc_temps_restitution(Processus p);
int Proc_temps_attente(Processus p);
int Proc_temps_reponse(Processus p);
float Taux_occupation(int temps_CPU_actif, int temps_total);

#endif