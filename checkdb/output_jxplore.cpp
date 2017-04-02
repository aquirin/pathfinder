#include "output_jxplore.h"

/**************************************************\
Write a graph DB in the JXplore format.
This format allow the visualization of the DB using
a Java application.
Notes:
- Manage properly multiple graphs in the DB.
\*************************************************/
void write_jxplore_file(GLOBAL* gP, vector<Graph*> DB) {

    int i, j;
	char *s1;	// Help for the output
	int limit1, limit2;	// Help for the output
	
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
	
	// Print the header
	fprintf(fout, "size=%ld\n", DB.size());
	fprintf(fout, "BEGIN_LIST\n");
	

	for(unsigned int ng=0; ng<DB.size(); ng++) {
	
		Graph* G = DB[ng];
	
		// Print the object header
		fprintf(fout, "BEGIN_OBJECT\n");
	
		// Print some properties
		fprintf(fout, "objective_vect_size=2\n");
		fprintf(fout, "objective_vect_name_1=node_size\n");
		fprintf(fout, "objective_vect_name_2=edge_size\n");
		fprintf(fout, "objective_vect_value_1=%d\n", G->nodes());
		fprintf(fout, "objective_vect_value_2=%d\n", G->edges());
	
		fprintf(fout, "BEGIN_STRUCT\n");

		//G->print();
  
	    // Print all the nodes
	    for(i=0; i<G->nodes(); i++)
	    {
			if (G->get_node(i).visible)
			{
				// Use the category name for the label of each node
				fprintf(fout, "v %d \"%s\"\n", translated_dico_index(gP, G, i), s1=str_replace(G->node_label(i), " ", "\\n"));
				free(s1);
			}
		}

		for(i=0; i<G->nodes(); i++)
		{
			if(gP->output_opt_direction==OUTPUT_OPT_DIRECTED) {
				limit1 = 0;
				limit2 = G->nodes();
			}
			else {
				limit1 = i+1;
				limit2 = G->nodes();
			}
			for(j=limit1; j<limit2; j++)
			{
				bool flag = false;
				if(gP->output_opt_direction==OUTPUT_OPT_DIRECTED)
					flag = (G->weight(i,j)!=0.0);
				else
					flag = ((G->weight(i,j)!=0.0) || (G->weight(j,i)!=0.0));
				if (flag && (G->get_node(i).visible) && (G->get_node(j).visible))
				{
					if(gP->output_opt_label==OUTPUT_OPT_EMPTYLABEL)
					{
						// Use nothing for the label of each node
						// Better to extract substructure, because each weight could be different for each graph
						fprintf(fout, "%c %d %d \"\"\n",((gP->output_opt_direction==OUTPUT_OPT_DIRECTED)?'d':'u'), translated_dico_index(gP, G, i),translated_dico_index(gP, G, j));
					}
					else if(gP->output_opt_label==OUTPUT_OPT_WEIGHTLABEL)
					{
						// Use the weight for the label of each node
						fprintf(fout, "%c %d %d \"%f\"\n",((gP->output_opt_direction==OUTPUT_OPT_DIRECTED)?'d':'u'),translated_dico_index(gP, G, i),translated_dico_index(gP, G, j), G->weight(i,j));
					}
					else if(gP->output_opt_label==OUTPUT_OPT_IJLABEL)
					{
						// Use the indices for the label of each node
						fprintf(fout, "%c %d %d \"%d-%d\"\n",((gP->output_opt_direction==OUTPUT_OPT_DIRECTED)?'d':'u'), translated_dico_index(gP, G, i),translated_dico_index(gP, G, j), translated_dico_index(gP, G, i),translated_dico_index(gP, G, j));
					}
					else if(gP->output_opt_label==OUTPUT_OPT_EDGELABEL)
					{
						// Use the original values for the label of each node
						fprintf(fout, "%c %d %d \"%s\"\n",((gP->output_opt_direction==OUTPUT_OPT_DIRECTED)?'d':'u'), translated_dico_index(gP, G, i),translated_dico_index(gP, G, j), G->edge_label(i,j));
						//printf("EDGE %d %d %s\n", i, j, G->edge_label(i,j));
						//fprintf(fout, "%c %d %d \"\"\n",((gP->output_opt_direction==OUTPUT_OPT_DIRECTED)?'d':'u'), translated_dico_index(gP, G, i),translated_dico_index(gP, G, j));
					}
				}
			}
		}

		fprintf(fout, "END_STRUCT\n");
	
		// Print the object footer
		fprintf(fout, "END_OBJECT\n");
	
	} // end for(ng)
	
	// Print the footer
	fprintf(fout, "END_LIST\n");
	
	if(gP->filename_output != NULL)
		fclose(fout);
	
}



