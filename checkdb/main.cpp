/*
Toolbox for scientograms.
See the help (-h)
*/

#include "main.h"


/**********************************************************************************/
/***************** LEE LOS VALORES DESDE EL FICHERO DE ENTRADA ********************/
/**********************************************************************************/

void Input(int argc, char *argv[]){

  Graph* graph_main, *graph_union, *graph_diff;
  vector<Graph*> graph_main_db, graph_union_db, graph_diff_db;
  int i, j, k;
  char cadena[256];
  float aux;
  float minimo = -1.0;
  int num_difffields = 0;	// Number of links connecting nodes with two different fields
  float dfl_sum = 0, dfl_num = 0;		// Values for difffields-links
  float non_dfl_sum = 0, non_dfl_num = 0;		// Values for non difffields-links
  int dfl_weird_num = 0, non_dfl_weird_num = 0;		// Cound for (non-)DFL weird links
  int dfl_non_weird_num = 0, non_dfl_non_weird_num = 0;		// Cound for (non-)DFL non weird links
  
  /*if(argc>3){
	printf("\n[ERROR] - Formato de entrada incorrecto.\n\t\t ./pfnet fichero_de_entrada\n");
	exit(1);
	}
  }*/
  
  
  // Analyze all the options
  int opt, meth;
  int nopt=1; // Number of successfully parsed options to declare
              // the command line correct
  char* filename = NULL;
  char* filename_union = NULL;
  char* filename_diff = NULL;
  char* filename_dico = NULL;
  char* buffer;
  // Default values
  int filter_type = 0;
  int nfilter_type = 0;
  int stat_type = 0;
  int input_type = INPUT_PAJEK;
  
  int modif_type = 0;
  int load_scored_db = 0;
  float extra_value = 8.0;		// Value used for weird links, filter threshold, modif. constant. and the difference type
  int do_not_use_predecessors = 0;
  extern char* optarg;
  GLOBAL gP;		// Global parameters
  

  // Initialisations
  gP.filename_ISI_DB = strdup(DATABASE_NAME);
  gP.filename_output = NULL;
  gP.check_type = 0;
  gP.output_type = OUTPUT_NONE;
  gP.output_opt_direction = OUTPUT_OPT_DIRECTED;
  gP.output_opt_label = OUTPUT_OPT_EMPTYLABEL;
  gP.output_opt_sorting = OUTPUT_OPT_UNSORTED;
  gP.num_categories = 0;
  gP.field_table = NULL;
  gP.scored_table = NULL;
  gP.num_scored_relations = 0;
  gP.dico_table = NULL;
  gP.num_dico_entries = 0;
  gP.num_nodos_visible = 0;
  gP.ending_table = NULL;
  gP.isolated_table = NULL;
  gP.pesos = NULL;
  gP.predecessors = NULL;
  gP.size_sp = NULL;
  gP.central_node = 0;
  gP.num_nodes_visible = 0;
  

  // Loop
  while (1)
    {
		// Not yet used: abegjklxz
      opt = getopt( argc, argv, "c:m:u:d:q:r:n:o:hf:w:si:pt:y:v:" );
      if (opt == -1)
        break;
      switch (opt)
        {
	case 'h':
	  // -h : help
	  printf("Usage: %s [options]\n"
	  "AT LEAST ONE OF THEM IS NEEDED (evaluated in the following order):\n"
	  "\t-c <type>        Do some checks (all, recognized, tot_recognized, diff_field, all_dfl_weird). See also -w (for all_dfl_weird)\n"
	  "\t-m <type>        Modification of non-zero weights (add, sub_from_cst, set, multiply, compare_with). See -w\n"
	  "\t-u <filename>    Union with the specified network (must have the same number and values of nodes). See also -o\n"
	  "\t-d <filename>    Difference with the specified network (the -d network is considered as the old one) (if -w=0, get only the unsigned difference ABS(SIGN(x)-ABS(SIGN(y), if w=1 get the floating difference). See also -o and -w\n"
	  "\t-q <type>        Filter (keep) nodes (all, unisolated)\n"
	  "\t-r <type>        Filter (keep) links (all, less, greater, equal, less_equal, greater_equal, sfl, dfl, filter3). See also -w (for less/greater/equal/less_equal/greater_equal) and -o\n"
	  "\t-n <type>        Give some statistics (all, num_nodes, num_edges, graph_type, symmetry, num_doubled, num_diff_doubled, links, nodes, central, cycles, show_cycles)\n"
	  "\t                   'central' statistics gives:\n"
	  "\t                     (1) the node minimizing the sum of the distance between a node and all the nodes\n"
	  "\t                     (2) the node minimizing the maximal distance\n"
	  "\t                     (3) the node maximizing the number of inner links\n"
	  "\t                     (4) the node maximizing the sum of links. This is the most stable for normal and pfnet maps\n"
	  "\t-o <type>        Output type (none, matrix, edges, arcs, human, dot, plain, gaston, subdue, jxplore, gspan,\n"
	  "\t                             subdue,{directed|undirected|weight|noedgelabel|ijlabel|edgelabel|sorted|unsorted}*\n"
	  "\t                             jxplore,{directed|undirected|weight|noedgelabel|ijlabel|edgelabel}*\n"
	  "\t                             (example: subdue,noedgelabel,undirected - default for subdue: directed,noedgelabel,unsorted)).\n"
	  "OPTIONAL:\n"
	  "\t-h               Help\n"
	  "\t-f <filename>    Input by a filename instead of standard input\n"
	  "\t-w <value>       Value of the 'weird threshold', the 'filter threshold', the 'cst value' or the 'diff type'. See -r, -c, -m or -d\n"
	  "\t-s               Use the scored DB (" SCORED_DATABASE ")\n"
	  "\t-i <filename>    Use the provided file instead of " DATABASE_NAME " (to desactivate, use \"\")\n"
	  "\t-p               Avoid the computation of the predecessor matrix (could save time a/o memory)\n"
	  "\t-t <filename>    Use the given dictionnary to translate the index of the nodes and edges, using their names. Concern only the 'dot', the 'subdue' and the 'gaston' outputs.\n"
	  "\t-y <type>        Input type (pajek, subdue, gaston, gspan, ffsm, prolog).\n"
	  "\t-v <filename>    Output filename. If not provided, output on stdout. Useful for multiple graphs DB.\n"
	  "\nExamples:\n"
	  "%s -f file.net -n all                     Give some statistics about the network\n"
	  "%s -f file.net -r less -w 100 -o edges    Return all the links below 100\n"
	  "\n", argv[0], argv[0], argv[0]);
	  exit(0);
	  break;
	case 'r':
	  // -r <link filter type>
	  if(!strcmp(optarg, "all")) filter_type = FILTER_ALL;
	  else if(!strcmp(optarg, "less")) filter_type = FILTER_LESS;
	  else if(!strcmp(optarg, "greater")) filter_type = FILTER_GREATER;
	  else if(!strcmp(optarg, "equal")) filter_type = FILTER_EQUAL;
	  else if(!strcmp(optarg, "less_equal")) filter_type = FILTER_LESS_EQUAL;
	  else if(!strcmp(optarg, "greater_equal")) filter_type = FILTER_GREATER_EQUAL;
	  else if(!strcmp(optarg, "sfl")) filter_type = FILTER_SFL;
	  else if(!strcmp(optarg, "dfl")) filter_type = FILTER_DFL;
	  else if(!strcmp(optarg, "filter3")) filter_type = FILTER_FILTER3;
	  nopt--;
	  break;
	case 'q':
	  // -q <node filter type>
	  if(!strcmp(optarg, "all")) nfilter_type = NFILTER_ALL;
	  else if(!strcmp(optarg, "unisolated")) nfilter_type = NFILTER_UNISOLATED;
	  nopt--;
	  break;
	case 'n':
	  // -n <statistic name>
	  if(!strcmp(optarg, "all")) stat_type = STAT_ALL;
	  else if(!strcmp(optarg, "num_nodes")) stat_type = STAT_NUM_NODES;
	  else if(!strcmp(optarg, "num_edges")) stat_type = STAT_NUM_EDGES;
	  else if(!strcmp(optarg, "graph_type")) stat_type = STAT_GRAPH_TYPE;
	  else if(!strcmp(optarg, "symmetry")) stat_type = STAT_SYMMETRY;
	  else if(!strcmp(optarg, "num_doubled")) stat_type = STAT_NUM_DOUBLED;
	  else if(!strcmp(optarg, "num_diff_doubled")) stat_type = STAT_NUM_DIFF_DOUBLED;
	  else if(!strcmp(optarg, "links")) stat_type = STAT_LINKS;
	  else if(!strcmp(optarg, "nodes")) stat_type = STAT_NODES;
	  else if(!strcmp(optarg, "central")) stat_type = STAT_CENTRAL;
	  else if(!strcmp(optarg, "cycles")) stat_type = STAT_CYCLES;
	  else if(!strcmp(optarg, "show_cycles")) stat_type = STAT_SHOW_CYCLES;
	  nopt--;
	  break;
	case 'c':
	  // -c <check name>
	  if(!strcmp(optarg, "all")) gP.check_type = CHECK_ALL;
	  else if(!strcmp(optarg, "recognized")) gP.check_type = CHECK_RECOGNIZED;
	  else if(!strcmp(optarg, "tot_recognized")) gP.check_type = CHECK_TOT_RECOGNIZED;
	  else if(!strcmp(optarg, "diff_field")) gP.check_type = CHECK_DIFF_FIELD;
	  else if(!strcmp(optarg, "all_dfl_weird")) gP.check_type = CHECK_ALL_DFL_WEIRD;
	  nopt--;
	  break;
	case 'o':
	  // -o <output type>
	  if(!strcmp(optarg, "none")) gP.output_type = OUTPUT_NONE;
	  else if(strstr(optarg, "matrix")!=NULL) gP.output_type = OUTPUT_MATRIX;
	  else if(strstr(optarg, "edges")!=NULL) gP.output_type = OUTPUT_EDGES;
	  else if(strstr(optarg, "arcs")!=NULL) gP.output_type = OUTPUT_ARCS;
	  else if(strstr(optarg, "human")!=NULL) gP.output_type = OUTPUT_HUMAN;
	  else if(strstr(optarg, "dot")!=NULL) gP.output_type = OUTPUT_DOT;
	  else if(strstr(optarg, "plain")!=NULL) gP.output_type = OUTPUT_PLAIN;
	  else if(strstr(optarg, "subdue")!=NULL) gP.output_type = OUTPUT_SUBDUE;
	  else if(strstr(optarg, "gaston")!=NULL) gP.output_type = OUTPUT_GASTON;
	  else if(strstr(optarg, "jxplore")!=NULL) gP.output_type = OUTPUT_JXPLORE;
	  else if(strstr(optarg, "gspan")!=NULL) gP.output_type = OUTPUT_GSPAN;
	  {
		  char buffer[1000];
		  sprintf(buffer, "%s,", optarg);
		  if(strstr(buffer, ",directed,")!=NULL) gP.output_opt_direction = OUTPUT_OPT_DIRECTED;
		  if(strstr(buffer, ",undirected,")!=NULL) gP.output_opt_direction = OUTPUT_OPT_UNDIRECTED;
		  if(strstr(buffer, ",weight,")!=NULL) gP.output_opt_label = OUTPUT_OPT_WEIGHTLABEL;
		  if(strstr(buffer, ",noedgelabel,")!=NULL) gP.output_opt_label = OUTPUT_OPT_EMPTYLABEL;
		  if(strstr(buffer, ",ijlabel,")!=NULL) gP.output_opt_label = OUTPUT_OPT_IJLABEL;
		  if(strstr(buffer, ",edgelabel,")!=NULL) gP.output_opt_label = OUTPUT_OPT_EDGELABEL;
		  if(strstr(buffer, ",sorted,")!=NULL) gP.output_opt_sorting = OUTPUT_OPT_SORTED;
		  if(strstr(buffer, ",unsorted,")!=NULL) gP.output_opt_sorting = OUTPUT_OPT_UNSORTED;
	  }
	  
	  /*else if(!strcmp(optarg, "subdue")) gP.output_type = OUTPUT_SUBDUE_NOWEIGHT;
	  else if(!strcmp(optarg, "subdue_noweight")) gP.output_type = OUTPUT_SUBDUE_NOWEIGHT;
	  else if(!strcmp(optarg, "subdue_weight")) gP.output_type = OUTPUT_SUBDUE_WEIGHT;
	  else if(!strcmp(optarg, "subdue_directed_edgename")) gP.output_type = OUTPUT_SUBDUE_DIRECTED_EDGENAME;
	  else if(!strcmp(optarg, "subdue_directed_emptyname")) gP.output_type = OUTPUT_SUBDUE_DIRECTED_EMPTYNAME;*/
	  nopt--;
	  break;
	case 'm':
	  // -m <modification type>
	  if(!strcmp(optarg, "add")) modif_type = MODIFY_ADD;
	  else if(!strcmp(optarg, "sub_from_cst")) modif_type = MODIFY_SUB_FROM_CST;
	  else if(!strcmp(optarg, "set")) modif_type = MODIFY_SET;
	  else if(!strcmp(optarg, "multiply")) modif_type = MODIFY_MULTIPLY;
	  else if(!strcmp(optarg, "compare_with")) modif_type = MODIFY_COMPARE_WITH;
	  nopt--;
	  break;
        case 'u':
          // -u <filename> (second filename for the union)
	  filename_union = strdup(optarg);
	  nopt--;
          break;
        case 'd':
          // -d <filename> (second filename for the difference)
	  filename_diff = strdup(optarg);
	  nopt--;
          break;
        case 'f':
          // -f <filename>
	  filename = strdup(optarg);
          break;
	case 'w':
	  // -w <weird threshold>
	  extra_value = atof(optarg);
	  break;
	 case 's':
	  // -s
	  load_scored_db = 1;
	  break;
	 case 'i':
	  // -i <filename>
	  free(gP.filename_ISI_DB);
	  if(strlen(optarg) == 0)
	    gP.filename_ISI_DB = NULL;
	  else
	    gP.filename_ISI_DB = strdup(optarg);
	  break;
	 case 'v':
	  // -v <filename>
	  if(gP.filename_output != NULL)
	  	free(gP.filename_output);
	  if(strlen(optarg) == 0)
	    gP.filename_output = NULL;
	  else
	    gP.filename_output = strdup(optarg);
	  break;
	 case 'p':
	  // -p
	  do_not_use_predecessors = 1;
	  break;
        case 't':
          // -t <filename> (dictionnary)
	  filename_dico = strdup(optarg);
          break;
	case 'y':
	  // -y <input type>
	  if(!strcmp(optarg, "pajek")) input_type = INPUT_PAJEK;
	  else if(!strcmp(optarg, "subdue")) input_type = INPUT_SUBDUE;
	  else if(!strcmp(optarg, "gaston")) input_type = INPUT_GASTON;
	  else if(!strcmp(optarg, "gspan")) input_type = INPUT_GSPAN;
	  else if(!strcmp(optarg, "ffsm")) input_type = INPUT_FFSM;
	  else if(!strcmp(optarg, "prolog")) input_type = INPUT_PROLOG;
	  break;
        case '?':
          break;
        default:
          printf ("Bad input character : 0%o !!\n", opt);
        }
    }
    
  if(nopt>0){
	fprintf(stderr, "\n[ERROR] - You have forgotten one mandatory option. See %s -h.\n", argv[0]);
	exit(1);
	}
	
  // Load the category database
  fprintf(stderr, "Load the category database...\n");
  load_database(&gP);
  
  // Load the scored category database
  if(load_scored_db) {
    fprintf(stderr, "Load the scored category database...\n");
	load_scored_database(&gP);
  }
  else gP.num_scored_relations=0;
  	
  // Load the dictionnary database
  
  if(filename_dico!=NULL) {
  	fprintf(stderr, "Load the dictionnary database...\n");
	load_dico_database(&gP, filename_dico);
  }
  else gP.num_dico_entries=0;
  
  // Open the graph file
  fprintf(stderr, "Open the graph file...\n");
  graph_main_db = open_generic_file(&gP, filename, input_type);
  graph_main = graph_main_db[0];
  
  // Open the second file (for union)
  if(filename_union != NULL) {
    fprintf(stderr, "Open the second file (for union)...\n");
  	graph_union_db = open_generic_file(&gP, filename_union, input_type);
	graph_union = graph_union_db[0];
  	if(graph_main->nodes()!=graph_union->nodes()){
    	fprintf(stderr, "\n[ERROR] - The union file (%s, %d nodes) must have the same number of nodes as the main file (%d nodes) !\n", filename_union, graph_union->nodes(), graph_main->nodes());
    	exit(2);
  	}
  }
  
  // Open the second file (for difference)
  if(filename_diff != NULL) {
    fprintf(stderr, "Open the second file (for difference)...\n");
	graph_diff_db = open_generic_file(&gP, filename_diff, input_type);
	graph_diff = graph_diff_db[0];
    if(graph_main->nodes()!=graph_diff->nodes()){
      fprintf(stderr, "\n[ERROR] - The difference file (%s, %d nodes) must have the same number of nodes as the main file (%d nodes) !\n", filename_diff, graph_diff->nodes(), graph_main->nodes());
      exit(2);
    }
  }
  
  gP.ending_table = NULL;
  gP.isolated_table = NULL;
       
  // Modification of the weight matrix
#ifdef MODIFICATION_MATRIX
  fprintf(stderr, "Modification of the weight matrix...\n");
  for(i=0; i<graph_main->nodes(); i++)
    for(j=0; j<graph_main->nodes(); j++){
      if(graph_main->weight(i,j) != 0) {
	    float weight = graph_main->weight(i,j);
        switch(modif_type)
        {
          case MODIFY_ADD:
            weight += extra_value;
            break;
          case MODIFY_SUB_FROM_CST:
            weight = extra_value - weight;
            break;
          case MODIFY_SET:
            weight = extra_value;
            break;
          case MODIFY_MULTIPLY:
            weight *= extra_value;
            break;
          case MODIFY_COMPARE_WITH:
            weight = (weight-extra_value)/fabs(weight-extra_value);
            break;
        }
		graph_main->set_weight(i,j,weight);
      }
	}
#endif
    
  // Compute a new graph being the difference between the main graph and the 'difference' graph
  if (filename_diff!=NULL)
  {
	
    int stable = 0;
    int ci, cj;		// The location of the edge NEW(i,j) in com* is in COM(ci,cj)
	Graph *graph_common;
	
	fprintf(stderr, "Compute a new graph being the difference between the main graph and the 'difference' graph...\n");

    // The original graph is (graph_main)
    // The graph to compare with is (graph_diff)
    // The common graph is in (graph_common)
    
    // Allocation of the graph
	graph_common = new Graph(1);
	
    // Create a common DB of labels (fusion of the labels of node_table and graph_diff->node_table)
    
    /*printf("OLD (%d):\n", graph_main->nodes());
    for(i=0;i<graph_main->nodes();i++)
      printf("%d => %s\n", i+1, graph_main->get_node(i).label);
    printf("NEW (%d):\n", graph_diff->nodes());
    for(i=0;i<graph_diff->nodes();i++)
      printf("%d => %s\n", i+1, graph_diff->get_node(i).label);*/
      
    // Compute the common 'graph_common->nodes()'
	//graph_common->nodes() = graph_main->nodes();
    //for(i=0;i<graph_diff->nodes();i++)
    //  if(find_category_in_table(graph_diff->get_node(i).label, graph_main->nodes(), graph_main)==-1) // New category found in new*
    //    graph_common->num_nodes++;

    // Creation of graph_common->node_table[]
    for(i=0;i<graph_main->nodes();i++) // From the labels of the original graph
    {
      //graph_common->node_table[i].visible = 1;
      //graph_common->get_node(i).label = strdup(graph_main->get_node(i).label);
      //graph_common->node_table[i].field = ((buffer=find_field(&gP, graph_main->get_node(i).label)) != NULL)?(strdup(buffer)):(NULL);
      //graph_common->node_table[i].line = strdup(graph_main->node_table[i].line);
      stable++;
	  graph_common->add_node(graph_main->get_node(i).label);
    }
    for(i=0;i<graph_diff->nodes();i++) // From the labels of the new graph
    {
      if(find_category_in_table(graph_diff->get_node(i).label, stable, graph_common)==-1)
      {
        //graph_common->node_table[stable].visible = 1;
        //graph_common->node_table[stable].category = strdup(graph_diff->get_node(i).label);
        //graph_common->node_table[stable].field = ((buffer=find_field(&gP, graph_diff->get_node(i).label)) != NULL)?(strdup(buffer)):(NULL);
	//graph_common->node_table[stable].line = strdup(graph_diff->node_table[i].line);
	stable++;
		graph_common->add_node(graph_diff->get_node(i).label);
      }
    }
    
    /*printf("COM (%d):\n", graph_common->nodes());
    for(i=0;i<graph_common->nodes();i++)
      printf("%d => %s\n", i+1, graph_common->get_node(i).label);*/
    
    // Creation of graph_common->weights[][]
    for(i=0; i<graph_main->nodes(); i++)
      for(j=0; j<graph_main->nodes(); j++)  // From the weights of the original graph
        if(graph_main->weight(i,j) != 0.0) {
          //graph_common->weight(i,j) = graph_main->weight(i,j);
		  float weight = graph_main->weight(i,j);
		  if(extra_value == 0)
		    weight =  IS_NUM(graph_common->weight(i,j));
		  graph_common->add_edge(i, j, weight, NULL, 1);

    }
    for(i=0; i<graph_diff->nodes(); i++){
      ci = find_category_in_table(graph_diff->get_node(i).label, graph_common->nodes(), graph_common);
      for(j=0; j<graph_diff->nodes(); j++)  // From the weights of the new graph
        if(graph_diff->weight(i,j) != 0.0){
	  cj = find_category_in_table(graph_diff->get_node(j).label, graph_common->nodes(), graph_common);
          // In the new matrix weights[i][j]:
	  // * similar links will be set to 0
	  // * added links will have a positive value
	  // * deleted links will have a negative value
	  float weight = graph_common->weight(ci, cj);
	  
	  if(extra_value == 0)
	  {
	    // With this one, we consider only the fact there is a link or not
	    // So if ORIG[i][j]=2 and NEW[i][j]=1  ==>  COM[i][j]=0  (no new link added/deleted)
	    weight -= IS_NUM(graph_diff->weight(i,j));
	  }
	  else
	  {
	    // With this technique, the weights are simply substracted
	    // So if ORIG[i][j]=2 and NEW[i][j]=1  ==>  COM[i][j]=1
	    weight -= graph_diff->weight(i,j);
	  }
	  graph_common->set_weight(ci, cj, weight);
	}
    } // END of for()

    /*printf("graph_common->weights[][]:\n");
    for(i=0; i<graph_common->nodes(); i++)
      for(j=i+1; j<graph_common->nodes(); j++)
        if(graph_common->weight(i,j)!=0.0)
	  printf("%s => %s : %f\n", graph_common->get_node(i).label, graph_common->get_node(j).label, graph_common->weight(i,j));*/

    // Replace the original graph (graph_main)
    //Libera_matriz_dinamica_float(graph_main->weights, graph_main->nodes());
    //graph_main->weights = graph_common->weights;
    //free_node_table(graph_main->nodes(), graph_main->node_table);
    //graph_main->node_table = graph_common->node_table;
    //graph_main->nodes() = graph_common->nodes();
	delete graph_main;
	graph_main = graph_common;
	
    /*// Compute the difference matrix
    for(i=0; i<graph_main->nodes(); i++)
      for(j=0; j<graph_main->nodes(); j++){
	graph_main->weight(i,j) = graph_diff->weight(i,j)-graph_main->weight(i,j);
      }*/
      
    // Compute the number of edges
    //graph_main->num_edges = get_num_edges(graph_main);
    
    // Free the memory
    //free_node_table(graph_diff->nodes(), graph_diff->node_table);
    //free_node_table(graph_common->nodes(), graph_common->node_table);
    //Libera_matriz_dinamica_float(graph_common->weights, graph_common->nodes());
    
  } // END of if (filename_diff!=NULL)
  
  // Search for the ending and the isolated nodes
#ifdef ISOLATED_NODE
  fprintf(stderr, "Search for the ending and the isolated nodes...\n");
  compute_ending(&gP, graph_main);
#endif
  
  // Filter the nodes
#ifdef FILTER_NODES
  fprintf(stderr, "Filter the nodes...\n");
  gP.num_nodes_visible = graph_main->nodes();
  for(i=0; i<graph_main->nodes(); i++)
  {
  	if((nfilter_type==NFILTER_UNISOLATED) && (is_isolated(&gP,i)))
	{
		// Remove the node #i
		graph_main->set_visibility(i, 0);
		gP.num_nodes_visible--;
	}
  }
#endif
  
  // Filter the links
#ifdef FILTER_LINKS
  fprintf(stderr, "Filter the links...\n");
  for(i=0; i<graph_main->nodes(); i++)
  {
	for(j=0; j<graph_main->nodes(); j++)
	{
	  if(!((filter_type==FILTER_ALL || filter_type==FILTER_FILTER3)
	    || ((filter_type==FILTER_LESS) && (graph_main->weight(i,j)<extra_value))
	    || ((filter_type==FILTER_GREATER) && (graph_main->weight(i,j)>extra_value))
	    || ((filter_type==FILTER_EQUAL) && (graph_main->weight(i,j)==extra_value))
	    || ((filter_type==FILTER_LESS_EQUAL) && (graph_main->weight(i,j)<=extra_value))
	    || ((filter_type==FILTER_GREATER_EQUAL) && (graph_main->weight(i,j)>=extra_value))
	    || ((filter_type==FILTER_SFL) && (is_sfl(&gP, graph_main, i,j)==1))
	    || ((filter_type==FILTER_DFL) && (is_sfl(&gP, graph_main, i,j)==0))))
	    graph_main->set_weight(i,j,0.0);
	}
  }
#endif
  
#ifdef USE_PREDECESSORS
  if(do_not_use_predecessors != 1)
  {
    // Allocation of predecessors[][][]
    gP.predecessors = (int ***) malloc(sizeof(int**)*(graph_main->nodes()+1));
    if(gP.predecessors == NULL){
  	printf("\n[ERROR] - Apertura del fichero %s incorrecta.\n", argv[1]);
	exit(3);
  	}
  
    // Reserva de memoria para las matrices de predecessors
    for(i=0;i<graph_main->nodes()+1;i++)
  	gP.predecessors[i] = Matriz_dinamica_int(graph_main->nodes(), graph_main->nodes());
  
    // Allocation of size_sp[][]
    gP.size_sp = Matriz_dinamica_int(graph_main->nodes(), graph_main->nodes());
  
    // Search for the central node, and compute some useful tables
    /*printf("CEN(1): %d\n", floyd_warshall(graph_main->weights, 1));
    printf("CEN(2): %d\n", floyd_warshall(graph_main->weights, 2));
    printf("CEN(3): %d\n", floyd_warshall(graph_main->weights, 3));
    printf("CEN(4): %d\n", floyd_warshall(graph_main->weights, 4));*/
    compute_pesos_predecessors(&gP, graph_main);
    compute_table_size_sp(&gP, graph_main);
    gP.central_node = floyd_warshall(&gP, graph_main, TYPE_CENTRAL_NODE)-1;
  }
#endif

  /*int ii=77, jj=152;
  printf("measure_cen_i(%d) = %d\n", ii, measure_cen_i(ii-1));
  printf("measure_cen_ij(%d,%d) = %d\n", ii, jj, measure_cen_ij(ii-1, jj-1));
  printf("measure_bor_i(%d) = %d\n", ii, measure_bor_i(ii-1));
  printf("measure_bor_ij(%d,%d) = %d\n", ii, jj, measure_bor_ij(ii-1, jj-1));*/
    
    
  // Filter 'filter3'
  if(filter_type==FILTER_FILTER3)
  {
    int is_sym = graph_main->is_symmetrical();
    //int is_sym = 0;			// All the edges should be pruned twice, depending of the 2 MIN-values obtained from the 2 connected nodes

    // Allocate the memory to store the minimum value for each node
    float* nodes_minvalues;
    if((nodes_minvalues=(float *)calloc(graph_main->nodes(),sizeof(float)))==NULL){
      fprintf(stderr, "\n[ERROR] - Error during the allocation of nodes_minvalues !");
      exit(3);}
    
    // Fill the nodes_minvalues[] table
    for(i=0; i<graph_main->nodes(); i++)
    {
      int sfl_n=0;
      // Compute MIN(w(l(ij))) for all l(ij) | SFL(l(ij))=1
      nodes_minvalues[i]=CK_INFINITY;
      for(j=0; j<graph_main->nodes(); j++)
      {
        if((i!=j) && (graph_main->weight(i,j) != 0.0) && is_sfl(&gP, graph_main, i, j))
	{
		// SFL
		nodes_minvalues[i] = MIN(nodes_minvalues[i], graph_main->weight(i,j));
		sfl_n++;
	}
      }
      if(sfl_n==0) nodes_minvalues[i]=0;
    }

    // Filter the links
    for(i=0; i<graph_main->nodes(); i++)
    {
      if(nodes_minvalues[i]!=0.0)
      {
		// Delete the DFL links with a weight < nodes_minvalues[i]
		for(j=0; j<graph_main->nodes(); j++)
		{
			if((i!=j) && (graph_main->weight(i,j) != 0.0) && !is_sfl(&gP, graph_main, i, j)
				&& (graph_main->weight(i,j) < nodes_minvalues[i]))
			{
				// DFL
				graph_main->set_weight(i, j, 0.0);
				if(is_sym) graph_main->set_weight(j, i, 0.0);
			}
		}
	}
    }
  } // END of if(filter_type==FILTER_FILTER3)
    
    
  // Compute the number of edges
  fprintf(stderr, "Compute the number of edges...\n");
  int num_edges = graph_main->compute_num_edges();
    
  // Print some statistics
  fprintf(stderr, "Print some statistics...\n");
  if(stat_type==STAT_NUM_NODES) printf("%d\n", graph_main->nodes());
  else if(stat_type==STAT_NUM_EDGES) printf("%d\n", num_edges);
  else if(stat_type==STAT_GRAPH_TYPE) printf("%s\n", graph_main->get_graph_type());
  else if(stat_type==STAT_SYMMETRY) printf("%d\n", graph_main->is_symmetrical());
  else if(stat_type==STAT_NUM_DOUBLED) printf("%d\n", graph_main->get_num_existing());
  else if(stat_type==STAT_NUM_DIFF_DOUBLED) printf("%d\n", graph_main->get_num_different());
  else if(stat_type==STAT_ALL)
    printf("%s: %s type, %d nodes, %d edges, symmetry=%d, %d doubled links "
     "(should be 0), %d different dbl links (must be 0)\n",
     ((filename==NULL) || !strcmp(filename,"-"))?"(stdin)":filename, graph_main->get_graph_type(), graph_main->nodes(), num_edges,
     graph_main->is_symmetrical(), graph_main->get_num_existing(), graph_main->get_num_different());
   
   if((stat_type==STAT_CENTRAL) || (stat_type==STAT_ALL))
   {
     int fw1, fw2, fw3, fw4;
     fw1 = floyd_warshall(&gP, graph_main, 1);
     fw2 = floyd_warshall(&gP, graph_main, 2);
     fw3 = floyd_warshall(&gP, graph_main, 3);
     fw4 = floyd_warshall(&gP, graph_main, 4);
     printf("central nodes:\n(1) %d '%s'\n(2) %d '%s'\n(3) %d '%s'\n(4) %d '%s'\n",
     	fw1, graph_main->node_label(fw1-1),
	fw2, graph_main->node_label(fw2-1),
	fw3, graph_main->node_label(fw3-1),
	fw4, graph_main->node_label(fw4-1));
   }
   
   if((stat_type==STAT_CYCLES) || (stat_type==STAT_SHOW_CYCLES) || (stat_type==STAT_ALL))
   {
     int sc = (stat_type==STAT_SHOW_CYCLES)?1:0;
     int cyc = loop_detector(&gP, graph_main, sc);
     if(stat_type==STAT_ALL)
       printf("cycles: %d\n", cyc);
     else
       printf("%d\n", cyc);
   }
   
  if(graph_main->is_symmetrical()==0)
  	fprintf(stderr, "\n[ERROR] This matrix is not symmetrical, so you cannot apply the following checks.\n");
  
#if LINKS_DIFFERENT_FIELDS
  // Print the links connecting two differents fields
  // Global-DFL study (for all the nodes in the same time)
  for(i=0; i<graph_main->nodes(); i++)
    for(j=i+1; j<graph_main->nodes(); j++)
    {
      if(graph_main->weight(i,j) > 0)
      {
	      if(!is_sfl(&gP, graph_main, i,j))
	      {
	         // DFL (Different Field Link)
	        if((gP.check_type==CHECK_DIFF_FIELD) || (gP.check_type==CHECK_ALL))
		fprintf(stderr, "[ERROR] Node %d (%s:%s) is linked to a diff. field : node %d (%s:%s)\n",
			i+1, graph_main->get_node(i).field, graph_main->get_node(i).label,
			j+1, graph_main->get_node(j).field, graph_main->get_node(j).label);
	        num_difffields++;
		dfl_sum += graph_main->weight(i,j);
		dfl_num ++;
		if(graph_main->weight(i,j)<=extra_value) dfl_weird_num++;
		else dfl_non_weird_num++;
	      }
	      else
	      {
	        // non-DFL (link connecting two nodes in the same field)
		non_dfl_sum += graph_main->weight(i,j);
		non_dfl_num ++;
		if(graph_main->weight(i,j)<=extra_value) non_dfl_weird_num++;
		else non_dfl_non_weird_num++;		
	      }
      }
    }
#endif

  if((gP.check_type==CHECK_ALL_DFL_WEIRD) || (gP.check_type==CHECK_ALL))
  {
        fprintf(stderr, "%d links connect two nodes with different fields (DFL) !\n", num_difffields);
	fprintf(stderr, "DF-links values: number=%d, avg=%f\n", (int)dfl_num, dfl_sum/dfl_num);
	fprintf(stderr, "Non DF-links values: number=%d, avg=%f\n", (int)non_dfl_num, non_dfl_sum/non_dfl_num);
	fprintf(stderr, "Any-links values: number=%d, avg=%f\n", (int)(dfl_num+non_dfl_num), (dfl_sum+non_dfl_sum)/(dfl_num+non_dfl_num));
	fprintf(stderr, "A link is weird if its value is below or equal to %f\n", extra_value);
	fprintf(stderr, "%d DFL and weird, %d DFL and not weird, %d non-DFL and weird, %d non-DFL and not weird\n",
		dfl_weird_num, dfl_non_weird_num, non_dfl_weird_num, non_dfl_non_weird_num);
  }


  // Statistics about local-DFL (Different Field Link for each node)
  // ...
  
  // Global statistics
  if((stat_type==STAT_LINKS) || (stat_type==STAT_ALL))
  {
    float min=CK_INFINITY;	// Global minimal link value
    float max=-CK_INFINITY;	// Global maximal link value
    float avg=0;	// Global average link value
    for(i=0; i<graph_main->nodes(); i++)
      for(j=0; j<graph_main->nodes(); j++){
        if(graph_main->weight(i,j) != 0.0)
	{
          min = MIN(min, graph_main->weight(i,j));
          max = MAX(max, graph_main->weight(i,j));
          avg += graph_main->weight(i,j);
	}
    }
    avg /= num_edges;
    printf("LINKS: min=%f max=%f avg=%f\n", min, max, avg);
  }
    
  // Local statistics, for nodes
  // Minimal, maximal and average number of links for each nodes
  if((stat_type==STAT_NODES) || (stat_type==STAT_ALL))
  {
    float n_min=CK_INFINITY;		// Minimal of "number of links for each node"
    float n_max=-CK_INFINITY;		// Maximal of "number of links for each node"
    float n_avg=0;		// Average of "number of links for each node"
    float w_min=CK_INFINITY;		// Minimal of "avg value of links for each node"
    float w_max=-CK_INFINITY;		// Maximal of "avg value of links for each node"
    float w_avg=0;		// Average of "avg value of links for each node"
    printf("node#field#cat#num#min#max#avg#sfl_min#sfl_max#sfl_avg#sfl_n#dfl_min#dfl_max#dfl_avg#dfl_n#C_WL_i#CEN_i#BOR_i\n");
    for(i=0; i<graph_main->nodes(); i++)
    {
      float min=CK_INFINITY, max=-CK_INFINITY, avg=0;		// for the value of the connected link for each node
      float sfl_min=CK_INFINITY, sfl_max=-CK_INFINITY, sfl_avg=0, sfl_n=0;	// concerning the SFL links
      float dfl_min=CK_INFINITY, dfl_max=-CK_INFINITY, dfl_avg=0, dfl_n=0;	// concerning the DFL links
      float n=0;
      for(j=0; j<graph_main->nodes(); j++)
      {
        if(graph_main->weight(i,j) != 0.0)
	{
          min = MIN(min, graph_main->weight(i,j));
          max = MAX(max, graph_main->weight(i,j));
          avg += graph_main->weight(i,j);
	  n++;
	  if(is_sfl(&gP, graph_main, i, j))
	  {
	  	// SFL
          	sfl_min = MIN(sfl_min, graph_main->weight(i,j));
          	sfl_max = MAX(sfl_max, graph_main->weight(i,j));
          	sfl_avg += graph_main->weight(i,j);
	  	sfl_n++;
	  }
	  else
	  {
	  	// DFL
          	dfl_min = MIN(dfl_min, graph_main->weight(i,j));
          	dfl_max = MAX(dfl_max, graph_main->weight(i,j));
          	dfl_avg += graph_main->weight(i,j);
	  	dfl_n++;
	  }
	}
      }
      if(n>0) avg /= n;
      else{ avg=0; min=0; max=0; }
      if(sfl_n>0) sfl_avg /= sfl_n;
      else{ sfl_avg=0; sfl_min=0; sfl_max=0; }
      if(dfl_n>0) dfl_avg /= dfl_n;
      else{ dfl_avg=0; dfl_min=0; dfl_max=0; }

      n_min = MIN(n_min, n);
      n_max = MAX(n_max, n);
      n_avg += n;
      w_min = MIN(w_min, avg);
      w_max = MAX(w_max, avg);
      w_avg += avg;
	printf("%d#%s#%s#%d#%f#%f#%f#%f#%f#%f#%f#%f#%f#%f#%f#%d#%d#%d\n",
		i+1, graph_main->get_node(i).field, graph_main->get_node(i).label,
		(int)n, min, max, avg,
		sfl_min, sfl_max, sfl_avg, sfl_n,
		dfl_min, dfl_max, dfl_avg, dfl_n,
		cwl_categ(sfl_min, sfl_max, dfl_min, dfl_max),
		measure_cen_i(&gP, i), measure_bor_i(&gP, graph_main, i));
    } // END of for()
    n_avg /= graph_main->nodes();
    w_avg /= graph_main->nodes();
    printf("NODES (number of inner links): min=%f max=%f avg=%f\n", n_min, n_max, n_avg);
    printf("NODES (avg of inner link weights): min=%f max=%f avg=%f\n", w_min, w_max, w_avg);
  } // END of if((stat_type==STAT_NODES) || (stat_type==STAT_ALL))
  
  
  // Local statistics, for links
  if((stat_type==STAT_LINKS) || (stat_type==STAT_ALL))
  {
    printf("n1_num#n1_field#n1_cat#n2_num#n2_field#n2_cat#value#is_sfl#CEN_ij#BOR_ij\n");
    for(i=0; i<graph_main->nodes(); i++)
    {
      for(j=(graph_main->is_symmetrical()?i+1:0); j<graph_main->nodes(); j++)
      {
        if(graph_main->weight(i,j) != 0.0)
	{
          printf("%d#%s#%s#%d#%s#%s#%f#%d#%d#%d\n",
	  	i+1, graph_main->get_node(i).field, graph_main->get_node(i).label,
	  	j+1, graph_main->get_node(j).field, graph_main->get_node(j).label,
		graph_main->weight(i,j), is_sfl(&gP, graph_main, i,j),
		measure_cen_ij(&gP, graph_main, i, j), measure_bor_ij(&gP, graph_main, i, j));
	}
      }
    }
  } // END of if((stat_type==STAT_LINKS) || (stat_type==STAT_ALL))
  
  
  // Output the graph
  fprintf(stderr, "Output the graph...\n");
  write_generic_file(&gP, graph_main_db);

  // Clean the memory
  if(filename != NULL) free(filename);
  if(filename_union != NULL) free(filename_union);
  if(filename_diff != NULL) free(filename_diff);

  // Clean the memory
  //if(graph_main->weights != NULL) Libera_matriz_dinamica_float(graph_main->weights, graph_main->nodes());
  
  //if(graph_main->node_table != NULL) free_node_table(graph_main->nodes(), graph_main->node_table);
  
  for(i=0;i<gP.num_categories;i++)
  {
    if(gP.field_table[i].field!=NULL) free(gP.field_table[i].field);
    if(gP.field_table[i].category!=NULL) free(gP.field_table[i].category);
  }
  if(gP.field_table != NULL) free(gP.field_table);
  
  for(i=0;i<gP.num_scored_relations;i++)
  {
    if(gP.scored_table[i].f1!=NULL) free(gP.scored_table[i].f1);
    if(gP.scored_table[i].c1!=NULL) free(gP.scored_table[i].c1);
    if(gP.scored_table[i].f2!=NULL) free(gP.scored_table[i].f2);
    if(gP.scored_table[i].c2!=NULL) free(gP.scored_table[i].c2);
  }
  if(gP.scored_table != NULL) free(gP.scored_table);
  
  if(gP.ending_table) free(gP.ending_table);
  if(gP.isolated_table) free(gP.isolated_table);
  
  if(gP.pesos!=NULL)
  {
	  for(i=0;i<graph_main->nodes()+1;i++)
		if(gP.pesos[i]!=NULL)
			Libera_matriz_dinamica_float (gP.pesos[i], graph_main->nodes());
	free (gP.pesos);
  }

  if(gP.predecessors!=NULL)
  {
	for(i=0;i<graph_main->nodes()+1;i++)
		if(gP.predecessors[i]!=NULL)
			Libera_matriz_dinamica_int (gP.predecessors[i], graph_main->nodes());
	free (gP.predecessors);
  }
  
  if(gP.size_sp!=NULL)
	Libera_matriz_dinamica_int(gP.size_sp, graph_main->nodes());
}



