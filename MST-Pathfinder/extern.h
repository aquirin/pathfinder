#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "routines.h"

#define TRUE	1.0
#define FALSE	0.0
#define INFINITY	1000		// Value used to code a non-link between 2 nodes


/* STRUCTURES */
typedef struct _SEDGE
{
  int i, j;
  float v;
} SEDGE;


/* VARIABLES */
extern int q;		//parametro q para indicar el número de arcos max que puede tener un camino
extern float **pfnet;	//si pfnet[i][j]=TRUE hay un enlace entre los nodos i y j
extern int num_nodos;	//numero de nodos de la matriz
extern int num_edges;	// number of edges in the matrix
extern float **pesos_originales;
extern SEDGE **vedges;
extern int *compConnexe;
extern int *compConnexeRank;
extern int *compConnexeBuffer;
extern int *sameValuesNodes;


/* FUNCIONES Y PROCEDIMIENTOS*/
extern float ** Matriz_dinamica_float(int nfil, int ncol);	//reserva memoria para una matriz de flotantes
extern void Libera_matriz_dinamica_float (float **matriz, int nfils);	//libera memoria de una matriz de flotantes
extern void Input(int argc, char *argv[]);	//coge los datos de entrada y los guarda en las respectivas variables
extern void Programacion_Dinamica ();	//Función que resuelve el problema mediante programación dinamica
static int compedges_min(const void *_e1, const void *_e2);
static int compedges_max(const void *_e1, const void *_e2);
extern inline void ds_alloc(int n);
extern inline void ds_init(int n);
extern inline int ds_find(int x);
extern inline void ds_join(int n, int x, int y);
extern inline int ds_same(int x, int y);
extern char *strreplace(char *src, char *replace, char *to);
extern char *strreplace_all(char *src, char *replace, char *to);
