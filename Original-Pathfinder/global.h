#define TRUE	1.0
#define FALSE	0.0

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "routines.h"

// Time the computation time
// 0: no chronometer
// N: N iterations of the computation + print of the results
#define CHRONOMETER 0		// 151s in average
#define CHRONOMETER_ALL 0		// 151s in average

// Number of digits used for the values of the links
// Examples: ".8", ".10", "" for the maximal available precision
#define NDIGITS ""


/* VARIABLES */
int q;		//parametro q para indicar el número de arcos max que puede tener un camino
float **pfnet;	//si pfnet[i][j]=TRUE hay un enlace entre los nodos i y j
int num_nodos;	//numero de nodos de la matriz
float ***pesos;		//matriz de pesos asociados a los arcos
float **distancias;	//matriz de distancias
float **pesos_originales;

/* FUNCIONES Y PROCEDIMIENTOS*/
float ** Matriz_dinamica_float(int nfil, int ncol);	//reserva memoria para una matriz de flotantes
void Libera_matriz_dinamica_float (float **matriz, int nfils);	//libera memoria de una matriz de flotantes
void Input(int argc, char *argv[]);	//coge los datos de entrada y los guarda en las respectivas variables
void Programacion_Dinamica ();	//Función que resuelve el problema mediante programación dinamica


