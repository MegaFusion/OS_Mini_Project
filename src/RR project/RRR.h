// RR.c
#include <stdio.h>
#include <stdlib.h>
#include "RR.h"

// 就绪队列操作
FileReady creer_file_ready() {
    return NULL;
}

int est_file_ready_vide(FileReady f) {
    return f == NULL;
}

void enfiler_ready(FileReady* f, int index) {
    FileReady nouveau = (FileReady)malloc(sizeof(struct node_ready));
    if (nouveau == NULL) {
        printf("Erreur d'allocation mémoire\n");
        exit(1);
    }
    nouveau->processus_index = index;
    nouveau->suivant = NULL;
    
    if (est_file_ready_vide(*f)) {
        *f = nouveau;
    } else {
        FileReady tmp = *f;
        while (tmp->suivant != NULL) {
            tmp = tmp->suivant;
        }
        tmp->suivant = nouveau;
    }
}

int defiler_ready(FileReady* f) {
    if (est_file_ready_vide(*f)) {
        printf("File ready vide\n");
        return -1;
    }
    FileReady tmp = *f;
    int index = tmp->processus_index;
    *f = tmp->suivant;
    free(tmp);
    return index;
}

// 阻塞队列操作
FileBlocked creer_file_blocked() {
    return NULL;
}

int est_file_blocked_vide(FileBlocked f) {
    return f == NULL;
}

void enfiler_blocked(FileBlocked* f, int index, int duree_io) {
    FileBlocked nouveau = (FileBlocked)malloc(sizeof(struct node_blocked));
    if (nouveau == NULL) {
        printf("Erreur d'allocation mémoire\n");
        exit(1);
    }
    nouveau->processus_index = index;
    nouveau->temps_restant_io = duree_io;
    nouveau->suivant = NULL;
    
    if (est_file_blocked_vide(*f)) {
        *f = nouveau;
    } else {
        FileBlocked tmp = *f;
        while (tmp->suivant != NULL) {
            tmp = tmp->suivant;
        }
        tmp->suivant = nouveau;
    }
}

void retirer_blocked(FileBlocked* f, int index) {
    if (est_file_blocked_vide(*f)) return;
    
    FileBlocked courant = *f;
    FileBlocked precedent = NULL;
    
    while (courant != NULL && courant->processus_index != index) {
        precedent = courant;
        courant = courant->suivant;
    }
    
    if (courant == NULL) return;
    
    if (precedent == NULL) {
        *f = courant->suivant;
    } else {
        precedent->suivant = courant->suivant;
    }
    free(courant);
}

// 检查新到达的进程
void verifier_nouvelles_arrivees(Processus processus[], int nb_processus, 
                                 int temps_actuel, FileReady* ready) {
    for (int i = 0; i < nb_processus; i++) {
        if (processus[i].arrive == temps_actuel && processus[i].etat == 0) {
            processus[i].etat = 1;  // prêt
            enfiler_ready(ready, i);
            printf("[t=%d] Processus %d arrive\n", temps_actuel, i);
        }
    }
}

// 处理I/O完成
void gerer_IO(Processus processus[], int nb_processus, 
              FileBlocked* blocked, FileReady* ready, int temps_actuel) {
    if (est_file_blocked_vide(*blocked)) return;
    
    FileBlocked courant = *blocked;
    FileBlocked precedent = NULL;
    
    while (courant != NULL) {
        courant->temps_restant_io--;
        
        if (courant->temps_restant_io <= 0) {
            // I/O完成
            int idx = courant->processus_index;
            processus[idx].etat = 1;  // prêt
            enfiler_ready(ready, idx);
            
            // 移动到下一个CPU突发
            if (!estVideCycle(processus[idx].CPU)) {
                processus[idx].CPU = suivantCycle(processus[idx].CPU);
            }
            
            printf("[t=%d] Processus %d I/O termine, retour pret\n", 
                   temps_actuel, idx);
            
            // 从阻塞队列移除
            if (precedent == NULL) {
                *blocked = courant->suivant;
                free(courant);
                courant = *blocked;
            } else {
                precedent->suivant = courant->suivant;
                free(courant);
                courant = precedent->suivant;
            }
        } else {
            precedent = courant;
            courant = courant->suivant;
        }
    }
}