/***
Used to compare two elements of an edge vector.
***/

// Min value is ordered first
static int compedges_min(const void *_e1, const void *_e2)
{
  SEDGE **e1 = (SEDGE **) _e1;
  SEDGE **e2 = (SEDGE **) _e2;
  if((*e1)->w > (*e2)->w) return 1;
  else if((*e1)->w < (*e2)->w) return -1;
  else return 0;
}

// Max value is ordered first
static int compedges_max(const void *_e1, const void *_e2)
{
  SEDGE **e1 = (SEDGE **) _e1;
  SEDGE **e2 = (SEDGE **) _e2;
  if((*e2)->w > (*e1)->w) return 1;
  else if((*e2)->w < (*e1)->w) return -1;
  else return 0;
}



/***
Used to find the field of a category
***/

// Load the field-category database
// Read the file DATABASE_NAME (with less than DATABASE_SIZEMAX entries!!)
// and fill the 'gP->field_table' structure.
void load_database(GLOBAL *gP)
{
	char* filename = gP->filename_ISI_DB;
	FILE* fich;
	char cadena[1024];
	char *word;
	gP->num_categories=0;
	
	// Allocate the memory for the database
	if((gP->field_table=(SCATNODE *)calloc(DATABASE_SIZEMAX,sizeof(SCATNODE)))==NULL){
		printf("Error during the allocation of gP->field_table");
		exit(3);}
	
	if(gP->filename_ISI_DB==NULL)
	{
		//fprintf(stderr, "[WARNING] No database allocated. If wanted, use the -i option.\n");
		return;
	}
	
	// Open the database
	fich = fopen(filename,"r");
	if(fich==NULL){
		printf("\n[ERROR] - Cannot open the file %s !\n", filename);
		exit(2);
	}
	
	// Read the database
	while(1)
	{
		char field[256];
		char category[256];
		char* ret = fgets(cadena, 1024, fich);
		USE(ret);
		if(feof(fich)) break;
		if(cadena[0] == '#') continue;
		if(gP->num_categories >= DATABASE_SIZEMAX)
		{
			fprintf(stderr, "\n[ERROR] The database '%s' contain more than %d entries, as encoded in the program !\n", gP->filename_ISI_DB, DATABASE_SIZEMAX);
			exit(3);
		}
		
		word=strtok(cadena, ";\r\n\t");
		if(word!=NULL) gP->field_table[gP->num_categories].field = strdup(word);
		else gP->field_table[gP->num_categories].field = NULL;
		
		if(word==NULL)
			fprintf(stderr, "[ERROR] Cannot read properly the line (%d) '%s' of the file '%s' !\n", gP->num_categories, cadena, filename);

		if(word!=NULL) word=strtok(NULL, "\r\n\t");
		if(word!=NULL) gP->field_table[gP->num_categories].category = strdup(word);
		else gP->field_table[gP->num_categories].category = strdup("");
		
		gP->num_categories++;
	}
	fclose(fich);
}

