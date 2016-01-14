#include "output_human.h"

/**************************************************\
Write a graph DB in the Human format.
Notes:
- Write only the first graph of the DB.
\*************************************************/
void write_human_file(GLOBAL* gP, vector<Graph*> DB) {

	int i, j;
	Graph* G = DB[0];
	
	FILE *fout;
	if(gP->filename_output == NULL)
		fout = stdout;
	else {
		fout = fopen(gP->filename_output, "wb");
	}
	
    // Print in a human-readable format
    fprintf(fout, "Network with %d nodes and %d edges (%s):\n", gP->num_nodes_visible, G->compute_num_edges(), G->is_symmetrical()?"symmetrical":"non symmetrical");
    fprintf(fout, "ADDED LINKS:\n");
    for(i=0; i<G->nodes(); i++)
    	for(j=(G->is_symmetrical()?i+1:0); j<G->nodes(); j++)
            if ((G->weight(i,j)>0) && (G->get_node(i).visible) && (G->get_node(j).visible))
              fprintf(fout, "Node %d (%s:%s) is linked to node %d (%s:%s), weight: %f\n",
			i+1, G->get_node(i).field, G->node_label(i),
			j+1, G->get_node(j).field, G->node_label(j),
			G->weight(i,j));
    // Removed links
    fprintf(fout, "REMOVED LINKS:\n");
    for(i=0; i<G->nodes(); i++)
    	for(j=(G->is_symmetrical()?i+1:0); j<G->nodes(); j++)
            if ((G->weight(i,j)<0) && (G->get_node(i).visible) && (G->get_node(j).visible))
              fprintf(fout, "Node %d (%s:%s) is linked to node %d (%s:%s), weight: %f\n",
			i+1, G->get_node(i).field, G->node_label(i),
			j+1, G->get_node(j).field, G->node_label(j),
			G->weight(i,j));
			
  if(gP->filename_output != NULL)
  	fclose(fout);
			
}