// 主RR算法实现
void RR(Processus processus[], int nb_processus, int quantum) {
    // 初始化
    FileReady ready_queue = creer_file_ready();
    FileBlocked blocked_queue = creer_file_blocked();
    
    int temps_actuel = 0;
    int processus_termines = 0;
    int processus_courant = -1;
    int temps_execute_dans_quantum = 0;
    
    // 统计信息
    int temps_cpu_total = 0;
    float temps_attente_total = 0;
    float temps_restitution_total = 0;
    float temps_reponse_total = 0;
    
    // 为每个进程添加统计字段
    typedef struct {
        int temps_demarrage;      // 首次开始执行时间
        int temps_fin;            // 完成时间
        int temps_attente;        // 等待时间
        int temps_reponse;        // 响应时间
        int temps_execution;      // 总执行时间
        int temps_dernier_debut;   // 上次开始执行时间
    } StatsProcessus;
    
    StatsProcessus stats[nb_processus];
    for (int i = 0; i < nb_processus; i++) {
        stats[i].temps_demarrage = -1;
        stats[i].temps_fin = 0;
        stats[i].temps_attente = 0;
        stats[i].temps_reponse = 0;
        stats[i].temps_execution = 0;
        stats[i].temps_dernier_debut = -1;
    }
    
    printf("\n=== Début de la simulation RR (quantum=%d ms) ===\n\n", quantum);
    
    // 主循环
    while (processus_termines < nb_processus) {
        // 1. 检查新到达的进程
        verifier_nouvelles_arrivees(processus, nb_processus, temps_actuel, &ready_queue);
        
        // 2. 处理I/O完成
        gerer_IO(processus, nb_processus, &blocked_queue, &ready_queue, temps_actuel);
        
        // 3. 如果没有正在执行的进程，从就绪队列取一个
        if (processus_courant == -1 && !est_file_ready_vide(ready_queue)) {
            processus_courant = defiler_ready(&ready_queue);
            temps_execute_dans_quantum = 0;
            
            // 如果是首次执行，记录响应时间
            if (stats[processus_courant].temps_demarrage == -1) {
                stats[processus_courant].temps_demarrage = temps_actuel;
                stats[processus_courant].temps_reponse = 
                    temps_actuel - processus[processus_courant].arrive;
            }
            
            // 记录等待时间
            if (stats[processus_courant].temps_dernier_debut != -1) {
                stats[processus_courant].temps_attente += 
                    temps_actuel - stats[processus_courant].temps_dernier_debut;
            }
            stats[processus_courant].temps_dernier_debut = temps_actuel;
            
            processus[processus_courant].etat = 2;  // en_cours
            printf("[t=%d] Processus %d commence l'execution\n", 
                   temps_actuel, processus_courant);
        }
        
    // 4. 执行当前进程（如果有）
        if (processus_courant != -1) {
            // 获取当前进程CPU突发链表的头部节点指针
            Cycle current_node = processus[processus_courant].CPU;
            
            if (!estVideCycle(current_node)) {
                // 执行 1ms
                temps_actuel++;
                temps_cpu_total++;
                temps_execute_dans_quantum++;
                stats[processus_courant].temps_execution++;
                
                // 【核心修改】：直接递减链表节点中的剩余时间
                current_node->duree--; 
                
                // 检查当前这次 CPU 突发（Burst）是否完成
                if (current_node->duree <= 0) {
                    // CPU 突发完成，从链表中释放头节点
                    supTeteCycle(&processus[processus_courant].CPU);
                    
                    // 检查是否紧接着有 I/O 操作
                    if (!estVideCycle(processus[processus_courant].ES)) {
                        int duree_io = get_DureeCycle(processus[processus_courant].ES);
                        processus[processus_courant].etat = 3;  // 状态改为：bloqué
                        enfiler_blocked(&blocked_queue, processus_courant, duree_io);
                        
                        printf("[t=%d] Processus %d bloque pour I/O (%d ms)\n", 
                               temps_actuel, processus_courant, duree_io);
                        
                        stats[processus_courant].temps_dernier_debut = -1;
                        processus_courant = -1;
                    } 
                    // 检查进程是否所有的 CPU 突发都已完成（进程彻底结束）
                    else if (estVideCycle(processus[processus_courant].CPU)) {
                        processus[processus_courant].etat = 4;  // 状态改为：terminé
                        processus_termines++;
                        stats[processus_courant].temps_fin = temps_actuel;
                        
                        // 计算统计信息
                        int temps_restitution = temps_actuel - processus[processus_courant].arrive;
                        stats[processus_courant].temps_attente = temps_restitution - stats[processus_courant].temps_execution;
                        
                        temps_attente_total += stats[processus_courant].temps_attente;
                        temps_restitution_total += temps_restitution;
                        temps_reponse_total += stats[processus_courant].temps_reponse;
                        
                        printf("[t=%d] Processus %d termine\n", temps_actuel, processus_courant);
                        processus_courant = -1;
                    }
                }
            }
        }
        
        // 5. 检查时间片是否用完
        if (processus_courant != -1 && temps_execute_dans_quantum >= quantum) {
            // 时间片用完，当前进程放回就绪队列末尾
            printf("[t=%d] Quantum expire pour processus %d\n", 
                   temps_actuel, processus_courant);
            
            processus[processus_courant].etat = 1;  // prêt
            enfiler_ready(&ready_queue, processus_courant);
            
            // 记录等待时间
            stats[processus_courant].temps_dernier_debut = -1;
            
            processus_courant = -1;
        }
    }
    
    // 输出统计信息
    printf("\n=== Statistiques RR (quantum=%d ms) ===\n", quantum);
    printf("Temps d'attente moyen: %.2f ms\n", 
           temps_attente_total / nb_processus);
    printf("Temps de restitution moyen: %.2f ms\n", 
           temps_restitution_total / nb_processus);
    printf("Temps de reponse moyen: %.2f ms\n", 
           temps_reponse_total / nb_processus);
    printf("Taux d'occupation CPU: %.2f%%\n", 
           (float)temps_cpu_total / temps_actuel * 100);
    
    // 按进程输出详细信息
    printf("\n=== Details par processus ===\n");
    printf("Processus\tArrivee\tAttente\tReponse\tRestitution\n");
    for (int i = 0; i < nb_processus; i++) {
        printf("P%d\t\t%d\t%d\t%d\t%d\n", 
               i, 
               processus[i].arrive,
               stats[i].temps_attente,
               stats[i].temps_reponse,
               stats[i].temps_fin - processus[i].arrive);
    }
}