// Return 1 if the link (i,j) is a SFL (Same Field Link)
// Return 0 if the link (i,j) is a DFL (Different Field Link)
// Use the scored database if wished by the user (option -s disabled)
int is_sfl(GLOBAL *gP, Graph *G, int i, int j)
{
	float score = find_score(gP, G->get_node(i).field, G->get_node(i).label,
			G->get_node(j).field, G->get_node(j).label);
	if((G->get_node(i).field == NULL)
	  || (G->get_node(j).field == NULL)
	  || strcmp(G->get_node(i).field, G->get_node(j).field))
	{
		// DFL (Different Field Link)
		//return 0;		// Old behavior, without score
		return (score>0);
	}
	else
	{
		// SFL = non-DFL (link connecting two nodes in the same field)
		//return 1;		// Old behavior, without score
		return (score>=0);
	}
}


/***
Functions for the scored database
***/

// Load the scored field-category database
// Read the file SCORED_DATABASE (with less than SCORED_DATABASE_SIZEMAX entries!!)
// and fill the 'scored_gP->field_table' structure.
void load_scored_database(GLOBAL *gP)
{
	char* filename = (char*)SCORED_DATABASE;
	FILE* fich;
	char cadena[1024];
	char *word;
	gP->num_scored_relations=0;
	
	// Allocate the memory for the database
	if((gP->scored_table=(SSCOREDNODE *)calloc(SCORED_DATABASE_SIZEMAX,sizeof(SSCOREDNODE)))==NULL){
		printf("Error during the allocation of scored_table");
		exit(3);}
	
	// Open the database
	fich = fopen(filename,"r");
	if(fich==NULL){
		printf("\n[ERROR] - Cannot open the file %s !\n", filename);
		exit(2);
	}
	
	// Read the database
	while(1)
	{
		char* ret = fgets(cadena, 1024, fich);
		USE(ret);
		
		if(feof(fich)) break;
		if(cadena[0] == '#') continue;
		if(gP->num_scored_relations >= SCORED_DATABASE_SIZEMAX)
		{
			fprintf(stderr, "\n[ERROR] The database '%s' contain more than %d entries, as encoded in the program !\n", filename, SCORED_DATABASE_SIZEMAX);
			exit(3);
		}
		word=strtok(cadena, ";:\n\t");
		if(word!=NULL) gP->scored_table[gP->num_scored_relations].score = atof(word);
		else gP->scored_table[gP->num_scored_relations].score = 0;
		if(word!=NULL) word=strtok(NULL, ";:\n\t");
		if(word!=NULL) gP->scored_table[gP->num_scored_relations].f1 = strdup(word);
		else gP->scored_table[gP->num_scored_relations].f1 = NULL;
		if(word!=NULL) word=strtok(NULL, ";:\n\t");
		if(word!=NULL) gP->scored_table[gP->num_scored_relations].c1 = strdup(word);
		else gP->scored_table[gP->num_scored_relations].c1 = NULL;
		if(word!=NULL) word=strtok(NULL, ";:\n\t");
		if(word!=NULL) gP->scored_table[gP->num_scored_relations].f2 = strdup(word);
		else gP->scored_table[gP->num_scored_relations].f2 = NULL;
		if(word!=NULL) word=strtok(NULL, ";:\n\t");
		if(word!=NULL) gP->scored_table[gP->num_scored_relations].c2 = strdup(word);
		else gP->scored_table[gP->num_scored_relations].c2 = NULL;
		
		if(word==NULL)
			fprintf(stderr, "[ERROR] Cannot read properly the line (%d) '%s' of the file '%s' !\n", gP->num_scored_relations, cadena, filename);
		if(find_category(gP, gP->scored_table[gP->num_scored_relations].f1)==NULL)
			fprintf(stderr, "[ERROR] The field '%s' in the file '%s' is not recognized by the ISI database !\n", gP->scored_table[gP->num_scored_relations].f1, filename);
		if(find_field(gP, gP->scored_table[gP->num_scored_relations].c1)==NULL)
			fprintf(stderr, "[ERROR] The category '%s' in the file '%s' is not recognized by the ISI database !\n", gP->scored_table[gP->num_scored_relations].c1, filename);
		if(find_category(gP, gP->scored_table[gP->num_scored_relations].f2)==NULL)
			fprintf(stderr, "[ERROR] The field '%s' in the file '%s' is not recognized by the ISI database !\n", gP->scored_table[gP->num_scored_relations].f2, filename);
		if(find_field(gP, gP->scored_table[gP->num_scored_relations].c2)==NULL)
			fprintf(stderr, "[ERROR] The category '%s' in the file '%s' is not recognized by the ISI database !\n", gP->scored_table[gP->num_scored_relations].c2, filename);

		gP->num_scored_relations++;
	}
	fclose(fich);
}

