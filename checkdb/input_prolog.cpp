#include "input_prolog.h"



/**************************************************\
Open a graph DB in the Prolog format.
Specific to the Holder data format examples here: http://www.eecs.wsu.edu/mgd/gdb.html
Notes:
- Manage properly multiple graphs in the DB.
- Read undirected graphs
- Check the 'mutagenic' property to decide between positive or negative graph
\*************************************************/

vector<Graph*> open_prolog_file(GLOBAL* gP, FILE* fin) {

	// Start the conversion
	
	vector<Graph*> DB;
	char buffer[100];
	//int n_graph, n_node, n_label;
	//int e_graph, e_node1, e_node2, e_label;
	//int ret;
	char* cret;
	//int last_n_graph;
	Graph *G = NULL;
	
	while(true) {
		cret = fgets(buffer, 100, fin);	// buffer = "prop_atom(d1_1,c,22,-0.117)."
		if(cret==NULL)
			break;
		
		std::vector<std::string> v_fct = split(buffer, '(');	// v_fct = [ "prop_atom" , "d1_1,c,22,-0.117)." ]
		
		if(v_fct[0].compare("mutagenic") == 0) {
		
			// START OF A NEW GRAPH
			
			if(G != NULL) {
				// END OF THE PREVIOUS GRAPH
				// Store the previous graph in the DB
				DB.push_back(G);
				G = NULL;
			}
				
			std::vector<std::string> v_args0 = split(v_fct[1], ')');	// v_args0 = [ "d1_1,c,22,-0.117", "." ]
			std::vector<std::string> v_args = split(v_args0[0], ',');	// v_args = [ "d1_1", "c", "22", "-0.117" ]
			
			//std::cout << "Reading graph: " << v_args[0] << std::endl;
			
			// Allocation of a graph
			G = new Graph(Graph::EDGE_LIST_MODE);
			
			G->set_label((char*) v_args[0].c_str());
			
			// Read the type of the graph (positive or negative)
			if(v_args[1].compare("yes") == 0)
				G->set_class(1);
			else
				G->set_class(0);
				
		} else if(v_fct[0].compare("mol2atm") == 0) {
		
			std::vector<std::string> v_args0 = split(v_fct[1], ')');
			std::vector<std::string> v_args = split(v_args0[0], ',');

			// New node
			G->add_node((char*)v_args[1].c_str());
			
		} else if(v_fct[0].compare("prop_atom") == 0) {
		
			std::vector<std::string> v_args0 = split(v_fct[1], ')');
			std::vector<std::string> v_args = split(v_args0[0], ',');

			// Properties of a node
			G->set_node_label((char*)v_args[0].c_str(), (char*)v_args[1].c_str());
			
		} else if(v_fct[0].compare("bond") == 0) {
		
			std::vector<std::string> v_args0 = split(v_fct[1], ')');
			std::vector<std::string> v_args = split(v_args0[0], ',');

			// New edge
			G->add_edge((char*) v_args[0].c_str(), (char*) v_args[1].c_str(), 1.0, (char*) v_args[2].c_str(), 0);
		}
		
		if(feof(fin))
			break;
	}
	
	if(G != NULL) {
		DB.push_back(G);
	}
	
	return DB;
}

