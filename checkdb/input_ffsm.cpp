#include "input_ffsm.h"



/**************************************************\
Open a graph DB in the FFSM format.
Notes:
- FFSM format (at least for the chemical DB given
  as an example) is a set of 2 files: .nodes and .edges
  These two files should be merged together before
  the conversion.
- Manage properly multiple graphs in the DB.
\*************************************************/

vector<Graph*> open_ffsm_file(GLOBAL* gP, FILE* fin) {

	// Start the conversion
	
	vector<Graph*> DB;
	char buffer[100];
	int n_graph, n_node, n_label;
	int e_graph, e_node1, e_node2, e_label;
	int ret;
	char* cret;
	int last_n_graph = -1;
	Graph *G = NULL;
	
	while(true) {
		cret = fgets(buffer, 100, fin);
		if(cret==NULL)
			break;
		ret = sscanf(buffer, "node %d %d %d\n", &n_graph, &n_node, &n_label);
		if(ret != 0) {
			// Read a node entry
			//printf("> node %d %d %d\n", n_graph, n_node, n_label);
			if(last_n_graph != n_graph) {
				if(G != NULL) {
					// END OF THE PREVIOUS GRAPH
					// Store the previous graph in the DB
					DB.push_back(G);
				}
				// START OF A NEW GRAPH
				// Allocation of a graph
				G = new Graph(Graph::EDGE_LIST_MODE);
				last_n_graph = n_graph;
			}
			sprintf(buffer, "%d", n_label);
			G->add_node(n_label);
		}
		ret = sscanf(buffer, "edge %d %d %d %d\n", &e_graph, &e_node1, &e_node2, &e_label);
		if(ret != 0) {
			// Read an edge entry
			//printf("> edge %d %d %d %d\n", e_graph, e_node1, e_node2, e_label);
			
			// When reading an edge entry, the nodes of the previous graph have to be read
			DB.push_back(G);
			
			sprintf(buffer, "%d", e_label);
			DB[e_graph]->add_edge(e_node1, e_node2, 1.0, buffer, 1);
		}
		if(feof(fin))
			break;
	}
	
	
	//printf("END!!\n");
	
	/*
	
	int num_graphs = gaston_database.trees.size();
	for(int ng=0; ng<num_graphs; ng++) {
	
		// Allocation of a graph
		Graph *G = new Graph(Graph::EDGE_LIST_MODE);
		
	  // Total number of nodes
	  int num_nodes = gaston_database.trees[ng]->nodes.size();
	  
	  // Read the name of each node
	  for(int n=0; n<num_nodes; n++) {
	  	int nlabel = gaston_database.trees[ng]->nodes[n].nodelabel;
		sprintf(buffer, "%d", nlabel);
	  	G->add_node(buffer);
	  }
	  
	  // Read each edge
		for(int n=0; n<num_nodes; n++) {
	  		// Current number of edges
	  		int num_edges = gaston_database.trees[ng]->nodes[n].edges.size();

			for(int e=0; e<num_edges; e++) {
				EdgeLabel edgelabel = gaston_database.trees[ng]->nodes[n].edges[e].edgelabel;
				NodeId tonode = gaston_database.trees[ng]->nodes[n].edges[e].tonode;
				int tonlabel = gaston_database.trees[ng]->nodes[tonode].nodelabel;
				
				if(n<tonode) {
					sprintf(buffer, "%d", edgelabel);
					G->add_edge(n, tonode, 1.0, buffer, 0);
				}
			}
		}
		
		DB.push_back(G);
	}*/
	
	return DB;
}