// Find the score given by the expert to the relation
// between f1:c1 and f2:c2
float find_score(GLOBAL *gP, char* f1, char* c1, char* f2, char *c2)
{
	int i;
	if(!f1 || !c1 || !f2 || !c2)
		return 0;
	for(i=0;i<gP->num_scored_relations;i++)
	{
		if((!strcmp(f1, gP->scored_table[i].f1) && !strcmp(c1, gP->scored_table[i].c1)
                    && !strcmp(f2, gP->scored_table[i].f2) && !strcmp(c2, gP->scored_table[i].c2))
                  || (!strcmp(f1, gP->scored_table[i].f2) && !strcmp(c1, gP->scored_table[i].c2)
                      && !strcmp(f2, gP->scored_table[i].f1) && !strcmp(c2, gP->scored_table[i].c1)))
			return gP->scored_table[i].score;
	}
	return 0;
}

// Return the C_{WL}_i category (see the research report #2).
// S1: m_SFL, S2: M_SFL, D1: m_DFL, D2: M_DFL
int cwl_categ(float S1, float S2, float D1, float D2)
{
	if(S1==S2 && S1==D1 && S1==D2 && S1==0) return 7;
	else if (D2<S1) return 1;
	else if (D1<S1 && S1<=D2 && D2<S2) return 2;
	else if (D1<S1 && S2<=D2) return 3;
	else if (S1<=D1 && D2<S2) return 4;
	else if ((S1<=D1) && (D1<=S2) && (S2<=D2)) return 5;
	else if (S2<D1) return 6;
	else return 0;		// Unknown (should never appear)
}


