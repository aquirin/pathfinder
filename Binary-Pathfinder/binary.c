#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "routines.h"

// Time the computation time
// 0: no chronometer
// N: N iterations of the computation + print of the results
#define CHRONOMETER 18		// 8s in average
#define CHRONOMETER_ALL 0		// 8s in average

// Number of digits used for the values of the links
// Examples: ".8", ".10", ... Put "" for the maximal available precision
#define NDIGITS ""


#define ENTRADA_1 "*vertices %i"
#define ENTRADA_2 "*Vertices %i"
#define TRUE	1
#define FALSE	0

/* VARIABLES */


int q;		//parametro q para indicar el número de arcos max que puede tener un camino
float **pfnet;	//si pfnet[i][j]=TRUE hay un enlace entre los nodos i y j
int num_nodos;	//numero de nodos de la matriz
//unsigned int ***pesos;		//matriz de pesos asociados a los arcos
float **sim;	//matriz de distancias
float **simo;	//matriz de distancias
float **d1;	//matriz de distancias
float **d2;	//matriz de distancias
float **temp;	//matriz de distancias
float **simacum;	//matriz de distancias
float **pesos_originales;


void binary();




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


/*********************************************************************************/
/*********************** RESERVA DINAMICA DE MATRICES ENTERAS ********************/
/*********************************************************************************/


