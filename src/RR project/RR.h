// RR.h
#ifndef RR_H
#define RR_H

#include "processus.h"  // 包含你同学定义的Processus结构体

// 就绪队列节点
typedef struct node_ready {
    int processus_index;  // 进程在数组中的索引
    struct node_ready* suivant;
} *FileReady;

// 阻塞队列节点（用于I/O）
typedef struct node_blocked {
    int processus_index;
    int temps_restant_io;  // I/O剩余时间
    struct node_blocked* suivant;
} *FileBlocked;

// RR算法参数
typedef struct {
    int quantum;  // 时间片大小
} RR_Param;

// 函数声明
void RR(Processus processus[], int nb_processus, int quantum);
void afficher_statistiques_RR(Processus processus[], int nb_processus, 
                              int temps_total, int temps_cpu_total);

#endif