// Compute the pesos[][][] and the predecessors[][][] matrices
void compute_pesos_predecessors(GLOBAL *gP, Graph *G)
{
  int i, j, k;
  
  // Allocation of pesos[][][]
  gP->pesos = (float ***) malloc(sizeof(float**)*(G->nodes()+1));
  if(gP->pesos ==NULL){
  	printf("\n[ERROR] - Cannot allocate the memory for pesos[][][].\n");
	exit(3);
  	}

  //reserva de memoria para las matrices de pesos, distancias y pfnet
  for(i=0;i<G->nodes()+1;i++)
  	gP->pesos[i] = Matriz_dinamica_float(G->nodes(), G->nodes());

  // Initialisation of pesos[][]
  for(i=0; i<G->nodes(); i++)		//Inicializamos la matriz de pesos
  	for(j=0; j<G->nodes(); j++){
		if(G->weight(i,j) == 0)
			gP->pesos[0][i][j] = CK_INFINITY;
		else
			gP->pesos[0][i][j] = G->weight(i,j);
		}
 
   // Initialisation of predecessors
  for(k=0;k<=G->nodes();k++)
    for(i=0; i<G->nodes(); i++)		//Inicializamos la matriz de predecessors
  	for(j=0; j<G->nodes(); j++){
		if(i==j || gP->pesos[0][i][j]==CK_INFINITY)
			gP->predecessors[k][i][j] = (int)CK_INFINITY;
		else
			gP->predecessors[k][i][j] = i;
	}

  for(k=1;k<=G->nodes();k++)
    for(i=0;i<G->nodes();i++)
    {
      // Start at (i+1) if the matrix is symmetrical, or 0 if not.
      for(j=0;j<G->nodes();j++)
      {
	// Original Floyd-Warshall
	//gP->pesos[k][i][j] = MIN( gP->pesos[k-1][i][j] , (gP->pesos[k-1][i][k-1]+gP->pesos[k-1][k-1][j]) );
	if(gP->pesos[k-1][i][j] <= (gP->pesos[k-1][i][k-1] + gP->pesos[k-1][k-1][j]))
	{
		gP->pesos[k][i][j] = gP->pesos[k-1][i][j];
		gP->predecessors[k][i][j] = gP->predecessors[k-1][i][j];
	}
	else
	{
		gP->pesos[k][i][j] = (gP->pesos[k-1][i][k-1] + gP->pesos[k-1][k-1][j]);
		gP->predecessors[k][i][j] = gP->predecessors[k-1][k-1][j];
	}
      }
      gP->pesos[k][i][i] = 0;
      gP->predecessors[k][i][i] = 0;
    }
    
    /*for(i=0;i<G->nodes();i++)
    for(j=0;j<G->nodes();j++)
    {
      printf("%d %d %d\n", i, j, gP->predecessors[G->nodes()][i][j]);
    }*/
}