unsigned int * * Matriz_dinamica_entero(int nfils, int ncols)
{

   unsigned int **matriz;
   int i,j,error=0;
   /*reservo memoria para un vector de nfil punteros a enteros largos*/
   if((matriz=(unsigned int **)calloc(nfils,sizeof(unsigned int *)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}

   else
   /*para cada componente del vector (puntero a entero largo) reservo memoria para
   un vector de ncols de enteros largos*/
       for(i=0;(i<nfils) && !error;i++)
          if((matriz[i]=(unsigned int*)calloc(ncols,sizeof(unsigned int)))==NULL)
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

/**********************************************************************************/
/***************** LEE LOS VALORES DESDE EL FICHERO DE ENTRADA ********************/
/**********************************************************************************/

void Input(int argc, char *argv[]){

  FILE *fich;
  int i, j;
  char cadena[1024];
  float aux;
  //double factor;
  float minimo = -1;

  if(argc>3){
	printf("\n[ERROR] - Formato de entrada incorrecto.\n\t\t ./pfnet [-|fichero_de_entrada] [q]\n");
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
  	char* retc = fgets(cadena, 1024, fich);
	  if(retc==NULL) {
		printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
		exit(2);
	  }
	printf("%s", cadena);
	}

  char* retc = fgets(cadena, 1024, fich);
  if(retc==NULL) {
	printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
	exit(2);
  }
  printf("*edges\n");

  d1 = Matriz_dinamica_float(num_nodos, num_nodos);
  d2 = Matriz_dinamica_float(num_nodos, num_nodos);
  simo = Matriz_dinamica_float(num_nodos, num_nodos);
  simacum = Matriz_dinamica_float(num_nodos, num_nodos);

  pfnet = Matriz_dinamica_float(num_nodos, num_nodos);
  sim = d1;
  temp=d2;
  pesos_originales = Matriz_dinamica_float(num_nodos, num_nodos);

  for(i=0; i<num_nodos; i++)		//Inicializamos la matriz de pesos
  	for(j=0; j<num_nodos; j++){
		d1[i][j] = 0;
		d2[i][j] = 0;
		simo[i][j] = 0;
		pesos_originales[i][j] = 0;
		}

  //Dependiendo del formato de entrada leemos de una forma u otra.
  if(strncmp(cadena,"*matrix",7)==0 || strncmp(cadena,"*Matrix",7)==0 || strncmp(cadena,"*MATRIX",7)==0){
	for(i=0; i<num_nodos; i++)
		for(j=0; j<num_nodos; j++){
			int ret = fscanf(fich,"%f",&aux);
			  if(ret==0) {
				printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
				exit(2);
			  }
			if(aux!=0){
				pesos_originales[i][j] = aux;
				if(aux > minimo)
				    minimo = aux;
				}
			}

  }else if(strncmp(cadena,"*edges",6)==0 || strncmp(cadena,"*Edges",6)==0 || strncmp(cadena,"*EDGES",6)==0){
  	while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF ){
		if(i==j)
			pesos_originales[i-1][j-1] = 0;
		else
  			pesos_originales[i-1][j-1] = aux;
  		if(aux > minimo)
		    minimo = aux;
  		}
  }else{
	printf("[ERROR] - Formato del fichero de entrada no valido.\n");
	exit(3);
  }


  /*Paso la matriz de pesos a enteros para que las operaciones sean mas rapidas
  for(i=0; i<num_nodos; i++)
    for(j=0; j<num_nodos; j++){
        if (j==i) 
		pesos_originales[i][j]=0;
        simo[i][j] = (unsigned int) (pesos_originales[i][j] * factor);
	  sim[i][j]=simo[i][j];
        }*/

  for(i=0; i<num_nodos; i++)
    for(j=i; j<num_nodos; j++){
        if (j==i) 
		pesos_originales[i][j]=0;
        else if (pesos_originales[i][j]==0)
            pesos_originales[i][j]= pesos_originales[j][i];
        else if (pesos_originales[j][i]==0)
            pesos_originales[j][i]= pesos_originales[i][j];
        else if (pesos_originales[j][i]!=pesos_originales[i][j])
            {
		pesos_originales[i][j]= (pesos_originales[i][j]+pesos_originales[j][i])/2;
            pesos_originales[j][i]= pesos_originales[i][j];
            }
        simo[i][j] = pesos_originales[i][j];
	  simo[j][i]=simo[i][j];
	  sim[i][j]=simo[i][j];
	  sim[j][i]=simo[i][j];
        }



}


/***************************************************************************************/
/***************************************************************************************/
void sumasims(float **ac, float **s, float **r){
  int j,k,z;
  float maximo, minimo_local;

 	for(j=0;j<num_nodos;j++){
		for(k=0;k<num_nodos;k++){
			maximo = 0;
			for(z=0;z<num_nodos && j!=k ;z++){
				if( ac[j][z]!=0 && s[z][k]!=0 && z!=k && z!=j){
					if(ac[j][z] < s[z][k])
						minimo_local = ac[j][z];
					else
						minimo_local = s[z][k];

					if(minimo_local > maximo && minimo_local!=0)
						maximo = minimo_local;
					}
				}
			if(maximo > ac[j][k])
				r[j][k] = maximo;
                  else
				r[j][k] = ac[j][k];

		}  //for k
	}  //for j
}

void copiamatriz(float **s1, float **s2){
int i, j;
  for(i=0; i<num_nodos; i++)
    for(j=0; j<num_nodos; j++)
	  s1[i][j]=s2[i][j];

}

void binary (){
  int i,j, privez=1, nq=0;
  float **temp2;	//matriz de distancias


  if (q%2){
     copiamatriz(simacum,sim);
     privez=0;
     nq++;

  }
  //Calculo de las q matrices de pesos
  for(i=1;(i*2)<=q;i*=2){
      sumasims(sim, sim, temp);
      temp2=sim;
	sim=temp;
	temp=temp2;
      if ((q-nq)%(4*i))
        {
        if (privez){
	      copiamatriz(simacum,sim);
     		privez=0;
            }
        else
            {
            sumasims(simacum, sim, temp);
            temp2=simacum;
            simacum=temp;
            temp=temp2;
            
             }
          nq+=(2*i);
         }
  } //for i


  if (q != nq)
	fprintf(stderr, "Error de q: %d - %d\n", q, nq);

  //Calculo de la PFNET a partir de las matrices de pesos y distancias
  for(i=0;i<num_nodos;i++)
  	for(j=0;j<num_nodos;j++){
		if(simacum[i][j]==simo[i][j] && simo[i][j]!=0)
			pfnet[i][j]=pesos_originales[i][j];
		else if (simo[i][j]!=0)
			pfnet[i][j] = FALSE;
            }
}


int _main(int argc, char *argv[]){
  int i, j;

  Input(argc,argv);

//#if CHRONOMETER == 0
  binary();
/*#else
  baseh_chronostart();
  for(r=0;r<CHRONOMETER;r++)
    binary();
  fprintf(stderr," Temps(s): %f\n",baseh_chronostop(0)/CHRONOMETER);
#endif
*/
  for(i=0; i<num_nodos; i++){
  	for(j=i+1; j<num_nodos; j++)
         if (pfnet[i][j] > 0)
		printf("%d %d %" NDIGITS "f\n", i+1, j+1, pfnet[i][j]);
	}
  
  return 0;
}


int main(int argc, char *argv[]){

//#if CHRONOMETER_ALL == 0
  _main(argc, argv);
/*#else
  baseh_chronostart();
  for(r=0;r<CHRONOMETER_ALL;r++)
    _main(argc, argv);
  fprintf(stderr," Temps_(tout)(s): %f\n",baseh_chronostop(0)/CHRONOMETER_ALL);
#endif
*/
}


