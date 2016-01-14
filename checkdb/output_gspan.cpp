#include "output_gspan.h"

/**************************************************\
Write a graph DB in the gSpan format.
Notes:
- Manage properly multiple graphs in the DB.
- Work only if the graph stored has numeric labels
\*************************************************/
void write_gspan_file(GLOBAL* gP, vector<Graph*> DB) {

    int i, j;
	char *s1;	// Help for the output
	int limit;	// Help for the output
	
	// Open the output file
	FILE *fout;	
	if(gP->filename_output == NULL) {
		// Open stdout
		fout = stdout;
	}
	else {
		// Use the given filename
		fout = fopen(gP->filename_output, "wb");
	}
	
	for(int ng=0; ng<DB.size(); ng++) {
	
		Graph* G = DB[ng];
	
		// Print the graph header
		fprintf(fout, "c graph%d.g %d 1\n", ng, ng);
		fprintf(fout, "t # %d\n", ng);
	
	    // Print all the nodes
	    for(i=0; i<G->nodes(); i++)
	    {
			if (G->get_node(i).visible)
			{
				// Use the category name for the label of each node
				fprintf(fout, "v %d %d\n", translated_dico_index(gP, G, i)-1, atoi(G->node_label(i)));
			}
		}

		for(i=0; i<G->nodes(); i++)
		{
			limit = i;
			for(j=0; j<limit; j++)
			{
				if (((G->weight(i,j)!=0.0) || (G->weight(j,i)!=0.0)) && (G->get_node(i).visible) && (G->get_node(j).visible))
				{

					// Use the indices for the label of each node
					fprintf(fout, "%c %d %d %d\n",'e', translated_dico_index(gP, G, i)-1,translated_dico_index(gP, G, j)-1, atoi(G->edge_label(i, j)));

				}
			}
		}
		
	} // end for(ng)
	
	if(gP->filename_output != NULL)
		fclose(fout);
	
}



