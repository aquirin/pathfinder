/*
Objective: to create random .NET files (printed in the output)
Parameters:
- size: the size of the matrix
- symetry: 1 for a symetrical matrix or 0 for a unsymetrical matrix
- diagonal value: the value in the diagonal (for instance, 0)
- type: 0 for integer, 1 for real numbers
- min/max: range of the random values
- proba_infinity: probability to produce the INFINITY value

<program> <size> <symetry> <diagonal value> <type (0 for int)> <min> <max> <proba_infinity>
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define SIZE_MAX 1000000
#define INFINITY 1000		// Value used to code a non-link between 2 nodes

#define MAX(X,Y) (((X)>(Y))?(X):(Y))

// Global parameters
int size, symetry;
float diagonal_value, vmin, vmax, proba_inf, proba_edge;


float aleaf()
{
  return (float)rand()/RAND_MAX;
}

float aleafmm(float min, float max)
{
  return min+(max-min)*aleaf();
}

int aleaimm(int min, int max)
{
  return (int)(min+(max-min)*aleaf());
}

/**************************************************\
TYPE 1.
Compute a random graph using the given parameters.
\**************************************************/
void type1(float** tab) {
  float r;
  int i, j;
  for(i=0;i<size;i++)
  {
    for(j=(symetry?i:0);j<size;j++)
    {
      if(i==j) r=diagonal_value;
      else r=aleafmm(vmin, vmax);
      if(aleaf()<proba_inf) r=INFINITY;
      tab[i][j] = r;
      if(symetry)
        tab[j][i] = r;
    }
  }
}

/**************************************************\
TYPE 2.
Compute a random sparse graph using the given parameters.
\**************************************************/
void type2(float** tab) {
  float r;
  int i, j;
  for(i=0;i<size;i++)
  {
    for(j=i+1;j<size;j++)
    {
      if(aleaf()<proba_edge) {
        r=aleafmm(vmin, vmax);
        if(aleaf()<proba_inf) r=INFINITY;
        tab[i][j] = r;
      }
    }
  }
}

/**************************************************\
TYPE 3.
Compute a random tree using the given parameters.
Fully connected.
\**************************************************/
/*void type3_sub(float** tab, int root, int* nc);
void type3(float** tab) {
  int i;
  int *nc = (int*) malloc(size*sizeof(int));	// Not connected table
  for(i=0;i<size;i++)
    nc[i] = -2;
  type3_sub(tab, -1, 0, nc);
  free(nc);
}
void type3_sub(float** tab, int father, int root, int* nc) {
  int i, r;
  nc[root] = father;
  for(i=0;i<size;i++)
    if((nc[i]==-2) && (aleaf()<proba_edge)) {
      tab[root][i] = aleafmm(vmin, vmax);
      nc[i]==root;
      type3_sub(tab, i, nc);
    }
  for(i=0;i<size;i++)
    if(nc[i]==2) {
      type3_sub(tab, i, nc);
    }
}*/
void type3(float** tab) {
  int i, tofill, num, previous, current;
  int *nc = (int*) malloc(size*sizeof(int));	// Not connected table
  float r;
  for(i=0;i<size;i++)
    nc[i] = 1;
  nc[0] = 0;
  tofill = size-1;
  previous = 0;
  current = 0;
  
  while(tofill>0) {
    num = aleaimm(1,MAX(1,proba_edge*tofill));
    //printf("num %d, tofill %d\n", num, tofill);
    for(i=current+1;i<current+num+1;i++) {
      tofill--;
      //printf("%d %d %d\n", previous, i, current);
      r=aleafmm(vmin, vmax);
      tab[previous][i] = r;
      if(symetry)
        tab[i][previous] = r;
    }
    current = i-1;
    previous++;
  }
  
  free(nc);
}

int main(int argc, char** argv)
{
  int i, j, type, mat_type;
  float **tab;
  
  if(argc!=10)
  {
    fprintf(stderr, "Usage: %s <map-type (1-3)> <size> <symetry> <diagonal value> <value-type (0 for int, 1 for real)> <min> <max> <proba_infinity> <proba_edge>\n", argv[0]);
    exit(1);
  }
  
  struct timeval timer;
  gettimeofday(&timer, 0);
  srand(timer.tv_usec);

  // Obtain the parameters
  mat_type = atoi(argv[1]);
  size = atoi(argv[2]);
  symetry = atoi(argv[3]);
  diagonal_value = atof(argv[4]);
  type = atoi(argv[5]);
  vmin = atoi(argv[6]);
  vmax = atoi(argv[7]);
  proba_inf = atof(argv[8]);
  proba_edge = atof(argv[9]);
  
  printf("*vertices %d\n", size);
  for(i=0;i<size;i++)
    printf("%d \"%d\" ellipse x_fact 1.22866290766363 y_fact 1.22866290766363 fos 3.5 bw 0.0  ic Emerald\n", i+1, i+1);

  // Initialize the graph
  tab = (float**) malloc(size*size*sizeof(float));
  for(i=0;i<size;i++)
    tab[i] = (float*) malloc(size*sizeof(float));
  for(i=0;i<size;i++)
    for(j=0;j<size;j++)
       tab[i][j] = 0.0;
  
  printf("*matrix\n");
  
  // Compute the matrix
  switch(mat_type) {
  case 1:
    type1(tab);
    break;
  case 2:
    type2(tab);
    break;
  case 3:
    type3(tab);
    break;
  default:
    fprintf(stderr, "Matrix type not recognized!\n");
    return -1;
    break;
  }
  
  // Print the matrix
  for(i=0;i<size;i++)
  {
    for(j=0;j<size;j++)
    {
      if(type==0)
        printf("%d ", (int)tab[i][j]);
      else
        printf("%f ", tab[i][j]);
    }
    printf("\n");
  }
  
  for(i=0;i<size;i++)
    free(tab[i]);
  free(tab);

  exit(0);
}
