#include "global.h"
#include "extern.h"

#define ENTRADA_1 "*vertices %i"
#define ENTRADA_2 "*Vertices %i"

/*
Note: it is possible that a bug avoid the use of the q parameter
(if q is different to n-1, the results are not the one expected)
*/


/**********************************************************************************/
/***************** LEE LOS VALORES DESDE EL FICHERO DE ENTRADA ********************/
/**********************************************************************************/

void Input(int argc, char *argv[]){

  FILE *fich;
  int i, j, k;
  char cadena[1025];
  float aux;
  float minimo = -1.0;

  if(argc>2){
	printf("\n[ERROR] - Formato de entrada incorrecto.\n\t\t ./pfnet [-|fichero_de_entrada]\n");
	exit(1);
	}

  if (argc==1 || !strcmp(argv[1],"-"))
      fich=stdin;
  else
      {
	fich = fopen(argv[1],"r");
	if(fich==NULL){
	printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
	exit(2);
	}
      }
      

  int ret = fscanf(fich, ENTRADA_1, &num_nodos);	//lee el numero de nodos
  printf(ENTRADA_1, num_nodos);		    //Mostramos por pantalla el numero de nodos

  for (i=0; i<num_nodos+1;i++){		//obviamos la informacion de cada nodo
  	char* ret = fgets(cadena, 1024, fich);
	printf("%s", cadena);
	}

#if ORIGINAL == 2
  /*// Allocation of predecessors[][]
  //reserva de memoria para las matrices de predecessors
  predecessors = Matriz_dinamica_char(num_nodos, num_nodos);*/
#endif

  // Allocation of pesos[][]
  //reserva de memoria para las matrices de pesos, distancias y pfnet
  pesos = Matriz_dinamica_float(num_nodos, num_nodos);

	
  // Allocation of pfnet[][] and pesos_originales[][]
  pfnet = Matriz_dinamica_float(num_nodos, num_nodos);
  pesos_originales = Matriz_dinamica_float(num_nodos, num_nodos);

  for(i=0; i<num_nodos; i++)		//Inicializamos la matriz de pesos
  	for(j=0; j<num_nodos; j++){
		pesos_originales[i][j] = 0.0;
		}

  // Open the .NET file
  open_net(fich);
	
  for(i=0;i<num_nodos;i++)		//Inicializamos la matriz pfnet[][]
  	for(j=0;j<num_nodos;j++){
		pfnet[i][j] = pesos_originales[i][j];
	}
	
	
	
  // Initialisation of the symmetry indicator
  is_sym = is_symmetrical();
  
  fclose(fich);
}


/***************************************************************************************/
/***************************** PROGRAMACIÓN DINAMICA ***********************************/
/***************************************************************************************/