// Run the Floyd-Warshll algorithm to compute the distances between
// each node, in order to find the central node (many definitions)
// Type should be a number between 1 and 4
// (1) the node minimizing the sum of the distance between a node and all the nodes
// (2) the node minimizing the maximal distance
// (3) the node maximizing the number of inner links
// (4) the node maximizing the sum of links  <== the most stable for normal and pfnet maps (node 30 or 7)
// The returned node is 1-based !
int floyd_warshall(GLOBAL *gP, Graph *G, int type)
{
  int i, j, k, n;
  float wsum, bestsum, wmax, bestmax;
  float wnum, bestnum, wtot, besttot;
  int bestn1, bestn2, bestn3, bestn4;
  
  // Compute bestn1 and bestn2  
  bestn1 = bestn2 = 0;
  bestsum = bestmax = CK_INFINITY;
  k=G->nodes();
  for(i=0;i<k;i++)
  {
    wsum = wmax = 0;
    n = 0;
    for(j=0;j<G->nodes();j++)
    {
      if((gP->pesos[k][i][j]!=CK_INFINITY) && (gP->pesos[k][i][j]!=0.0))
      {
        wsum += gP->pesos[k][i][j];
	wmax = MAX(wmax, gP->pesos[k][i][j]);
	n++;
      }
    }
    if(n>0)
    {
	    if(bestsum>wsum) { bestsum=wsum; bestn1=i+1; }
	    if(bestmax>wmax) { bestmax=wmax; bestn2=i+1; }
    }
  }
  
  // Compute bestn3 and bestn4
  bestn3 = bestn4 = 0;
  bestnum = besttot = 0;
  for(i=0;i<G->nodes();i++)
  {
    wnum = wtot = 0;
    n = 0;
    for(j=0;j<G->nodes();j++)
    {
      if((G->weight(i,j)!=CK_INFINITY) && (G->weight(i,j)!=0.0))
      {
	wnum++;
	wtot += G->weight(i,j);
	n++;
      }
    }
    if(n>0)
    {
	    if(bestnum<wnum) { bestnum=wnum; bestn3=i+1; }
	    if(besttot<wtot) { besttot=wtot; bestn4=i+1; }
    }
  }
 
  /*printf("(1) bestn1=%d bestsum=%f\n", bestn1, bestsum);
  printf("(2) bestn2=%d bestmax=%f\n", bestn2, bestmax);
  printf("(3) bestn3=%d bestnum=%f\n", bestn3, bestnum);
  printf("(4) bestn4=%d besttot=%f\n", bestn4, besttot);*/
  
  switch(type)
  {
    case 1: return bestn1;
    case 2: return bestn2;
    case 3: return bestn3;
    case 4: return bestn4;
    default: return -1;		// Should never arise
  }
}

