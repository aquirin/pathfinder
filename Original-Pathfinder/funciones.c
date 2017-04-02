#include "extern.h"

#define ENTRADA_1 "*vertices %i"
#define ENTRADA_2 "*Vertices %i"



/**********************************************************************************/
/***************** LEE LOS VALORES DESDE EL FICHERO DE ENTRADA ********************/
/**********************************************************************************/

void Input(int argc, char *argv[]){

  FILE *fich;
  int i, j;
  char cadena[1025];
  float aux;
  //float minimo = -1.0;

  if(argc!=2 && argc!=3){
	printf("\n[ERROR] - Formato de entrada incorrecto.\n\t\t ./pfnet fichero_de_entrada [q]\n");
	exit(1);
	}

  fich = fopen(argv[1],"r");
  if(fich==NULL){
  	printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
	exit(2);
  	}

  int ret = fscanf(fich, ENTRADA_1, &num_nodos);	//lee el numero de nodos
  if(ret==0) {
	printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
	exit(2);
  }
  printf(ENTRADA_1, num_nodos);		    //Mostramos por pantalla el numero de nodos
  
  if(argc == 3)
  	q = atoi(argv[2]);
  else
  	q = num_nodos -1;

  for (i=0; i<num_nodos+1;i++){		//obviamos la informacion de cada nodo
  	char* ret = fgets(cadena, 1024, fich);
	  if(ret==NULL) {
		printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
		exit(2);
	  }
	printf("%s", cadena);
	}

  char* ret2 = fgets(cadena, 1024, fich);
  if(ret2==NULL) {
	printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
	exit(2);
  }
  printf("*edges\n");
  
  pesos = (float ***) malloc(sizeof(float**)*q);
  if(pesos ==NULL){
  	printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
	exit(3);
  	}

  //reserva de memoria para las matrices de pesos, distancias y pfnet
  for(i=0;i<q;i++)
  	pesos[i] = Matriz_dinamica_float(num_nodos, num_nodos);

  pfnet = Matriz_dinamica_float(num_nodos, num_nodos);
  distancias = Matriz_dinamica_float(num_nodos, num_nodos);
  pesos_originales = Matriz_dinamica_float(num_nodos, num_nodos);

  for(i=0; i<num_nodos; i++)		//Inicializamos la matriz de pesos
  	for(j=0; j<num_nodos; j++){
		pesos[0][i][j] = 0.0;
		pesos_originales[i][j] = 0.0;
		}

  //Dependiendo del formato de entrada leemos de una forma u otra.
  if(strncmp(cadena,"*matrix",7)==0 || strncmp(cadena,"*Matrix",7)==0 || strncmp(cadena,"*MATRIX",7)==0)
	{
	for(i=0; i<num_nodos; i++)
		for(j=0; j<num_nodos; j++)
			{
			int ret = fscanf(fich,"%f",&aux);
			  if(ret==0) {
				printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
				exit(2);
			  }
			pesos_originales[i][j] = aux;
            		pesos[0][i][j] = aux;
			}

  	}
  else
	{
	if(strncmp(cadena,"*edges",6)==0 || strncmp(cadena,"*Edges",6)==0 || strncmp(cadena,"*EDGES",6)==0)
		{
  		while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF )
			{
			pesos_originales[i-1][j-1] = aux;
	        	pesos[0][i-1][j-1] = aux;
			/* si el fichero de entrada solo tiene los ejes de la media matriz diagonal */
			pesos_originales[j-1][i-1] = aux;
			pesos[0][j-1][i-1] = aux;
  			}
		}
  	else
		{
		printf("[ERROR] - Formato del fichero de entrada no valido.\n");
		exit(3);
  		}
	}

}


/***************************************************************************************/
/***************************** PROGRAMACIÓN DINAMICA ***********************************/
/***************************************************************************************/
void Programacion_Dinamica (){
  int i,j,k,z;
  float maximo, minimo_local;

  //Calculo de las q matrices de pesos
  for(i=1;i<q;i++){
  	for(j=0;j<num_nodos;j++){
		for(k=0;k<num_nodos;k++){
			maximo = 0.0;
			for(z=0;z<num_nodos && j!=k ;z++){
				if( pesos[0][j][z]!=0.0 && pesos[i-1][z][k]!=0.0 && z!=k && z!=j){
					minimo_local = 0.0;
					if(pesos[0][j][z] < pesos[i-1][z][k])
						minimo_local = pesos[0][j][z];
					else
						minimo_local = pesos[i-1][z][k];

					if(minimo_local > maximo && minimo_local!=0)
						maximo = minimo_local;
					}
				}
			if(maximo!=0.0)
				pesos[i][j][k] = maximo;
			else
				pesos[i][j][k] = 0.0;
		}  //for k
	}  //for j
  } //for i



  //Calculo de la matriz Dq de distancias
  for(i=0;i<num_nodos;i++){
  	for(j=0;j<num_nodos;j++){
		maximo = 0.0;
		for(k=0;k<q;k++)
			if(pesos[k][i][j] > maximo && pesos[k][i][j]!=0.0)
				maximo = pesos[k][i][j];
		if(maximo!=0.0)
			distancias[i][j] = maximo;
   		else
          		distancias[i][j] = 0.0;

	}  //for j
  }  //for i


  //Calculo de la PFNET a partir de las matrices de pesos y distancias
  for(i=0;i<num_nodos;i++)
  	for(j=0;j<num_nodos;j++){
		if(distancias[i][j]==pesos[0][i][j] && pesos[0][i][j]!=0.0)
			pfnet[i][j]=pesos_originales[i][j];
		else if (pesos[0][i][j]!=0.0)
			pfnet[i][j] = FALSE;
	}
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
