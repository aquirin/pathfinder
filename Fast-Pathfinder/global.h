#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define TRUE	1.0
#define FALSE	0.0
#define OUTPUT_MATRIX   1
#define OUTPUT_EDGES    2
#define OUTPUT_ARCS     3


// Type of algorithm / edge selection
// ORIGINAL=0: original floyd-warshall algorithm
// ORIGINAL=1: modified PFNet + edge selection using the same way as original PFNet (fastest) [way1]
// ORIGINAL=2: modified PFNet + edge selection using the predecessor matrix [way2]
#define ORIGINAL 1

// Time the computation time
// 0: no chronometer
// N: N iterations of the computation + print of the results
#define CHRONOMETER 0
#define CHRONOMETER_ALL 0

// Enable the triangular optimisation
// Instead of computing all the cells of the distance matrix, compute only the cells in the upper triangle
// In this case, it is needed to sort the index of cells, i.e. to use M[min(a,b)][max(a,b)] instead of
// M[a][b]
#define TRIANGULAR_OPTIMISATION 1

// Number of digits used for the values of the links
// Examples: ".8", ".10", "" for the maximal available precision
#define NDIGITS ""


/* VARIABLES */
float **pfnet;	//si pfnet[i][j]=TRUE hay un enlace entre los nodos i y j
int num_nodos;	//numero de nodos de la matriz
float **pesos;		//matriz de pesos asociados a los arcos
float **pesos_originales;
char **predecessors;		// Matrix of the predecessor nodes (values are 1-based)
int is_sym;		// 1 if the original matrix is symmetrical


/* FUNCIONES Y PROCEDIMIENTOS*/
float ** Matriz_dinamica_float(int nfil, int ncol);	//reserva memoria para una matriz de flotantes
void Libera_matriz_dinamica_float (float **matriz, int nfils);	//libera memoria de una matriz de flotantes
char ** Matriz_dinamica_char(int nfil, int ncol);	//reserva memoria para una matriz de char
void Libera_matriz_dinamica_char (char **matriz, int nfils);	//libera memoria de una matriz de char
void Input(int argc, char *argv[]);	//coge los datos de entrada y los guarda en las respectivas variables
void Programacion_Dinamica ();	//Función que resuelve el problema mediante programación dinamica
void open_net(FILE* fich);
int is_symmetrical(void);
void save_net(float** pfnet, FILE* fich, int type);