// Compute the table of the ending nodes
// => ending_table[i] is 1 if the node #i is a leaf of the graph (but not isolated)
// Compute the table of the isolated nodes
// => isolated_table[i] is 1 if the node #i is isolated
void compute_ending(GLOBAL *gP, Graph *G)
{
  int i, j, n;
  
  // Allocation of ending_table[]
  if((gP->ending_table=(int *)calloc(G->nodes(),sizeof(int)))==NULL){
       fprintf(stderr, "\n[ERROR] - Error during the allocation of ending_table !");
       exit(3);}
       
  // Allocation of isolated_table[]
  if((gP->isolated_table=(int *)calloc(G->nodes(),sizeof(int)))==NULL){
       fprintf(stderr, "\n[ERROR] - Error during the allocation of isolated_table !");
       exit(3);}
      
  for(i=0;i<G->nodes();i++)
  {
    n = 0;
    for(j=0;j<G->nodes();j++)
    {
      if((G->weight(i,j)!=CK_INFINITY) && (G->weight(i,j)!=0.0))
	n++;
    }
    //printf("ENDING = %d (n=%d)\n", i+1, n);
    gP->ending_table[i]=(n==1);
    gP->isolated_table[i]=(n==0);
    /*if(n==1)
    {
      //printf("ENDING = %d (n=%d)\n", i+1, n);
      ending_table[i]=1;
    }*/
  }
}

// Compute the table of the size of the shortest path between two nodes.
void compute_table_size_sp(GLOBAL *gP, Graph *G)
{
  int i, j, s, n;

  // Computation of size_sp[][]
  for(i=0;i<G->nodes();i++)
  for(j=0;j<G->nodes();j++)
  {
    s=j;
    n=0;
    while(s!=i)
    {
      s=gP->predecessors[G->nodes()][i][s];
      if(s==CK_INFINITY) { n=(int)CK_INFINITY; break; }
      if(n>G->nodes()) { n=(int)CK_INFINITY; break; }
      n++;
    }
    gP->size_sp[i][j]=n;
  }
}

// Return 1 if the node is an ending node (but not isolated)
// The argument is 0-based !
int is_ending(GLOBAL *gP, int n)
{
  return gP->ending_table[n];
}

// Return 1 if the node is an isolated node
// The argument is 0-based !
int is_isolated(GLOBAL *gP, int n)
{
  return gP->isolated_table[n];
}

