#ifndef __TASK2_H
#define __TASK2_H

//am nevoie si de functiile de la taskul1 pentru initializarea grafului
#include "task1.h"

int is_ferma(char *,int);
int ModDijkstra (graf *,int *, int *,int,char *,FILE **g);
int min_dist_next_ferma(int *dist,int n,char *);
int Best_ferm(graf *,char *,int ,int );
void task2(FILE *f,FILE **g,int);

#endif
