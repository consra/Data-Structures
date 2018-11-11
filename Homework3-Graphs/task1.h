#ifndef __TASK1_H__
#define __TASK1_H__

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

//structura pentru retinerea unui graf
typedef struct graf {
	int dim;
	int **mat;
} graf;

//structura pentru stiva
//ma ajuta la afisarea drumului
typedef struct Queue{
        int data;
        struct Queue *next;
}Queue;

void print_path2(int *parent, int v,const int end,FILE **g);
Queue *push (Queue *top, int data);
void task1(FILE *,FILE **);
int Dijkstra (graf *graph, int src,int end,FILE **g);
void add_con (graf *, int , int , int );
Queue *pop(Queue *q);
graf *init (int );
int find_ferma(char *v);
int min_dist  (int *dist, int *visited, int n);

#endif
