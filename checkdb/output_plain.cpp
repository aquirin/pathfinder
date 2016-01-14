#include "output_plain.h"

/**************************************************\
Write a graph DB in the Plain format.
Notes:
- Write only the first graph of the DB.
\*************************************************/
void write_plain_file(GLOBAL* gP, vector<Graph*> DB) {

    int i, j;
	Graph* G = DB[0];
	
	FILE *fout;
	if(gP->filename_output == NULL)
		fout = stdout;
	else {
		fout = fopen(gP->filename_output, "wb");
	}
	
	// Print in the Graphviz input PLAIN format
    fprintf(fout, "graph 0.000 0.000 0.000\n");
    // Print all the labels
    for(i=0; i<G->nodes(); i++)
    {
		if (G->get_node(i).visible)
		{
		  // Use the category name for the label of each node
		  fprintf(fout, "node %d  %f %f 1.000 1.000 \"%s\" filled circle white gold2\n",
			  translated_dico_index(gP, G, i), G->get_node(i).posx, G->get_node(i).posy, G->node_label(i));
		}
    }
    // Print all the edges
    for(i=0; i<G->nodes(); i++)
    	for(j=0; j<i; j++)
    	    if (((G->weight(i,j)!=0.0) || (G->weight(j,i)!=0.0)) && (G->get_node(i).visible) && (G->get_node(j).visible))
	    {
	      // Using the weights for labels
		  fprintf(fout, "edge %d %d 4 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 \"%f\" 0.000 0.000 solid black\n",
			  translated_dico_index(gP, G, i), translated_dico_index(gP, G, j), G->weight(i,j));
	    }
    fprintf(fout, "stop\n");
	
  if(gP->filename_output != NULL)
  	fclose(fout);
	
}
