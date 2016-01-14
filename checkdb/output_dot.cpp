#include "output_dot.h"

/**************************************************\
Write a graph DB in the DOT format.
Notes:
- Write only the first graph of the DB.
\*************************************************/
void write_dot_file(GLOBAL* gP, vector<Graph*> DB) {

	int i, j;
	Graph* G = DB[0];
	
	FILE *fout;
	if(gP->filename_output == NULL)
		fout = stdout;
	else {
		fout = fopen(gP->filename_output, "wb");
	}

    // Print in the Graphviz input DOT format
    // To export in PNG, use:
    // dot -Gratio=fill -Gsize=15,15 -Kneato -Tpng -o map.png map.dot && eog map.png
    // fontsize=20 for Francia/Japan, 30 for Espana/Alemania/China
    //baseh_chronostart();
    //int k;
    //for(k=0;k<1000;k++){
    fprintf(fout, "graph G {\n");
    // Normal graph
    //fprintf(fout, "\tnode [shape=none, margin=0, fixedsize=false, width=0.2, height=0.2];\n");
    // Normal graph
    //fprintf(fout, "\tx, margin=0, fixedsize=false, /*width=1.2, height=1.2*/];\n");
    // Yellow circles + maximizing the proximity between nodes
    fprintf(fout, "\tratio=auto;\n");
    fprintf(fout, "\tsize=\"10,10\";\n");
    fprintf(fout, "\toverlap=\"scale\"; \n");
    fprintf(fout, "\tsep=-0.3;\n");
    fprintf(fout, "\tcenter=true;\n");
    fprintf(fout, "\t\n");
    fprintf(fout, "\tedge [size=1];\n");
    //fprintf(fout, "\tnode [shape=circle, margin=0, fixedsize=true, width=1.0, height=1.0, fontsize=20, fillcolor=gold2, style=filled, color=white];\n");
	fprintf(fout, "\tnode [shape=box, margin=\"0,0\", fixedsize=false, fontsize=%d, fillcolor=gold2, style=filled, color=white, width=0, height=0];\n", DOT_FONTSIZE);
    // Yellow circles + minimizing the crossing between edges
    /*fprintf(fout, "\tratio=auto;\n");
    fprintf(fout, "\tsize=\"10,10\";\n");
    fprintf(fout, "\toverlap=\"scale\"; \n");
    fprintf(fout, "\tsep=-0.3;\n");
    fprintf(fout, "\tcenter=true;\n");
    fprintf(fout, "\tmodel=subset;\n");
    fprintf(fout, "\t\n");
    fprintf(fout, "\tedge [size=1];\n");
    fprintf(fout, "\tnode [shape=circle, margin=0, fixedsize=true, width=3.0, height=3.0,\n");
    fprintf(fout, "\t\tfontsize=30, fillcolor=gold2, style=filled, color=white];\n");*/
    // Print all the labels
    for(i=0; i<G->nodes(); i++)
    {
	if (G->get_node(i).visible)
	{
	  // Use the category name for the label of each node
	  //fprintf(fout, "\t%d [label=\"%s\"];\n", translated_dico_index(gP, G, i), s1=str_replace(G->node_table[i].category, " ", "\\n"));
	  // free(s1);
	  fprintf(fout, "\t%d [label=\"%s\"];\n", translated_dico_index(gP, G, i), G->node_label(i));
	  
	}
    }
    // Print all the edges
    for(i=0; i<G->nodes(); i++)
    	for(j=0; j<i; j++)
    	    if (((G->weight(i,j)!=0.0) || (G->weight(j,i)!=0.0)) && (G->get_node(i).visible) && (G->get_node(j).visible))
					{
						if(gP->output_opt_label==OUTPUT_OPT_EMPTYLABEL) {
							// Without the weights
							fprintf(fout, "\t%d -- %d;\n",translated_dico_index(gP, G, i),translated_dico_index(gP, G, j));
						} else {
							// Using the weights for labels
							fprintf(fout, "\t%d -- %d [label=\"%f\"];\n",translated_dico_index(gP, G, i),translated_dico_index(gP, G, j), G->weight(i,j));
						}
					}
    fprintf(fout, "}\n");
    //}
    //fprintf(stderr, "/* Generated in %f seconds. */\n", (double)baseh_chronostop(0)/1000);
	
  if(gP->filename_output != NULL)
  	fclose(fout);
	
}
