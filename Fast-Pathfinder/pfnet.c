#include "global.h"
#include "routines.h"



int _main(int argc, char *argv[]){
  int i, j, r;

  Input(argc,argv);
  
	/******************/
	 /*printf("***** Matrix saltos[][] :\n");
	 for(i=0;i<num_nodos;i++)
	 {
	 for(j=0;j<num_nodos;j++)
	 {
	 printf(" %d", (int)saltos[i][j]);
	 }
	 printf("\n");
	 }
	
	printf("q: %d\n", q);*/
	
	 /******************/
	
#if CHRONOMETER == 0
  Programacion_Dinamica();
#else
  //baseh_chronostart();
  for(r=0;r<CHRONOMETER;r++)
  {
    Programacion_Dinamica();
    //fprintf(stderr," %f",baseh_chronostop(0));
  }
  //fprintf(stderr," Temps(s): %f\n",baseh_chronostop(0)/CHRONOMETER);
#endif

  // Print the new .NET on the standard output.  
  if(is_sym == 1)
    save_net(pfnet, stdout, OUTPUT_EDGES);
  else
    save_net(pfnet, stdout, OUTPUT_ARCS);
  
  /*// Print the "*predecessors" matrix
  printf("*predecessor\n");
  for(i=0;i<num_nodos;i++)
  {
    for(j=0;j<num_nodos;j++)
    {
      printf("%d ", (int)predecessors[i][j]);
    }
    printf("\n");
  }*/
  
	Libera_matriz_dinamica_float (pesos_originales, num_nodos);
	Libera_matriz_dinamica_float (pfnet, num_nodos);

	Libera_matriz_dinamica_float (pesos, num_nodos);
	Libera_matriz_dinamica_int (saltos, num_nodos);
	
#if ORIGINAL == 2
  Libera_matriz_dinamica_char (predecessors, num_nodos);
#endif

  return 0;
}


int main(int argc, char *argv[]){
  int r;

#if CHRONOMETER_ALL == 0
  _main(argc, argv);
#else
  /* baseh_chronostart();
  for(r=0;r<CHRONOMETER_ALL;r++)
  {
    _main(argc, argv);
    //fprintf(stderr," %f",baseh_chronostop(0));
  }
  fprintf(stderr," Temps_(tout)(s): %f\n",baseh_chronostop(0)/CHRONOMETER_ALL);
  */
#endif

}
