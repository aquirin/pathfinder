#include "output_pajek.h"

/**************************************************\
Write a graph DB in the Pajek format.
Notes:
- Manage properly multiple graphs in the DB.
\*************************************************/
void write_pajek_file(GLOBAL* gP, vector<Graph*> DB) {

  int i, j;

	if((gP->filename_output == NULL) && (DB.size()>1)) {
	  	fprintf(stderr, "\n[ERROR] - Cannot write multiple graphs on stdout with the Pajek format! Solutions:\n");
		fprintf(stderr, "\t\t - Change the output format.\n");
		fprintf(stderr, "\t\t - Specify an output file.\n");
		fprintf(stderr, "\t\t - Provide a single graph database.\n");
		exit(2);
	}
	
	for(int ng=0; ng<DB.size(); ng++) {
	
	FILE *fout;	
	if(gP->filename_output == NULL) {
		// Open stdout
		fout = stdout;
	}
	else {
		if(DB.size()==1) {
			// Use the given filename
			fout = fopen(gP->filename_output, "wb");
		} else {
			// Multiple graphs: use extensions
			char name[1024];
			sprintf(name, "%s.%d", gP->filename_output, ng+1);
			fout = fopen(name, "wb");
		}
	}
	
	Graph* G = DB[ng];
	
	// Header
    fprintf(fout, ENTRADA_1, G->nodes());
	
	// Nodes
    for (i=0; i<G->nodes();i++){
      if(G->get_node(i).visible)
        //fprintf(fout, "%s", G->get_node(i).line);
	fprintf(fout, "%d \"%s\"\n", i+1, G->node_label(i));
    }
	
  // Output the links
  switch(gP->output_type)
  {
  case OUTPUT_MATRIX:
    // Print the "*matrix" matrix
    fprintf(fout, "*matrix\n");
    for(i=0; i<G->nodes(); i++)
    {
    	for(j=0; j<G->nodes(); j++)
	{
	    if ((G->get_node(i).visible) && (G->get_node(j).visible))
	      fprintf(fout, "%.10f ",G->weight(i,j));
	}
        fprintf(fout, "\n");
    }
  break;
  case OUTPUT_EDGES:
    // Print the "*edges" matrix
    fprintf(fout, "*edges\n");
    for(i=0; i<G->nodes(); i++)
    	for(j=i+1; j<G->nodes(); j++)
    	    if ((G->weight(i,j)!=0.0) && (G->get_node(i).visible) && (G->get_node(j).visible))
  	      fprintf(fout, "%d %d %.10f\n",i+1,j+1,G->weight(i,j));
  break;
  case OUTPUT_ARCS:
    // Print the "*arcs" matrix
    fprintf(fout, "*arcs\n");
    for(i=0; i<G->nodes(); i++)
    	for(j=0; j<G->nodes(); j++)
    	    if ((G->weight(i,j)!=0.0) && (G->get_node(i).visible) && (G->get_node(j).visible))
  	      fprintf(fout, "%d %d %.10f\n",i+1,j+1,G->weight(i,j));
  break;
  }
  
  if(gP->filename_output != NULL)
  	fclose(fout);
  }
	
}
