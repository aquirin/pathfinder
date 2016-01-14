#include "output_gaston.h"

/**************************************************\
Write a graph DB in the Gaston format.
Notes:
- Manage properly multiple graphs in the DB.
\*************************************************/
void write_gaston_file(GLOBAL* gP, vector<Graph*> DB) {

  int i, j;
  
	FILE *fout;
	if(gP->filename_output == NULL)
		fout = stdout;
	else {
		fout = fopen(gP->filename_output, "wb");
	}
	
  for(int ng=0; ng<DB.size(); ng++) {

	// Header
    fprintf(fout, "t # %d\n", ng);
	
	Graph *G = DB[ng];
	
	//G->print();
	
	// Nodes
    for (i=0; i<G->nodes();i++){
      if(G->get_node(i).visible)
	    fprintf(fout, "v %d %d\n", i, translated_dico_index(gP, G, i));
    }
	
	// Edges
    /*for(i=0; i<G->nodes(); i++)
    	for(j=0; j<G->nodes(); j++) {
			//printf("d1 %f\n", G->weight(i,j));
			//printf("d2 %f\n", G->get_node(i).visible);
			//printf("d3 %f\n", G->get_node(j).visible);
			//printf("\n");
    	    if ((G->weight(i,j)!=0.0) && (G->get_node(i).visible) && (G->get_node(j).visible)) {
  	          int ei = find_edge_index(G, i, j);
			  fprintf(fout, "e %d %d %d\n",i,j,translated_dico_index_edge(gP, G, ei));
			}
		}*/
	SNEXTEDGE nxe = G->first_edge();
	int num_edges = G->edges();
	//printf("num_edges: %d\n", num_edges);
	for(int e=0; e<num_edges; e++) {
		//printf("Explore edge e=%d, nxe={i=%d, j=%d, e=%d}\n", e, nxe.i, nxe.j, nxe.e);
		SEDGE edge = G->get_edge(nxe);
		//printf("\tGot edge={i=%d, j=%d, w=%f, lab='%s', directed=%d}\n",
		//	edge.i, edge.j, edge.w, edge.label, edge.directed);
		
		if ((edge.w!=0.0) && (G->get_node(edge.i).visible) && (G->get_node(edge.j).visible)) {
			int ei = find_edge_index(G, edge.i, edge.j);
			fprintf(fout, "e %d %d %d\n",edge.i,edge.j,translated_dico_index_edge(gP, G, ei));
		}
		nxe = G->next_edge(nxe);
	}
	
  }
  
  if(gP->filename_output != NULL)
  	fclose(fout);
}


// Find the index of the edge (i,j)
int find_edge_index(Graph* G, int i, int j) {
	int e, n1, n2;
	int num_edges = G->compute_num_edges();
	for(e=0; e<num_edges; e++) {
		n1 = G->get_edge(e).i;
		n2 = G->get_edge(e).j;
		if(G->get_edge(e).directed) {
			if((n1==i) && (n2==j))
				return e;
		} else {
			if((n1==i) && (n2==j))
				return e;
			if((n2==i) && (n1==j))
				return e;
		}
		
	}
	return -1;
}
