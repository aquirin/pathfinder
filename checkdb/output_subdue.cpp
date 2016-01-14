#include "output_subdue.h"

/**************************************************\
Write a graph DB in the Subdue format.
Notes:
- Manage properly multiple graphs in the DB.
- Does not keep the order of the graphs (output the
  positive graphs then the negative graphs).
\*************************************************/
void write_subdue_file(GLOBAL* gP, vector<Graph*> DB) {

	int i, j;
	char *s1;	// Help for the output
	int limit1, limit2;	// Help for the output

	for(int ng=0; ng<DB.size(); ng++) {

		FILE *fout;
		if(gP->filename_output == NULL) {
			// Open stdout
			fout = stdout;
		}
		else {
			// Use the given filename
			fout = fopen(gP->filename_output, "wb");
		}

		Graph* G = DB[ng];

		// Print in the Subdue input format

		// Print the header
		if(DB.size()>1) {
			if(ng>0)
				fprintf(fout, "\n");
			if(G->get_class()==1)
				fprintf(fout, "XP\n");
			else
				fprintf(fout, "XN\n");
		}

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
		// Print all the edges
		if(gP->output_opt_sorting==OUTPUT_OPT_SORTED)
		{
			print_sorted_edge(gP, G, fout);
		}
		else
		{
			for(i=0; i<G->nodes()-1; i++)
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
		}

		if(gP->filename_output != NULL)
			fclose(fout);
	}

}

/***
Output the edge in a sorted way. When an edge 'i-j' is printed,
at least another edge including i or j has been printed before.
This could be used with some algorithms that want to read only
already known nodes at each line.

  Put all the nodes in White (W)
  Put one node (any) in Grey (G)
  While a grey node S exists, do
    For each link (S,S'), do
      If Color(S')=G
        PRINT("S-S'")
      Else If Color(S')=W
        PRINT("S-S'")
        Color(S')=G
      Else // Color(S')=B
      End If
    End For
    Color(S)=B
  End While

WARNING! This function can PRUNE a part of the tree: if it is
a forest (not all the nodes are connected), only the tree
containing the fist node (node[0]) will be printed.

 ***/

void print_sorted_edge(GLOBAL *gP, Graph *G, FILE *fout)
{
	int i, gr, num_grey=1, num_black=0, cycle=0;
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
		//printf("Grey found : %d\n", gr);
		// Look for each edge (S,S')
		for(i=0;i<G->nodes();i++)
		{
			if((G->weight(gr,i)!=CK_INFINITY) && (G->weight(gr,i)!=0.0))
			{
				// We found a link (S,S') = (gr,i)
				//printf("\tLink found : %d - %d (color: %c - %c)\n", gr, i, color[gr], color[i]);
				if((color[i]=='g') || (color[i]=='w'))
				{
					// Print the edge gr-i
					if(gP->output_opt_label==OUTPUT_OPT_EMPTYLABEL)
					{
						// Use nothing for the label of each node
						// Better to extract substructure, because each weight could be different for each graph
						fprintf(fout, "%c %d %d \"\"\n",((gP->output_opt_direction==OUTPUT_OPT_DIRECTED)?'d':'u'), translated_dico_index(gP, G, gr),translated_dico_index(gP, G, i));
					}
					else if(gP->output_opt_label==OUTPUT_OPT_WEIGHTLABEL)
					{
						// Use the weight for the label of each node
						fprintf(fout, "%c %d %d \"%f\"\n",((gP->output_opt_direction==OUTPUT_OPT_DIRECTED)?'d':'u'), translated_dico_index(gP, G, gr),translated_dico_index(gP, G, i), G->weight(gr,i));
					}
					else if(gP->output_opt_label==OUTPUT_OPT_IJLABEL)
					{
						// Use the indices for the label of each node
						fprintf(fout, "%c %d %d \"%d-%d\"\n",((gP->output_opt_direction==OUTPUT_OPT_DIRECTED)?'d':'u'), translated_dico_index(gP, G, gr),translated_dico_index(gP, G, i), gr+1, i+1);
					}
					else if(gP->output_opt_label==OUTPUT_OPT_EDGELABEL)
					{
						// Use the original values for the label of each node
						fprintf(fout, "%c %d %d \"%s\"\n",((gP->output_opt_direction==OUTPUT_OPT_DIRECTED)?'d':'u'), translated_dico_index(gP, G, gr),translated_dico_index(gP, G, i), G->edge_label(gr,i));
					}
					if(color[i]=='w')
					{
						color[i]='g';
						num_grey++;
					}
				}
				//printf("\t\tModified into : %d - %d (color: %c - %c)\n", gr, i, color[gr], color[i]);

			}
		}
		color[gr]='b';
		num_grey--;
		//printf("\tModified into : %d (color: %c)\n", gr, color[gr]);
	}
	Libera_vector_char(color);
}