/***

Algorithm Floyd-Warshall

Modified to take account of the maximal path value corresponding to the
minimum edge value

Useful variables:
num_nodos - number of nodes
pfnet[][] - used to return the final matrix
pesos_originales[][] - original weight matrix (eq. to W(1) or D(1))
pesos[][] - weight matrix W

***/
void Programacion_Dinamica (){
  int i,j,k,z;
  int beg;
  float maximo, minimo_local, minimo;


  // Initialisation of pesos[][]
  for(i=0; i<num_nodos; i++)		//Inicializamos la matriz de pesos
  	for(j=0; j<num_nodos; j++){
		pesos[i][j] = pesos_originales[i][j];
		}
		
#if ORIGINAL == 2
  // Initialisation of predecessors
  /*for(i=0; i<num_nodos; i++)		//Inicializamos la matriz de predecessors
  	for(j=0; j<num_nodos; j++){
		if(i==j || pesos[i][j]==INFINITY2)
			predecessors[i][j] = 0;
		else
			predecessors[i][j] = 1;
	}*/
#endif

  // Compute the weight matrix for each iteration
  // The result is stored in pesos[][]
  // Memory is saved by "forgetting" the weight matrix for iteration 1...(k-1) when we
  // are at the iteation k ( matrix (k) is needed for the computation of matrix (k) )
  
  for(k=0;k<num_nodos;k++)
  {
    for(i=0;i<num_nodos;i++)
    {
      // Start at (i+1) if the matrix is symmetrical, or 0 if not.
#if TRIANGULAR_OPTIMISATION == 1
      beg = (is_sym==1)?i+1:0;
#else
      beg=0;
#endif
      for(j=beg;j<num_nodos;j++)
      {
#if ORIGINAL == 0
	// Original Floyd-Warshall
	// Based on : pesos[k+1][i][j] = MIN( pesos[k][i][j] , (pesos[k][i][k]+pesos[k][k][j]) );
	if(pesos[i][j] > (pesos[i][k] + pesos[k][j]))
	{
		pesos[i][j] = (pesos[i][k] + pesos[k][j]);
		//predecessors[i][j] = predecessors[k][j];
	}
#else
	// Modified Floyd-Warshall
	// Based on : pesos[k+1][i][j] = MAX( pesos[k][i][j] , MIN(pesos[k][i][k],pesos[k][k][j]) );
#if TRIANGULAR_OPTIMISATION == 1
	minimo = MIN(pesos[MIN(i,k)][MAX(i,k)],pesos[MIN(k,j)][MAX(k,j)]);
#else
	minimo = MIN(pesos[i][k],pesos[k][j]);
#endif
	if(pesos[i][j] < minimo)
	{
		pesos[i][j] = minimo;
#if ORIGINAL == 2
		//predecessors[i][j] = 0;
		pfnet[i][j] = FALSE;
#endif
	} // if(pesos[i][j] < minimo) 
#endif
	
      }  // for j
      
      pesos[i][i] = 0;
#if ORIGINAL == 2
      //predecessors[i][i] = 1;
#endif

    } // for i

  //int i2, j2;
  /*printf("***** Matrix predecessors[][] :\n");
  for(i2=0;i2<num_nodos;i2++)
  {
    for(j2=0;j2<num_nodos;j2++)
    {
      printf(" %d", (int)predecessors[i2][j2]);
    }
    printf("\n");
  }*/
  /*printf("***** Matrix pesos[][] :\n");
  for(i2=0;i2<num_nodos;i2++)
  {
    for(j2=0;j2<num_nodos;j2++)
    {
      printf(" %d", (int)pesos[i2][j2]);
    }
    printf("\n");
  }
  printf("*****\n");*/


   
  }  // for k

  /*// Print pesos[][]
  for(k=0;k<=q+1;k++)
  {
  //k = q-1;
  printf("***** Matrix pesos[][] :\n");
  for(i=0;i<num_nodos;i++)
  {
    for(j=0;j<num_nodos;j++)
    {
      printf(" %f", pesos[i][j]);
    }
    printf("\n");
  }
  printf("*****\n");
  }*/
  
  /*// Print predecessors[][]
  for(k=0;k<=q+1;k++)
  {
  //k = q-1;
  printf("***** Matrix predecessors[][] :\n");
  for(i=0;i<num_nodos;i++)
  {
    for(j=0;j<num_nodos;j++)
    {
      printf(" %d", (int)predecessors[i][j]);
    }
    printf("\n");
  }
  printf("*****\n");
  }*/

#if ORIGINAL < 2
  //Calculo de la PFNET a partir de las matrices de pesos y distancias
  // The "distancias[][]" matrix is replaced by pesos[][]
  // THIS PART IS FASTER THAN THE SECOND PART
  for(i=0;i<num_nodos;i++)
  {
      // Start at (i+1) if the matrix is symmetrical, or 0 if not.
      beg = (is_sym==1)?i+1:0;
      for(j=beg;j<num_nodos;j++){
		if(pesos[i][j]==pesos_originales[i][j] && pesos_originales[i][j]!=0.0)
			pfnet[i][j]=pesos_originales[i][j];
		else if (pesos_originales[i][j]!=0.0)
			pfnet[i][j] = FALSE;
	}
  }
#else
  /*// Substitution of the PFNET selection by the fusion of the
  // predecessors[][] matrix for every values of k
  // SECOND PART
  for(i=0;i<num_nodos;i++)
    for(j=0;j<num_nodos;j++){
      if(predecessors[i][j]==1)	// if true, it is a component of the shortest path
      {	
	pfnet[i][j]=pesos_originales[i][j];
      }		
    }*/
#endif
    
	
  // The predecessors[][] matrix is not printed in the output
  // predecessors[i][j] indicates the number of the node to go from the node i to the node j
  // So, the complete path from a node N1 to a node N2, is:
  // the last edge of this path is the edge  s1=predecessors[N1][N2]  =>  N2
  // the previous edge is    s2=predecessors[N1][s1]  s1
  // the previous edge is    s3=predecessors[N1][s2]  s2
  // ... until s(n) is N1
}



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


char * * Matriz_dinamica_char(int nfils, int ncols)
{

   char **matriz;
   int i,j,error=0;
   /*reservo memoria para un vector de nfil punteros a enteros largos*/
   if((matriz=(char **)calloc(nfils,sizeof(char *)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}

   else
   /*para cada componente del vector (puntero a entero largo) reservo memoria para
   un vector de ncols de enteros largos*/
       for(i=0;(i<nfils) && !error;i++)
          if((matriz[i]=(char*)calloc(ncols,sizeof(char)))==NULL)
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


void Libera_matriz_dinamica_char (char **matriz, int nfils)
{
   int i;

   for(i=0;i<nfils;i++)
	free (matriz[i]);
   free (matriz);
}
