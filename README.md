# Simulateur d'Ordonnancement de Processus

Simulateur pédagogique d'algorithmes d'ordonnancement de processus écrit en C. Il supporte quatre algorithmes classiques et affiche un diagramme de Gantt coloré dans le terminal, avec export CSV optionnel.

## Auteurs
- ZURKHANG Tenzin
- YANG Kawei
- RAZOLIFERASON Tsanta

## Fonctionnalités

- **4 algorithmes** : FIFO, SJF, SJRF (préemptif), Round Robin
- **Cycles CPU et E/S** : chaque processus alterne des rafales CPU et des opérations d'entrée/sortie
- **Affichage coloré** dans le terminal (codes ANSI)
- **Indicateurs de performance** : temps de restitution, d'attente, de réponse (par processus et en moyenne), taux d'occupation CPU
- **Export CSV** des résultats
- **Chargement depuis fichier** ou processus de test par défaut


## Compilation

```bash
make        # compiler
make clean  # supprimer l'exécutable
make re     # recompiler from scratch
```

Requiert `gcc` avec les flags `-Wall -Wextra`.


## Utilisation

```bash
./main -a <ALGO> [options]
```

### Algorithmes

| Option | Description |
|
| `-a FIFO` | First In First Out (non préemptif) |
| `-a SJF` | Shortest Job First (non préemptif) |
| `-a SJRF` | Shortest Job Remaining First (préemptif) |
| `-a RR` | Round Robin |

### Options

| Option | Description |
|
| `-q <n>` | Quantum pour RR (défaut : 2) |
| `-f <fichier>` | Charger les processus depuis un fichier |
| `-csv` | Sauvegarder les résultats dans un fichier CSV |
| `-h` | Afficher l'aide |

### Exemples

```bash
# FIFO avec les processus par défaut
./main -a FIFO

# Round Robin avec quantum 3 et export CSV
./main -a RR -q 3 -csv

# SJF depuis un fichier
./main -a SJF -f processus.txt

# SJRF depuis un fichier avec export CSV
./main -a SJRF -f processus.txt -csv
```


## Format du fichier de processus

Chaque ligne décrit un processus :

```
<pid> <arrivée> <CPU1> <CPU2> ... / <ES1> <ES2> ...
```

- Les valeurs CPU et E/S sont des durées en ticks
- Le `/` sépare les cycles CPU des cycles E/S
- Les lignes commençant par `#` sont des commentaires

### Exemple

```
# pid  arrivée  CPU...  /  ES...
1      0        3 2     /  2
2      1        1 3     /  3
3      2        2 3     /  1
```

Ici, le processus 1 arrive à t=0, effectue 3 ticks CPU, puis 2 ticks d'E/S, puis 2 ticks CPU.


## Affichage

```
=== Ordonnancement FIFO ===

Legende : CPU = en_cours  PRT = pret  BLQ = bloque  FIN = termine  = non arrive

Temps          P1     P2     P3
0              CPU      
1              CPU    PRT   
2              CPU    PRT    PRT
3              BLQ    CPU    PRT
...
Restitution    9      11     10
Reponse        0      2      4
Attente        2      5      3
Restitution_M  10.00
Reponse_M      2.00
Attente_M      3.33
T_occupation   85.00 %
```


## Structure du projet

```
.
├── main.c               # Point d'entrée, parsing des arguments
├── ordonnancement.c/h   # Boucle principale, affichage, export CSV
├── SJF.c/h              # Sélection du processus le plus court (non préemptif)
├── SJRF.c/h             # Sélection du processus le plus court (préemptif)
├── Liste.c/h            # Liste chaînée de processus
├── ProcStruct.c/h       # Structure Processus et Cycle (CPU/E/S)
├── Calcul_temps.c/h     # Calcul des indicateurs de performance
├── lecture_fichier.c/h  # Parsing du fichier de processus
└── Makefile
```


## États d'un processus

| Code | État | Couleur |
|
| ` | Non arrivé | Blanc |
| `PRT` | Prêt | Jaune |
| `CPU` | En cours d'exécution | Bleu |
| `BLQ` | Bloqué (E/S) | Violet |
| `FIN` | Terminé | Vert |
