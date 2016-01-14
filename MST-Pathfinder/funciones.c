#include "global.h"

#define ENTRADA_1 "*vertices %i\n"
#define ENTRADA_2 "*Vertices %i\n"

#if ORIGINAL == 0
  // Original Kruskal
static int compedges_min(const void *_e1, const void *_e2);
#endif
static int compedges_max(const void *_e1, const void *_e2);


/**********************************************************************************/
/***************** LEE LOS VALORES DESDE EL FICHERO DE ENTRADA ********************/
/**********************************************************************************/

void Input(int argc, char *argv[]){

  FILE *fich;
  int i, j, k;
  char cadena[1024];
  float aux;
#if DEBUG == 2
  char debug_f1[256], *debug_f2, *debug_f3, *debug_f4, *debug_filename;
  time_t rawtime;
  FILE *debug_file;
#endif

  if(argc>3){
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
	
#if DEBUG == 2
  time ( &rawtime );
  sprintf(debug_f1, "KFDEBUG_%s.net", ctime (&rawtime));
  debug_f2 = strreplace_all(debug_f1, " ", "_");
  debug_f3 = strreplace_all(debug_f2, "\n", "");
  debug_f4 = strreplace_all(debug_f3, ":", "-");
  debug_filename = debug_f4;
  free(debug_f2);
  free(debug_f3);
  debug_file = fopen(debug_filename, "w");
  if(debug_file == NULL)
  {
	printf("\n[ERROR] - Cannot open in write access the file '%s'.\n", debug_filename);
	free(debug_filename);
	exit(1);
  }
  while(1)
  {
    cadena[0]=0;
    fgets(cadena, 1024, fich);
    fprintf(debug_file, "%s", cadena);
    if(feof(fich)) break;
  }
  fclose(debug_file);
  rewind(fich);
  free(debug_filename);
#endif

  num_nodos = get_num_nodes(fich); //lee el numero de nodos
  printf(ENTRADA_1, num_nodos);		    //Mostramos por pantalla el numero de nodos
  
  if(argc == 3)
  	q = atoi(argv[2]);
  else
  	q = num_nodos -1;

  pfnet = Matriz_dinamica_float(num_nodos, num_nodos);
  pesos_originales = Matriz_dinamica_float(num_nodos, num_nodos);

  for(i=0; i<num_nodos; i++) {		//Inicializamos la matriz de pesos
  	char* ret = fgets(cadena, 1024, fich);	// obviamos la informacion de cada nodo
	USE(ret);
	printf("%s", cadena);
  	for(j=0; j<num_nodos; j++) {
		pesos_originales[i][j] = 0.0;		//Inicializamos la matriz de pesos
		pfnet[i][j] = FALSE;				//Inicializamos la matriz pfnet[][]
	}
  }
  char* ret = fgets(cadena, 1024, fich);
  USE(ret);

	
  // Initialisation of the number of edges
  num_edges = 0;
	
  //Dependiendo del formato de entrada leemos de una forma u otra.
  if(strncmp(cadena,"*matrix",7)==0 || strncmp(cadena,"*Matrix",7)==0 || strncmp(cadena,"*MATRIX",7)==0)
	{
	for(i=0; i<num_nodos; i++)
		for(j=0; j<num_nodos; j++)
			{
			int reti = fscanf(fich,"%f",&aux);
			USE(reti);
			if((aux > 0) && (pesos_originales[i][j] == 0.0) && (i!=j))
				num_edges++;
			pesos_originales[i][j] = aux;
			}

  	}
  else
	{
	if(strncmp(cadena,"*edges",6)==0 || strncmp(cadena,"*Edges",6)==0 || strncmp(cadena,"*EDGES",6)==0)
		{
  		while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF )
			{
#if DEBUG == 1
			if(pesos_originales[i-1][j-1] != 0.0)
			  printf("@@ %d %d %f\n", i, j, pesos_originales[i-1][j-1]);
#endif
			if((aux > 0) && (pesos_originales[i-1][j-1] == 0.0))
				num_edges++;
			if((aux > 0) && (pesos_originales[j-1][i-1] == 0.0))
				num_edges++;
			pesos_originales[i-1][j-1] = aux;
			/* si el fichero de entrada solo tiene los ejes de la media matriz diagonal */
			pesos_originales[j-1][i-1] = aux;
  			}
		}
  	else
		{
		printf("[ERROR] - Formato del fichero de entrada no valido (found: '%s').\n", cadena);
		exit(3);
  		}
	}
	
  // Only undirected graphs are used with Kruskal
  num_edges /= 2;
	
  //printf("Number of edges: %d\n", num_edges);
  
  // Allocation of vedges[]
  vedges = (SEDGE **) malloc(sizeof(SEDGE*)*(num_edges));
  if(vedges == NULL){
  	printf("\n[ERROR] - Cannot allocate the memory for vedges.\n");
	exit(3);
  	}
      
  // Allocation of the list of edges
  // [OPTIMISATION] This list can be filled during the reading
  // of the input file, so pesos_originales[] is no longer needed.
  k=0;
  for(i=0;i<num_nodos;i++)
    for(j=i+1;j<num_nodos;j++)
      if(pesos_originales[i][j] > 0)
      {
        vedges[k] = (SEDGE *)malloc(sizeof(SEDGE));
	k++;
      }
      
  if(k!=num_edges)
  {
    printf("\n[WARNING] - The are some non null values on the diagonal!\n");
    //exit(3);
  }
      
  // Allocation of compConnexe[]
  ds_alloc(num_nodos);
	
  // Allocation of sameValuesNodes[]
  sameValuesNodes = (int *) malloc(sizeof(int)*(num_edges));
  if(sameValuesNodes == NULL){
  	printf("\n[ERROR] - Cannot allocate the memory for sameValuesNodes.\n");
	exit(3);
  	}

      
  fclose(fich);
}


