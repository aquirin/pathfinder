#include "input_pajek.h"


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


/***
Open .NET files (matrix, edges or arcs)
Read the core part (definition of the edges)
***/
void open_net(FILE* fich, Graph* G)
{
  int i, j;
  float aux;
  char cadena[1025];
  
  char* ret = fgets(cadena, 1024, fich);
  USE(ret);

  // Initialisation of the number of edges
  //G->num_existing = 0;
  //G->num_different = 0;
  
  //Dependiendo del formato de entrada leemos de una forma u otra.
  if(strncmp(cadena,"*matrix",7)==0 || strncmp(cadena,"*Matrix",7)==0 || strncmp(cadena,"*MATRIX",7)==0)
  {
	//G->graph_type = "matrix";
	for(i=0; i<G->nodes(); i++)
		for(j=0; j<G->nodes(); j++)
		{
			int reti = fscanf(fich,"%f",&aux);
			USE(reti);
			if(aux != 0)
				G->add_edge(i, j, aux, NULL, 1);
			/*if(G->weights[i][j] != 0.0)
			{
				if(G->weights[i][j] != aux)
					G->num_different++;
				G->num_existing++;
			}
			if(aux != 0)
				G->weights[i][j] = aux;*/
		}

  }
  else if(strncmp(cadena,"*edges",6)==0 || strncmp(cadena,"*Edges",6)==0 || strncmp(cadena,"*EDGES",6)==0)
  {
	//G->graph_type = "edges";
  	while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF )
	{
		/*if(G->weights[i-1][j-1] != 0.0)
		{
			if(G->weights[i-1][j-1] != aux)
				G->num_different++;
			G->num_existing++;
		}
		if(aux != 0)
		{
			G->weights[i-1][j-1] = aux;
			// si el fichero de entrada solo tiene los ejes de la media matriz diagonal
			G->weights[j-1][i-1] = aux;
		}*/
		if(aux != 0) {
			G->add_edge(i-1, j-1, aux, NULL, 0);
		}

  	}
  }
  else if(strncmp(cadena,"*arcs",5)==0 || strncmp(cadena,"*Arcs",5)==0 || strncmp(cadena,"*ARCS",5)==0)
  {
	//G->graph_type = "arcs";
  	while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF )
	{
		/*if(G->weights[i-1][j-1] != 0.0)
		{
			if(G->weights[i-1][j-1] != aux)
				G->num_different++;
			G->num_existing++;
		}
		if(aux != 0)
			G->weights[i-1][j-1] = aux;*/
		if(aux != 0)
			G->add_edge(i-1, j-1, aux, NULL, 1);

  	}
  }
  else
  {
	//G->graph_type = "error";
	//printf("[ERROR] - (open_net) Formato del fichero de entrada no valido. '%s' not recognized !\n", cadena);
	//exit(3);
  }
  

  // Compute the statistics
  //G->num_edges = get_num_edges(G);

  /*// Print pesos[][][]
  printf("***** Matrix weights[][] :\n");
  for(i=0;i<num_nodes;i++)
  {
    for(j=0;j<num_nodes;j++)
    {
      printf(" %f", weights[i][j]);
    }
    printf("\n");
  }
  printf("*****\n");*/
	
}


/***
Open .NET files in a new table (matrix, edges or arcs)
If __node_table is not null, store also the names of the nodes in
__node_table[].category
***/
/*float ** open_new_net(FILE* fich, int* __num_nodes, SNODE **__node_table)
{
  int i, j, k;
  float aux;
  char cadena[1024];
  int _num_nodes;
  float **_pesos_originales;
  char *pos1, *pos2;

  // Start to scan the input file
  _num_nodes = get_num_nodes(fich);

  // Allocation of pesos_originales[][]
  _pesos_originales = Matriz_dinamica_float(_num_nodes, _num_nodes);
  
  // Allocation of __node_table[]
  if((__node_table) != NULL)
  {
    if(((*__node_table)=(SNODE *)calloc(_num_nodes,sizeof(SNODE)))==NULL){
         fprintf(stderr, "\n[ERROR] - Error during the allocation of __node_table !");
         exit(3);}
  }
       
  // Initialize the weight matrix
  for(i=0; i<_num_nodes; i++)
  	for(j=0; j<_num_nodes; j++){
		_pesos_originales[i][j] = 0.0;
	}
       
  // Read the name of each node for the second file (union/difference)
  for (i=0; i<_num_nodes;i++)
  {
    fgets(cadena, 1024, fich);
    if((__node_table) != NULL)
    {
      // Read also the name of each nodes
      (*__node_table)[i].line = strdup(cadena);
      pos1 = strchr(cadena, '\"');
      pos2 = strchr(pos1+1, '\"');
      *pos2 = 0;
      (*__node_table)[i].visible = 1;
      (*__node_table)[i].category = strdup(pos1+1);
      (*__node_table)[i].field = NULL;
		char* strpos = strtok(pos2+1, " ");
		if(!strcmp(strpos, "ellipse"))
		{
			(*__node_table)[i].posx = 0;
			(*__node_table)[i].posy = 0;
		}
		else
		{
			(*__node_table)[i].posx = atof(strpos);
			(*__node_table)[i].posy = atof(strtok(NULL, " "));
		}
    }
  }

  // Read the type of the input format
  fgets(cadena, 1024, fich);

  //Dependiendo del formato de entrada leemos de una forma u otra.
  if(strncmp(cadena,"*matrix",7)==0 || strncmp(cadena,"*Matrix",7)==0 || strncmp(cadena,"*MATRIX",7)==0)
  {
	for(i=0; i<_num_nodes; i++)
		for(j=0; j<_num_nodes; j++)
		{
			fscanf(fich,"%f",&aux);
			if(aux != 0)
				_pesos_originales[i][j] = aux;
		}

  }
  else if(strncmp(cadena,"*edges",6)==0 || strncmp(cadena,"*Edges",6)==0 || strncmp(cadena,"*EDGES",6)==0)
  {
  	while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF )
	{
		if(aux != 0)
		{
			_pesos_originales[i-1][j-1] = aux;
			// si el fichero de entrada solo tiene los ejes de la media matriz diagonal
			_pesos_originales[j-1][i-1] = aux;
		}

  	}
  }
  else if(strncmp(cadena,"*arcs",5)==0 || strncmp(cadena,"*Arcs",5)==0 || strncmp(cadena,"*ARCS",5)==0)
  {
  	while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF )
	{
		if(aux != 0)
			_pesos_originales[i-1][j-1] = aux;
  	}
  }
  else
  {
	printf("[ERROR] - (open_new_net) Formato del fichero de entrada no valido. '%s' not recognized !\n", cadena);	
	exit(3);
  }
  
  //fclose(fich);
  *__num_nodes = _num_nodes;
  return _pesos_originales;
}*/



