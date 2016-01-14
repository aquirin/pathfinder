#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define TRUE	1.0
#define FALSE	0.0

/* VARIABLES */
extern int q;		//parametro q para indicar el número de arcos max que puede tener un camino
extern float **pfnet;	//si pfnet[i][j]=TRUE hay un enlace entre los nodos i y j
extern int num_nodos;	//numero de nodos de la matriz
extern float ***pesos;		//matriz de pesos asociados a los arcos
extern float **distancias;	//matriz de distancias
extern float **pesos_originales;

/* FUNCIONES Y PROCEDIMIENTOS*/
extern float ** Matriz_dinamica_float(int nfil, int ncol);	//reserva memoria para una matriz de flotantes
extern void Libera_matriz_dinamica_float (float **matriz, int nfils);	//libera memoria de una matriz de flotantes
extern void Input(int argc, char *argv[]);	//coge los datos de entrada y los guarda en las respectivas variables
extern void Programacion_Dinamica ();	//Función que resuelve el problema mediante programación dinamica

