#define TRUE	1.0
#define FALSE	0.0

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "routines.h"


// Type of algorithm
// 0: Original Kruskal algorithm
// 1: Modified Kruskal algorithm (faster, but not good in the general case)
// 2: K-Fast-PFNet (same results in the general case)
#define ORIGINAL 2

// Type of basic functions
// 0: Practical run time (disjoined sets based on index) [FASTEST]
// 1: Theoretical run time (disjoined sets based on forest) [SLOWEST]
#define BASIC_FUNCTIONS 0

// Time the computation time
// 0: no chronometer
// N: N iterations of the computation + print of the results
#define CHRONOMETER 0
#define CHRONOMETER_ALL 0

// Type of debug
// 0: normal behavior
// 1: print some information
// 2: print the input file in a file named KFDEBUG_DATE_TIME.net
//    usefull when dealing with some obfuscate standard inputs 
#define DEBUG 0

// Number of digits used for the values of the links
// Examples: ".8", ".10", ... Put "" for the maximal available precision
#define NDIGITS ""

/* STRUCTURES */
typedef struct _SEDGE
{
  int i, j;
  double v;
} SEDGE;

/* VARIABLES */
int q;		//parametro q para indicar el número de arcos max que puede tener un camino
double **pfnet;	//si pfnet[i][j]=TRUE hay un enlace entre los nodos i y j
int num_nodos;	//numero de nodos de la matriz
int num_edges;	// number of edges in the matrix
double **pesos_originales;
SEDGE **vedges;
int *compConnexe;
int *compConnexeRank;
int *compConnexeBuffer;
int *sameValuesNodes;   // Represent the index of the edges with the same values


/* FUNCIONES Y PROCEDIMIENTOS*/
double ** Matriz_dinamica_float(int nfil, int ncol);	//reserva memoria para una matriz de flotantes
void Libera_matriz_dinamica_float (double **matriz, int nfils);	//libera memoria de una matriz de flotantes
void Input(int argc, char *argv[]);	//coge los datos de entrada y los guarda en las respectivas variables
void Programacion_Dinamica ();	//Función que resuelve el problema mediante programación dinamica
void ds_alloc(int n);
void ds_init(int n);
int ds_find(int x);
void ds_join(int n, int x, int y);
int ds_same(int x, int y);
char *strreplace(char *src, char *replace, char *to);
char *strreplace_all(char *src, char *replace, char *to);
int get_num_nodes(FILE* fich);
