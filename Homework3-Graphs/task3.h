#ifndef __TASK3_H
#define __TASK3_H

#include "task2.h"
#include "task1.h"

//creez o noua structura pentru a retine tipul fermei unui nod
//si campul vazut ce ma ajuta la generarea tuturor permutarilor
typedef struct Vector{
				int node;
				int ferma;
				int vazut;
}Vector;

void task3(FILE *f,FILE **g,int);
int is_that_ferma(char *,int ,int );
void task3_Floyd(graf *,int **,char *,Vector []
		,int,Vector *,int,int *,char **,char **);
void print_Floyd(int **, int , int );
int is_ready(Vector [],int );
int Floyd_Warshall_task3(graf *,char *,int ,Vector );
int **Simple_Floyd(graf *g);

#endif
