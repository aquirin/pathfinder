/*
General operations about memory.

*/

#include "memory.h"


/*********************************************************************************/
/************************ RESERVA DINAMICA DE MATRICES REALES ********************/
/*********************************************************************************/


float * * Matriz_dinamica_float(int nfils, int ncols)
{

   float **matriz;
   int i,j,error=0;
   /*reservo memoria para un vector de nfil punteros a enteros largos*/
   if((matriz=(float **)calloc(nfils,sizeof(float *)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}

   else
   /*para cada componente del vector (puntero a entero largo) reservo memoria para
   un vector de ncols de enteros largos*/
       for(i=0;(i<nfils) && !error;i++)
          if((matriz[i]=(float*)calloc(ncols,sizeof(float)))==NULL)
          {
             printf("Error en la asignacion de memoria\n");
             error=1;
             for(j=i-1;j>=0;j--)   /* si no se puede reservar, libero la memoria que*/
                free(matriz[j]);   /* habia reservado previamente */
             free(matriz);
             matriz=NULL;          /* igualo el puntero que controla la matriz a NULL */
          }
    return(matriz);                /* devuelvo el puntero que controla la matriz*/
}


void Libera_matriz_dinamica_float (float **matriz, int nfils)
{
   int i;

   for(i=0;i<nfils;i++)
	free (matriz[i]);
   free (matriz);
}


int * * Matriz_dinamica_int(int nfils, int ncols)
{

   int **matriz;
   int i,j,error=0;
   /*reservo memoria para un vector de nfil punteros a enteros largos*/
   if((matriz=(int **)calloc(nfils,sizeof(int *)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}

   else
   /*para cada componente del vector (puntero a entero largo) reservo memoria para
   un vector de ncols de enteros largos*/
       for(i=0;(i<nfils) && !error;i++)
          if((matriz[i]=(int*)calloc(ncols,sizeof(int)))==NULL)
          {
             printf("Error en la asignacion de memoria\n");
             error=1;
             for(j=i-1;j>=0;j--)   /* si no se puede reservar, libero la memoria que*/
                free(matriz[j]);   /* habia reservado previamente */
             free(matriz);
             matriz=NULL;          /* igualo el puntero que controla la matriz a NULL */
          }
    return(matriz);                /* devuelvo el puntero que controla la matriz*/
}


void Libera_matriz_dinamica_int (int **matriz, int nfils)
{
   int i;

   for(i=0;i<nfils;i++)
	free (matriz[i]);
   free (matriz);
}



char* Vector_char(int nfils)
{

   char*matriz;
   int i,j,error=0;
   /*reservo memoria para un vector de nfil punteros a enteros largos*/
   if((matriz=(char*)calloc(nfils,sizeof(char)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}
    return(matriz);                /* devuelvo el puntero que controla la matriz*/
}


void Libera_vector_char (char *matriz)
{
   int i;

   free (matriz);
}
