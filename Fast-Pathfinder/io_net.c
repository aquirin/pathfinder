#include "extern.h"
#include "global.h"

/***
Open .NET files (matrix, edges or arcs)
***/
void open_net(FILE* fich)
{
  int i, j, k;
  float aux;
  char cadena[1025];
  
  char* ret = fgets(cadena, 1024, fich);
    
  //Dependiendo del formato de entrada leemos de una forma u otra.
  if(strncmp(cadena,"*matrix",7)==0 || strncmp(cadena,"*Matrix",7)==0 || strncmp(cadena,"*MATRIX",7)==0)
  {
	for(i=0; i<num_nodos; i++)
		for(j=0; j<num_nodos; j++)
		{
			int ret = fscanf(fich,"%f",&aux);
			pesos_originales[i][j] = aux;
            		pesos[i][j] = aux;
		}

  }
  else if(strncmp(cadena,"*edges",6)==0 || strncmp(cadena,"*Edges",6)==0 || strncmp(cadena,"*EDGES",6)==0)
  {
  	while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF )
	{
		pesos_originales[i-1][j-1] = aux;
	       	pesos[i-1][j-1] = aux;
		/* si el fichero de entrada solo tiene los ejes de la media matriz diagonal */
		pesos_originales[j-1][i-1] = aux;
		pesos[j-1][i-1] = aux;
  	}
  }
  else if(strncmp(cadena,"*arcs",5)==0 || strncmp(cadena,"*Arcs",5)==0 || strncmp(cadena,"*ARCS",5)==0)
  {
  	while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF )
	{
		pesos_originales[i-1][j-1] = aux;
	       	pesos[i-1][j-1] = aux;
		
  	}
  }
  else
  {
	printf("[ERROR] - Formato del fichero de entrada no valido.\n");
	exit(3);
  }
  
  /*// Print pesos[][]
  printf("***** Matrix pesos_originales[][] :\n");
  for(i=0;i<num_nodos;i++)
  {
    for(j=0;j<num_nodos;j++)
    {
      printf(" %f", pesos_originales[i][j]);
    }
    printf("\n");
  }
  printf("*****\n");*/
	
}


/***
Return 1 if the matrix encoded in pesos_originales is symmetrical, 0 if not.
***/
int is_symmetrical(void)
{
  int i, j;
  for(i=0; i<num_nodos; i++)
    for(j=i+1; j<num_nodos; j++)
    {
      if(pesos_originales[i][j] != pesos_originales[j][i])
        return 0;
    }
  return 1;
}




/***
Write .NET files (matrix, edges or arcs)
***/
void save_net(float** pfnet, FILE* fich, int type)
{
  int i, j;
 
  switch(type)
  {
  case OUTPUT_MATRIX:
    // Print the "*matrix" matrix
    fprintf(fich, "*matrix\n");
    for(i=0; i<num_nodos; i++)
    {
    	for(j=0; j<num_nodos; j++)
  	  fprintf(fich, "%" NDIGITS "f ",pfnet[i][j]);
        fprintf(fich, "\n");
    }
  break;
  case OUTPUT_EDGES:
    // Print the "*edges" matrix
    fprintf(fich, "*edges\n");
    for(i=0; i<num_nodos; i++)
    	for(j=i+1; j<num_nodos; j++)
    		if (pfnet[i][j]!=FALSE)
  		    fprintf(fich, "%d %d %" NDIGITS "f\n",i+1,j+1,pfnet[i][j]);
  break;
  case OUTPUT_ARCS:
    // Print the "*arcs" matrix
    fprintf(fich, "*arcs\n");
    for(i=0; i<num_nodos; i++)
    	for(j=0; j<num_nodos; j++)
    		if (pfnet[i][j]!=FALSE)
  		    fprintf(fich, "%d %d %" NDIGITS "f\n",i+1,j+1,pfnet[i][j]);
  break;
  default:
	fprintf(stderr, "[ERROR] - Type for the output file not recognized.\n");
	exit(3);
  }


  }
