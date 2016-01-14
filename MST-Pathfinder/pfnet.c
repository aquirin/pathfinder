#include "global.h"
#include "routines.h"

int _main(int argc, char *argv[]){
  int i, j;

  Input(argc,argv);

#if CHRONOMETER == 0
  Programacion_Dinamica();
#else
  int r;
  baseh_chronostart();
  for(r=0;r<CHRONOMETER;r++)
  {
    Programacion_Dinamica();
    //fprintf(stderr," %f",baseh_chronostop(0));
  }
  fprintf(stderr," Temps(s): %f\n",baseh_chronostop(0)/CHRONOMETER);
#endif

  // Print the "*edges" matrix
  printf("*edges\n");
  for(i=0; i<num_nodos; i++)
  	for(j=0; j<num_nodos; j++)
  		if (pfnet[i][j]!=FALSE)
		    printf("%d %d %" NDIGITS "f\n",i+1,j+1,pfnet[i][j]);

  /*// Print the "*predecessors" matrix
  printf("*predecessor\n");
  for(i=0;i<num_nodos;i++)
  {
    for(j=0;j<num_nodos;j++)
    {
      printf("%d ", (int)predecessors[q+1][i][j]);
    }
    printf("\n");
  }*/
  
  Libera_matriz_dinamica_float (pesos_originales, num_nodos);
  Libera_matriz_dinamica_float (pfnet, num_nodos);

  for(i=0;i<num_edges;i++)
	free(vedges[i]);
  free(vedges);
  
  free(compConnexe);
  free(sameValuesNodes);
  
  return 0;
}

int main(int argc, char *argv[]){

#if CHRONOMETER_ALL == 0
  return _main(argc, argv);
#else
  int r;
  baseh_chronostart();
  for(r=0;r<CHRONOMETER_ALL;r++)
  {
    _main(argc, argv);
    //fprintf(stderr," %f",baseh_chronostop(0));
  }
  fprintf(stderr," Temps_(tout)(s): %f\n",baseh_chronostop(0)/CHRONOMETER_ALL);
  return 0;
#endif

}