// Compute the size of the shortest path between two nodes.
// The arguments are 0-based !
int size_shortestpath(GLOBAL *gP, int i, int j)
{
  return gP->size_sp[i][j];
}

// Compute the size of the shortest path between a node and a link (l1,l2).
// The arguments are 0-based !
int size_shortestpath_link(GLOBAL *gP, Graph *G, int i, int l1, int l2)
{
  if(G->weight(l1,l2)==0) return (int)CK_INFINITY;
  else return MIN(gP->size_sp[i][l1], gP->size_sp[i][l2]);
}

// Measure CEN(i) (node-based)
// The arguments are 0-based !
int measure_cen_i(GLOBAL *gP, int i)
{
  return size_shortestpath(gP, i, gP->central_node);
}

// Measure CEN(ij) (link-based)
// The arguments are 0-based !
int measure_cen_ij(GLOBAL *gP, Graph *G, int i, int j)
{
  return size_shortestpath_link(gP, G, gP->central_node, i, j);
}

// Measure BOR(i) (node-based)
// The arguments are 0-based !
int measure_bor_i(GLOBAL *gP, Graph *G, int i)
{
  int n, s, min=(int)CK_INFINITY;
  for(n=0;n<G->nodes();n++)
  {
    if(gP->ending_table[n])
    {
      s = size_shortestpath(gP, n, i);
      min = MIN(min, s);
    }
  }
  return min;
}

// Measure BOR(ij) (link-based)
// The arguments are 0-based !
int measure_bor_ij(GLOBAL *gP, Graph *G, int i, int j)
{
  int n, s, min=(int)CK_INFINITY;
  for(n=0;n<G->nodes();n++)
  {
    if(gP->ending_table[n])
    {
      s = size_shortestpath_link(gP, G, n, i, j);
      min = MIN(min, s);
    }
  }
  return min;
}


/***
Algorithm: loop/cycle detector

  Put all the nodes in White (W)
  Put one node (any) in Grey (G)
  While a grey node S exists, do
    For each link (S,S'), do
      If Color(S')=G
        PRINT("Problem with S")
        Color(S')=B
      Else If Color(S')=W
        Color(S')=G
      End If
    End For
    Color(S)=B
  End While
  
Return the minimum number of loop detected (that is NOT the real number of cycles, but depend on
how they are crossed by the algorithm. If 0, there is not loop in the graph.
***/

int loop_detector(GLOBAL *gP, Graph *G, int _show_cycles)
{
  int i, j, gr, num_grey=1, num_black=0, cycle=0;
  char* color = Vector_char(G->nodes());
  for(i=0;i<G->nodes();i++)
    color[i]='w';
  color[0]='g';
  while(num_grey>0)
  {
    // Look for a grey node
    for(i=0;i<G->nodes();i++)
      if(color[i]=='g')
        break;
    gr=i;
    // Look for each edge (S,S')
    for(i=0;i<G->nodes();i++)
    {
      if((G->weight(gr,i)!=CK_INFINITY) && (G->weight(gr,i)!=0.0))
      {
        // We found a link (S,S') = (gr,i)
        if(color[i]=='g')
        {
          // A cycle has been detected between 'gr' and 'i'
          cycle++;
          if(_show_cycles==1)
          {
            char *s1, *s2;
            printf("Cycle %d: '%s' (%d) - '%s' (%d)\n", cycle, s1=str_replace(G->node_label(gr), " ", (char*)"_"), translated_dico_index(gP, G, gr), s2=str_replace(G->get_node(i).label, " ", (char*)"_"), translated_dico_index(gP, G, i));
            free(s1);
            free(s2);
          }
          color[i]='b';
          num_black++;
          num_grey--;
        }
        else if(color[i]=='w')
        {
          color[i]='g';
          num_grey++;
        }
      }
    }
    color[gr]='b';
    num_grey--;
  }
  Libera_vector_char(color);
  return cycle;
}




/***
Functions for the dictionnary
***/

// Load the dictionnary
// Read the given file (with less than DICO_DATABASE_SIZEMAX entries!!)
// and fill the 'gP->dico_table' structure.
// FORMAT:
// index;label
// EXAMPLE:
// 0;spain
// 1;italy
// ...
void load_dico_database(GLOBAL *gP, char* filename)
{
	FILE* fich;
	char cadena[1024];
	char *word;
	char* seps = (char*)";\n\t";
	gP->num_dico_entries=0;

	// Allocate the memory for the database
	if((gP->dico_table=(SDICONODE *)calloc(DICO_DATABASE_SIZEMAX,sizeof(SDICONODE)))==NULL){
		printf("Error during the allocation of gP->dico_table");
		exit(3);}
	
	// Open the database
	fich = fopen(filename,"r");
	if(fich==NULL){
		printf("\n[ERROR] - Cannot open the file %s !\n", filename);
		exit(2);
	}
	
	// Read the database
	while(1)
	{
		char* ret = fgets(cadena, 1024, fich);
		USE(ret);
		
		if(feof(fich)) break;
		if(cadena[0] == '#') continue;
		if(gP->num_dico_entries >= DICO_DATABASE_SIZEMAX)
		{
			fprintf(stderr, "\n[ERROR] The database '%s' contain more than %d entries, as encoded in the program !\n", filename, DICO_DATABASE_SIZEMAX);
			exit(3);
		}
		word=strtok(cadena, seps);
		if(word!=NULL) gP->dico_table[gP->num_dico_entries].index = atoi(word);
		else gP->dico_table[gP->num_dico_entries].index = -1;
		if(word!=NULL) word=strtok(NULL, seps);
		if(word!=NULL) {
			if(word[0] == '"')	// Remove first and ending '"'
				word++;
			if(word[strlen(word)-1] == '"')
				word[strlen(word)-1] = 0;
			gP->dico_table[gP->num_dico_entries].name = strdup(word);
			//printf("#%s#\n", word);
		}
		else gP->dico_table[gP->num_dico_entries].name = strdup("(null)");
		
		if(word==NULL)
			fprintf(stderr, "[ERROR] Cannot read properly the line (%d) '%s' of the file '%s' !\n", gP->num_dico_entries, cadena, filename);

		gP->num_dico_entries++;
	}
	fclose(fich);
}

// Generic function to open an input file.
// Manage the type and the stdin feature.
vector<Graph*> open_generic_file(GLOBAL *gP, char* filename, int type) {
  FILE *fin;
  
  if ((filename==NULL) || !strcmp(filename,"-"))
      fin=stdin;
  else
  {
	  fin = fopen(filename,"r");
	  if(fin==NULL){
	  	fprintf(stderr, "\n[ERROR] - Cannot open the file %s !\n", filename);
		exit(2);
	  	}
  }
  
  if(type == INPUT_PAJEK) {
  	return open_pajek_file(gP, fin);
  } else if (type == INPUT_SUBDUE) {
  	fclose(fin);
  	return open_subdue_file(gP, filename);
  } else if (type == INPUT_GASTON) {
  	return open_gaston_file(gP, fin);
  } else if (type == INPUT_FFSM) {
  	return open_ffsm_file(gP, fin);
  } else if (type == INPUT_GSPAN) {
  	fclose(fin);
  	return open_gspan_file(gP, filename);
  } else if (type == INPUT_PROLOG) {
  	return open_prolog_file(gP, fin);
  }
}

// Generic function to write in an output file.
// Manage the type and the stdout feature.
void write_generic_file(GLOBAL *gP, vector<Graph*> graph_db) {

  if((gP->output_type==OUTPUT_MATRIX) || (gP->output_type==OUTPUT_EDGES) || (gP->output_type==OUTPUT_ARCS)) {
	write_pajek_file(gP, graph_db);
  } else if(gP->output_type==OUTPUT_HUMAN) {
  	write_human_file(gP, graph_db);
  } else if(gP->output_type==OUTPUT_DOT) {
  	write_dot_file(gP, graph_db);
  } else if(gP->output_type==OUTPUT_PLAIN) {
  	write_dot_file(gP, graph_db);
  } else if(gP->output_type==OUTPUT_SUBDUE) {
  	write_subdue_file(gP, graph_db);
  } else if(gP->output_type==OUTPUT_GASTON) {
  	write_gaston_file(gP, graph_db);
  } else if(gP->output_type==OUTPUT_JXPLORE) {
  	write_jxplore_file(gP, graph_db);
  } else if(gP->output_type==OUTPUT_GSPAN) {
  	write_gspan_file(gP, graph_db);
  }
  

}


int main(int argc, char *argv[]){
  int i, j, r;

  Input(argc,argv);
  
  return 0;
}
