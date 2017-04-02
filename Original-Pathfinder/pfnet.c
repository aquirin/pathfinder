#include "global.h"
#include "routines.h"

int _main(int argc, char *argv[]){
  int i, j;

  Input(argc,argv);

#if CHRONOMETER == 0
  Programacion_Dinamica();
#else
  baseh_chronostart();
  for(r=0;r<CHRONOMETER;r++)
    Programacion_Dinamica();
  fprintf(stderr," Temps(s): %f\n",baseh_chronostop(0)/CHRONOMETER);
#endif

  for(i=0; i<num_nodos; i++)
  	for(j=i+1; j<num_nodos; j++)
  		if (pfnet[i][j]!=FALSE)
		    printf("%d %d %" NDIGITS "f\n",i+1,j+1,pfnet[i][j]);

  Libera_matriz_dinamica_float (pesos_originales, num_nodos);
  Libera_matriz_dinamica_float (distancias, num_nodos);
  Libera_matriz_dinamica_float (pfnet, num_nodos);

  for(i=0;i<q;i++)
	Libera_matriz_dinamica_float (pesos[i], num_nodos);
  free (pesos);

  return 0;
}

int main(int argc, char *argv[]){

#if CHRONOMETER_ALL == 0
  _main(argc, argv);
#else
  baseh_chronostart();
  for(r=0;r<CHRONOMETER_ALL;r++)
    _main(argc, argv);
  fprintf(stderr,"Temps_(tout)(s): %f\n",baseh_chronostop(0)/CHRONOMETER_ALL);
#endif
}
