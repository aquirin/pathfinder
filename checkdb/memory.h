#ifndef __MEMORY_H__
#define __MEMORY_H__

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include "global.h"

// FUNCTIONS
float * * Matriz_dinamica_float(int nfils, int ncols);
void Libera_matriz_dinamica_float (float **matriz, int nfils);
int * * Matriz_dinamica_int(int nfils, int ncols);
void Libera_matriz_dinamica_int (int **matriz, int nfils);
char* Vector_char(int nfils);
void Libera_vector_char (char *matriz);

#endif // __MEMORY_H__