/***************************************************************************************/
/***************************** PROGRAMACIÓN DINAMICA ***********************************/
/***************************************************************************************/

/***

Algorithm Kruskal

Modified to take account of the maximal path value corresponding to the
minimum edge value. Only for undirected graph!

Useful variables:
q - value of 'q' in the PFNet Algorithm
num_nodos - number of nodes
pfnet[][] - used to return the final matrix
pesos_originales[][] - original weight matrix (eq. to W(1) or D(1))

***/
void Programacion_Dinamica (){
  int i,j,k;
  int size_svn;  // Size of the sameValuesNodes vector
  float firstv;
  int s1, s2, s1_k, s2_k;


  // Initialisation of the list of edges
  // [OPTIMISATION] This list can be filled during the reading
  // of the input file, so pesos_originales[] is no longer needed.
  k=0;
  for(i=0;i<num_nodos;i++)
    for(j=i+1;j<num_nodos;j++)
      if(pesos_originales[i][j] > 0)
      {
	vedges[k]->i = i;
	vedges[k]->j = j;
	vedges[k]->v = pesos_originales[i][j];
	k++;
      }
      
  // Initialisation of compConnexe[]
  ds_init(num_nodos);
   
   
   
  // Sort all the edges of the graph
#if ORIGINAL == 0
  // Original Kruskal
  qsort(vedges, num_edges, sizeof(vedges[0]), compedges_min);
#else
  // Modified Kruskal
  qsort(vedges, num_edges, sizeof(vedges[0]), compedges_max);
#endif

  // Select the edges if they do not form a connected component
  j = 0;

#if ORIGINAL < 2
  // Original/modified Kruskal
  while ((k < num_nodos-1) && (j<num_edges))
  {
    int s1 = vedges[j]->i;
    int s2 = vedges[j]->j;
    if (!ds_same(s1, s2))
    {
      k++;
      pfnet[s1][s2] = vedges[j]->v;
      ds_join(num_nodos,s1,s2);
    }
    j++;
  }

#else
  // Adaptation for PFNet: if the current edge have the same value that the last considered
  // edge, then the qsort ordering have chosen in an "a priori" way an order that does not
  // exist in reality (sequence 1-2-2 is not exactly the same that 1-2-2). Hypothesis:
  // the PFNet algorithm would include the both in the final edges
  
  while (j<num_edges)
  {
    size_svn = 0;

    // First, discover all the edges with the same values, connecting two
    // different connected components
    
    firstv = vedges[j]->v;
    for(;j<num_edges;j++)
    {
      //printf("k=%d, j=%d, s1=%d, s2=%d, v(i-j)=%f, lastv=%f\n", k, j, s1+1, s2+1, vedges[j]->v, lastv);
      if(firstv==vedges[j]->v)
      {
        s1 = vedges[j]->i;
        s2 = vedges[j]->j;
        if (!ds_same(s1, s2))
        {
	  sameValuesNodes[size_svn] = j;
          size_svn++;
          pfnet[s1][s2] = vedges[j]->v;
          //printf("GOT edge j=%d !\n", j);
          //lastv=vedges[j]->v; // to optimize
        }
      }
      else
	break;
    }

    // Second, connect all the connected components found
    
    //printf("Now, connect all the parts\n");
    //printf("** sameValuesNodes = [");
    //for (i=0;i<size_svn;i++)
    //  printf(" %d", sameValuesNodes[i]);
    //printf("]\n");
      
      
    for(k=0;k<size_svn;k++)
    {
       
      s1_k = vedges[sameValuesNodes[k]]->i;
      s2_k = vedges[sameValuesNodes[k]]->j;
	
      //printf("k=%d / %d, s1k=%d, s2k=%d, edge=%d\n", k, size_svn, s1_k+1, s2_k+1, sameValuesNodes[k]);

      ds_join(num_nodos, s1_k, s2_k);
    }
      
      //printf("** compConnexe = [");
      //for (i=0;i<num_nodos;i++)
      //  printf(" %d", compConnexe[i]);
      //printf("]\n");
      
   }
#endif
  
  /*for(k=0;k<num_edges;k++)
  {
    printf("Edge (%d, %d), value=%f\n",
    	vedges[k]->i, vedges[k]->j, vedges[k]->v);
  }*/
    
  /*// Print pfnet[][]
  printf("***** Matrix pfnet[][] :\n");
  for(i=0;i<num_nodos;i++)
  {
    for(j=0;j<num_nodos;j++)
    {
      printf(" %f", pfnet[i][j]);
    }
    printf("\n");
  }
  printf("*****\n");*/
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



/***
Used to compare two elements of an edge vector.
***/

// Min value is ordered first
#if ORIGINAL == 0
  // Original Kruskal
static int compedges_min(const void *_e1, const void *_e2)
{
  SEDGE **e1 = (SEDGE **) _e1;
  SEDGE **e2 = (SEDGE **) _e2;
  if((*e1)->v > (*e2)->v) return 1;
  else if((*e1)->v < (*e2)->v) return -1;
  else return 0;
}
#endif

// Max value is ordered first
static int compedges_max(const void *_e1, const void *_e2)
{
  SEDGE **e1 = (SEDGE **) _e1;
  SEDGE **e2 = (SEDGE **) _e2;
  if((*e2)->v > (*e1)->v) return 1;
  else if((*e2)->v < (*e1)->v) return -1;
  else return 0;
}


/***
Operators on disjoined sets
(classical functions based on the use of an index)
FASTEST WAY !

ds_init()	O(n)
ds_find()	O(1)
ds_join()	O(n)
ds_same()	O(1)
***/

#if BASIC_FUNCTIONS == 0

// Allocation of 'n' disjoined sets
inline void ds_alloc(int n)
{
  // Allocation of compConnexe[]
  compConnexe = (int *) malloc(sizeof(int)*(n));
  if(compConnexe == NULL){
  	printf("\n[ERROR] - Cannot allocate the memory for compConnexe.\n");
	exit(3);
  }
}

// Initialisation of 'n' disjoined sets
inline void ds_init(int n)
{
  int k;

  // Prepare the connected components
  for(k=0;k<n;k++)
    compConnexe[k]=k;
}

// Find the representant of the set of the element x
inline int ds_find(int x)
{
  return compConnexe[x];
}

// Join the two sets represented by x and y
inline void ds_join(int n, int x, int y)
{
  int i, aux = compConnexe[y];
  for (i=0;i<n;i++)
    if (compConnexe[i] == aux)
      compConnexe[i] = compConnexe[x];
}

// Return 1 if x and y are in the same set
inline int ds_same(int x, int y)
{
  return compConnexe[x] == compConnexe[y];
}

#endif


/***
Operators on disjoined sets
(optimized, based on forests)
SLOWEST WAY ! (because there is a lot of ds_same() operations,
and this version is the slowest version)

ds_init()	O(n)
ds_find()	O(log(n))
ds_join()	O(2.log(n))
ds_same()	O(2.log(n))
***/

#if BASIC_FUNCTIONS == 1

// Allocation of 'n' disjoined sets
inline void ds_alloc(int n)
{
  // Allocation of compConnexe[]
  compConnexe = (int *) malloc(sizeof(int)*(n));
  if(compConnexe == NULL){
  	printf("\n[ERROR] - Cannot allocate the memory for compConnexe.\n");
	exit(3);
  }
  
  // Allocation of compConnexeRank[]
  compConnexeRank = (int *) malloc(sizeof(int)*(n));
  if(compConnexeRank == NULL){
  	printf("\n[ERROR] - Cannot allocate the memory for compConnexeRank.\n");
	exit(3);
  }
  
  // Allocation of compConnexeBuffer[]
  compConnexeBuffer = (int *) malloc(sizeof(int)*(n));
  if(compConnexeBuffer == NULL){
  	printf("\n[ERROR] - Cannot allocate the memory for compConnexeBuffer.\n");
	exit(3);
  }
}

// Initialisation of 'n' disjoined sets
inline void ds_init(int n)
{
  int k;

  // Prepare the connected components
  for(k=0;k<n;k++)
  {
    compConnexe[k]=k;
    compConnexeRank[k]=0;
  }
}

// Find the representant of the set of the element x
inline int ds_find(int x)
{
  //if(x != compConnexe[x])
  //  compConnexe[x] = ds_find(compConnexe[x]);
  //return compConnexe[x];
  
  int k, s=0;
  while(x != compConnexe[x])
  {
    compConnexeBuffer[s] = x;
    s++;
    x = compConnexe[x];
  }
  for(k=0;k<s-1;k++)
    compConnexe[compConnexeBuffer[k]]=x;
  return x;
}

// Join the two sets represented by x and y
inline void ds_join(int n, int x, int y)
{
  int cx = ds_find(x);
  int cy = ds_find(y);
  if(compConnexeRank[cx]>compConnexeRank[cy])
    compConnexe[cy] = cx;
  else
    compConnexe[cx] = cy;
  if(compConnexeRank[cx]==compConnexeRank[cy])
    compConnexeRank[cy] = compConnexeRank[cy]+1;
}

// Return 1 if x and y are in the same set
inline int ds_same(int x, int y)
{
  //return ds_find(x) == ds_find(y);
  
  if(x != compConnexe[x]) x = ds_find(x);
  if(y != compConnexe[y]) y = ds_find(y);
  return x == y;
}

#endif


// Replace the string 'replace' in 'src' by 'to'
// doing the correct memory allocation
char *strreplace(char *src, char *replace, char *to)
{

        char *dest = NULL;
        char *retstr = NULL;

        if(src == NULL )
                return NULL;

        //returning the src if replace is NULL
        if(replace == NULL || to == NULL)
        {

                dest = (char *)malloc(strlen(src) + 1);
                if(dest == NULL)
                        return NULL;
                else
                        strcpy(dest, src);

                return dest;
        }

        //if replace is found in src
        if((retstr = strstr(src, replace)) != NULL)
        {

            int beforelen = retstr - src;
            int replacelen = strlen(replace);
            dest = (char *)malloc(strlen(src) + 1 + strlen(to) - replacelen);
            if(dest == NULL)
                    return NULL;

            strncpy(dest,src, beforelen);
            strcat(dest, to);
            strcat(dest, &src[beforelen + replacelen]);
            return dest;

        }
        else //return the string itself
        {
                dest = (char *)malloc(strlen(src) + 1);
                if(dest == NULL)
                        return NULL;

                else
                        strcpy(dest, src);

                return dest;

        }
}

// Replace all the occurence of 'replace' in 'src' by 'to'
char *strreplace_all(char *_src, char *replace, char *to)
{
     char* ret = NULL;
     char* src = strdup(_src);
     if(src == NULL)
     	return NULL;
     while(1)
     {
       ret = strreplace(src, replace, to);
       if((ret==NULL) || !strcmp(src, ret))
       {
         free(src);
         return ret;
       }
       free(src);
       src = ret;
     } 
}

/***
Get the number of nodes.
***/
int get_num_nodes(FILE* fich)
{
  int ret, nnodes;
  char cadena[1025];
  char* retc = fgets(cadena, 1024, fich);
  USE(retc);
  ret = sscanf(cadena, ENTRADA_1, &nnodes);	//lee el numero de nodos
  if(ret != 0) return nnodes;
  ret = sscanf(cadena, ENTRADA_2, &nnodes);
  if(ret != 0) return nnodes;
  fprintf(stderr, "\n[ERROR] - Nor '%s' or '%s' has been found on line 1 !\n", ENTRADA_1, ENTRADA_2);
  return 0;
}

