/*****
Class Graph.

*****/

#include "graph.h"

namespace CHECKDB {

/**************************************************\
Constructor.
_edge_mode:
  0: adjacency matrix representation: when it is small, weight[][] is filled
  1: edge list representation: edge_table is filled
\**************************************************/
Graph::Graph(int _edge_mode) {
	num_nodes = 0;
	edge_mode = _edge_mode;
	weights = NULL;
	num_existing = 0;
	num_different = 0;
	graph_type = NULL;
	num_edges = 0;
	directed_edges = -1;
	num_directed_edges = 0;
	num_non_directed_edges = 0;
	g_class = 1;
}


/**************************************************\
Destructor.
\**************************************************/
Graph::~Graph() {
  unsigned int i;
  for(i=0;i<node_table.size();i++)
  {
    if(node_table[i].label!=NULL) free(node_table[i].label);
    if(node_table[i].field!=NULL) free(node_table[i].field);
    if(node_table[i].line!=NULL) free(node_table[i].line);
  }
  for(i=0;i<edge_table.size();i++)
  {
    if(edge_table[i].label!=NULL) free(edge_table[i].label);
  }
  Libera_matriz_dinamica_float(weights, num_nodes);
}
	
/**************************************************\
Add an node. The key is an internal name used to
refer to this node instead of its index.
\**************************************************/
void Graph::add_node(char* key, char* label) {

	//printf("CALL add_node(%s)\n", label);
	if(edge_mode == 0) {
		// adjacency matrix representation
		float **new_weights;
		int i, j;
		new_weights = Matriz_dinamica_float(num_nodes+1, num_nodes+1);
		for(i=0;i<num_nodes;i++)
			for(j=0;j<num_nodes;j++) {
				new_weights[i][j] = weights[i][j];
			}
		Libera_matriz_dinamica_float(weights, num_nodes);
		weights = new_weights;
	} else {
		// edge list representation
	}
	
	SNODE node;
	node.key = strdup(key);
	node.label = strdup(label);
	node.field = NULL;
	node.line = NULL;
	node.visible = 1;
	node.posx = 0;
	node.posy = 0;
	node_table.push_back(node);
	num_nodes++;
}

void Graph::add_node(char* label) {

	add_node(label, label);
}

void Graph::add_node(int label) {
	char buffer[20];
	sprintf(buffer, "%d", label);
	add_node(buffer);
}

/**************************************************\
Add the edge (n1,n2) of weight w.
The nodes need to be already added.
Note: if 'w' is 0.0, the edge is not printed in output.
\**************************************************/
void Graph::add_edge(int n1, int n2, float w, char* label, char directed) {
	
	//printf("CALL add_edge(%d, %d, %f, %s, %d)\n", n1, n2, w, label, directed);
	if(edge_mode == 0) {
		// adjacency matrix representation
		weights[n1][n2] = w;
		if(!directed) {
			weights[n2][n1] = w;
		}
	} else {
		// edge list representation
		SEDGE edge;
		edge.i = n1;
		edge.j = n2;
		edge.w = w;
		edge.label = strdup(label);
		edge.directed = directed;
		edge_table.push_back(edge);
	}
	
	num_edges++;
	if(directed == 0)
		num_non_directed_edges++;
	else
		num_directed_edges++;
	if(num_non_directed_edges==0) {
		if(num_directed_edges==0) {
			directed_edges = -1;
		} else {
			directed_edges = 1;
		}
	} else {
		if(num_directed_edges==0) {
			directed_edges = 0;
		} else {
			directed_edges = -1;
		}
	}
}

/**************************************************\
Add the edge (n1,n2) of weight w, using the node keys.
The nodes need to be already added.
Note: if 'w' is 0.0, the edge is not printed in output.
\**************************************************/
void Graph::add_edge(char* key1, char* key2, float w, char* label, char directed) {
	int n1 = find_node(key1);
	int n2 = find_node(key2);
	if((n1>=0) && (n2>=0)) {
		//printf("Add edge: %d %d\n", n1, n2);
		add_edge(n1, n2, w, label, directed);
	}
}
	
/**************************************************\
Get the number of nodes.
\**************************************************/
int Graph::nodes() {
	return num_nodes;
}

/**************************************************\
Get the number of edges.
\**************************************************/
int Graph::edges() {
	return num_edges;
}


/**************************************************\
Get node n.
\**************************************************/
SNODE Graph::get_node(int n) {
	return node_table[n];
}


/**************************************************\
Get edge e.
\**************************************************/
SEDGE Graph::get_edge(int e) {
	
	if(edge_mode == 0) {
		// adjacency matrix representation
		int i, j, n2 = -1;
		for(i=0;i<num_nodes;i++)
			for(j=(directed_edges==0?i:0);j<num_nodes;j++) {
				if(fabs(weights[i][j])>GRAPH_EPSILON)
					n2++;
				if(n2 == e) {
					SEDGE edge;
					edge.i = i;
					edge.j = j;
					edge.w = weights[i][j];
					edge.label = NULL;
					edge.directed = directed_edges;
					return edge;
				}
			}
		SEDGE edge;
		edge.i = -1;
		edge.j = -1;
		edge.w = 0.0;
		edge.label = NULL;
		edge.directed = -1;
		return edge;
	} else {
		// edge list representation
		return edge_table[e];
	}
}

/**************************************************\
Get edge weight (n1, n2).
\**************************************************/
float Graph::weight(int n1, int n2) {
	
	if(edge_mode == 0) {
		// adjacency matrix representation
		return weights[n1][n2];
	} else {
		// edge list representation
		unsigned int e;
		for(e=0;e<edge_table.size();e++) {
			if((edge_table[e].i == n1) && (edge_table[e].j == n2)) {
				//printf("CALL weight(%d, %d) = %f\n", n1, n2, edge_table[e].w);
				return edge_table[e].w;
			}
		}
		return 0.0;
	}
}

/**************************************************\
Find the node index given its key. Return -1 if not
found.
\**************************************************/
int Graph::find_node(char* key) {
	for(unsigned int n=0;n<node_table.size();n++) {
		if(!strcmp(node_table[n].key, key))
			return n;
	}
	return -1;
}


/**************************************************\
Get node n label.
\**************************************************/
char* Graph::node_label(int n) {
	return node_table[n].label;
}

char* Graph::node_label(char* key) {
	int n = find_node(key);
	if(n>=0)
		return node_table[n].label;
	else
		return (char*)"";
}

/**************************************************\
Set the label of the node n.
\**************************************************/
void Graph::set_node_label(int n, char* label) {
	if(node_table[n].label!=NULL) free(node_table[n].label);
	node_table[n].label = strdup(label);
}

void Graph::set_node_label(char* key, char* label) {
	int n = find_node(key);
	if(n>=0) {
		if(node_table[n].label!=NULL) free(node_table[n].label);
		node_table[n].label = strdup(label);
	}
}


/**************************************************\
Get edge (i,j) label.
\**************************************************/
char* Graph::edge_label(int i, int j) {
	int e = find_edge(i, j);
	if(e>-1)
		return edge_table[e].label;
	else
		return (char *)""; //Graph::EMPTY_LABEL;
}

/**************************************************\
Get the graph label.
\**************************************************/
char* Graph::label(void) {
	return g_label;
}

/**************************************************\
Set the graph label.
\**************************************************/
void Graph::set_label(char* _label) {
	g_label = strdup(_label);
}

/**************************************************\
Set edge weight (n1, n2).
\**************************************************/
void Graph::set_weight(int n1, int n2, float w) {

	if(edge_mode == 0) {
		// adjacency matrix representation
		weights[n1][n2] = w;
	} else {
		// edge list representation
		unsigned int i;
		for(i=0;i<edge_table.size();i++) {
			if((edge_table[i].i == n1) && (edge_table[i].j == n2)) {
				edge_table[i].w = w;
				return;
			}
		}
	}
}

/**************************************************\
Set visibility node n.
\**************************************************/
void Graph::set_visibility(int n, int vis) {

	node_table[n].visible = vis;
}



/**************************************************\
Return 1 if the matrix encoded in weights is symmetrical, 0 if not.
\**************************************************/
int Graph::is_symmetrical(void)
{
  int i, j;
  for(i=0; i<nodes(); i++)
    for(j=i+1; j<nodes(); j++)
    {
      if(weight(i,j) != weight(j,i))
        return 0;
    }
  return 1;
}


/**************************************************\
Compute the number of edges in the graph
Is the matrix is symmetrical, each link is an edge, so M[i][j] and
M[j][i] is counted once, but only if not on the diagonal.
\**************************************************/
int Graph::compute_num_edges(void)
{
  /*int ne = 0;
  int is_sim = is_symmetrical();
  int i, j;
  for(i=0; i<nodes(); i++)
	for(j=0; j<nodes(); j++)
	{
		if(weight(i,j) != 0.0)
		  if(is_sim)
		  {
		    if(i==j) ne++;
		    else if(i<j) ne++;
		  }
		  else
		  {
		    ne++;
		  }
	}
  return ne;*/
  return num_edges;
}

/**************************************************\
Return some statistics.
\**************************************************/
int Graph::get_num_existing(void) {
	return num_existing;
}

int Graph::get_num_different(void) {
	return num_different;
}

char* Graph::get_graph_type(void) {
	return graph_type;
}

/**************************************************\
Print the graph.
\**************************************************/
void Graph::print(SNODE sn) {
	printf("%s (field=%s, vis=%d, pos=(%f,%f))\n", sn.label, sn.field, sn.visible, sn.posx, sn.posy);
}

void Graph::print(SEDGE se) {
	printf("%s (%d-%d, w=%f, dir=%d)\n", se.label, se.i, se.j, se.w, se.directed);
}

void Graph::print(void) {
	//int i, j;
	printf("==========\n");
	printf("nodes() = %d\n", nodes());
	for(int i=0;i<nodes();i++) {
		printf("NODE %d: ", i);
		print(get_node(i));
	}
	printf("edge_mode() = %d\n", edge_mode);
	printf("class() = %d\n", get_class());
	printf("edges() = %d\n", edges());
	printf("compute_num_edges() = %d\n", compute_num_edges());
	// Using the edge index
	for(int i=0;i<edges();i++) {
		printf("EDGE %d: ", i);
		print(get_edge(i));
	}
	// Using the weight (matrix representation)
	for(int i=0;i<nodes();i++)
		for(int j=0;j<nodes();j++) {
			if(fabs(weight(i,j))>GRAPH_EPSILON)
				printf("EDGE (%d,%d): w=%f\n", i, j, weight(i,j));
		}
	// Using the first()/next() interface
	SNEXTEDGE nxe = first_edge();
	int num_edges = edges();
	for(int e=0; e<num_edges; e++) {
		SEDGE edge = get_edge(nxe);
		printf("NEXTEDGE (%d,%d): w=%f, lab='%s'\n", edge.i, edge.j, edge.w, edge.label);
		nxe = next_edge(nxe);
	}
	printf("get_num_existing() = %d\n", get_num_existing());
	printf("get_num_different() = %d\n", get_num_different());
	printf("get_graph_type() = %s\n", get_graph_type());
	printf("is_directed() = %d\n", is_directed());
	printf("==========\n");
}

/**************************************************\
Gives the first edge of the graph.
\**************************************************/
SNEXTEDGE Graph::first_edge(void) {
	SNEXTEDGE nxe;
	int i, j;
	int nn = nodes();
	
	nxe.i = -1;
	nxe.j = -1;
	nxe.e = -1;
					
	if(edge_mode == 0) {
		// adjacency matrix representation
		for(i=0;i<nn;i++)
			for(j=0;j<nn;j++) {
				if(fabs(weight(i,j))>GRAPH_EPSILON) {
					nxe.i = i;
					nxe.j = j;
					return nxe;
				}
			}

	} else {
		// edge list representation
		nxe.e = 0;
	}
	return nxe;
}

/**************************************************\
Gives the next edge of the graph.
\**************************************************/
SNEXTEDGE Graph::next_edge(SNEXTEDGE old) {
	SNEXTEDGE nxe = old;
	//int i, j, e;
	int nn = nodes();
	if(directed_edges==0) {
		int v1 = MIN(nxe.i, nxe.j);
		int v2 = MAX(nxe.i, nxe.j);
		nxe.i = v1;
		nxe.j = v2;
	}
	
	if(edge_mode == 0) {
		// adjacency matrix representation
		do {
			nxe.j++;
			if(nxe.j == nn) {
				nxe.i++;
				nxe.j = (directed_edges==0?nxe.i:0);
			}
			if(nxe.i == nn)
				break;
		} while(fabs(weight(nxe.i,nxe.j))<GRAPH_EPSILON);
		return nxe;

	} else {
		// edge list representation
		nxe.e++;
		return nxe;
	}
}

/**************************************************\
Convert a SNEXTEDGE into an EDGE.
\**************************************************/
SEDGE Graph::get_edge(SNEXTEDGE nxe) {
	if(edge_mode == 0) {
		// adjacency matrix representation
		SEDGE edge;
		edge.i = nxe.i;
		edge.j = nxe.j;
		edge.w = weights[nxe.i][nxe.j];
		edge.label = NULL;
		edge.directed = directed_edges;
		return edge;

	} else {
		// edge list representation
		return get_edge(nxe.e);
	}
}

/**************************************************\
Find the index of the edge (i,j).
\**************************************************/
int Graph::find_edge(int _i, int _j) {

	int i=_i, j=_j;
	if(directed_edges==0) {
		i = MIN(_i, _j);
		j = MAX(_i, _j);
	}
	if(edge_mode == 0) {
		// adjacency matrix representation�
		if(fabs(weights[i][j])<GRAPH_EPSILON)
			return -1;
		int n2 = 0;
		for(int ni=0;ni<num_nodes;ni++)
			for(int nj=(directed_edges==0?ni:0);nj<num_nodes;nj++) {
				if((ni==i) && (nj==j))
					break;
				if(fabs(weights[ni][nj])>GRAPH_EPSILON)
					n2++;
			}
		return n2;
	} else {
		// edge list representation
		for(int e=0; e<num_edges; e++) {
			int n1 = edge_table[e].i;
			int n2 = edge_table[e].j;
			if( edge_table[e].directed) {
				if((n1==i) && (n2==j))
					return e;
			} else {
				if((n1==i) && (n2==j))
					return e;
				if((n2==i) && (n1==j))
					return e;
			}
		
		}
	}
	
	return -1;
}

/**************************************************\
Return if the edges of the graph are directed (1) or
not (0).
\**************************************************/
int Graph::is_directed(void) {
	return directed_edges;
}

/**************************************************\
Get the class of the graph.
\**************************************************/
int Graph::get_class(void) {
	return g_class;
}

/**************************************************\
Set the class of the graph:
1: positive graph
any other value: negative graph
\**************************************************/
void Graph::set_class(int c) {
	g_class = c;
}


}