/***
Write .NET files (matrix, edges or arcs)
***/
/*void save_net(Graph *G, FILE* fich, int type)
{
  int i, j;
 
  switch(type)
  {
  case OUTPUT_MATRIX:
    // Print the "*matrix" matrix
    fprintf(fich, "*matrix\n");
    for(i=0; i<G->num_nodes; i++)
    {
    	for(j=0; j<G->num_nodes; j++)
  	  fprintf(fich, "%.10f ",G->weights[i][j]);
        fprintf(fich, "\n");
    }
  break;
  case OUTPUT_EDGES:
    // Print the "*edges" matrix
    fprintf(fich, "*edges\n");
    for(i=0; i<G->num_nodes; i++)
    	for(j=i+1; j<G->num_nodes; j++)
    		if (G->weights[i][j]!=FALSE)
  		    fprintf(fich, "%d %d %.10f\n",i+1,j+1,G->weights[i][j]);
  break;
  case OUTPUT_ARCS:
    // Print the "*arcs" matrix
    fprintf(fich, "*arcs\n");
    for(i=0; i<G->num_nodes; i++)
    	for(j=0; j<G->num_nodes; j++)
    		if (G->weights[i][j]!=FALSE)
  		    fprintf(fich, "%d %d %.10f\n",i+1,j+1,G->weights[i][j]);
  break;
  default:
	fprintf(stderr, "[ERROR] - Type for the output file not recognized.\n");
	exit(3);
  }
}*/




/**************************************************\
Open a graph DB in the Pajek format.
Notes:
- Read only the first graph of the DB.
\*************************************************/
vector<Graph*> open_pajek_file(GLOBAL* gP, FILE* fin) {

  // OUTPUT
  Graph *G;
  
  // MISC
  int i;
  char cadena[1025];
  char *pos1, *pos2;
  //char *buffer;
  int non_recognized = 0;	// Number of nodes having non recognized categories

  // Allocation of the graph
  G = new Graph(0);
		 
  // Start to scan the input file
  int num_nodes = get_num_nodes(fin);
  
  // Read the name of each node
  for (i=0; i<num_nodes;i++){		//obviamos la informacion de cada nodo
  	char* ret = fgets(cadena, 1024, fin);
	USE(ret);
	//G->node_table[i].line = strdup(cadena);
	pos1 = strchr(cadena, '\"');
	pos2 = strchr(pos1+1, '\"');
	*pos2 = 0;
	G->add_node(pos1+1);
	// field = ((buffer=find_field(gP, G->node_table[i].category)) != NULL)?(strdup(buffer)):(NULL);
	/*char* strpos = strtok(pos2+1, " ");
	if(strcmp(strpos, "ellipse"))
	{
		G->node_table[i].posx = 0;
		G->node_table[i].posy = 0;
	}
	else
	{
		G->node_table[i].posx = atof(strpos);
		G->node_table[i].posy = atof(strtok(NULL, " "));
	}*/
	
	/*if(G->node_table[i].field == NULL)
		{
			if((gP->filename_ISI_DB!=NULL) && (ALLOW_CHECK_RECOGNIZED || (gP->check_type==CHECK_RECOGNIZED) || (gP->check_type==CHECK_ALL)))
				fprintf(stderr, "[ERROR] The category '%s' of the node %d was not recognized !\n", G->node_table[i].category, i+1);
			non_recognized++;
		}*/
  }
  if(non_recognized>0)
  if((gP->check_type==CHECK_TOT_RECOGNIZED) || (gP->check_type==CHECK_ALL))
	fprintf(stderr, "\n[ERROR] The categories of %d nodes were not recognized !\n", non_recognized);
	
  // Read the content of the file
  open_net(fin, G);
  fclose(fin);
  
  vector<Graph*> DB;
  DB.push_back(G);
  
  //G->print();

  return DB;
  
}

