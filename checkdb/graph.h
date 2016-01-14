#ifndef __GRAPH_H__
#define __GRAPH_H__

/* INCLUDES */
#include "base.h"
#include "global.h"
#include "memory.h"
#include <vector>
using namespace std;

namespace CHECKDB {

#define GRAPH_EPSILON 0.000000001

/* STRUCTURES */
typedef struct _SEDGE
{
  int i, j;			// node in, out
  float w;			// weight
  char* label;
  char directed;
} SEDGE;


typedef struct _SNODE
{
  char* key;		// Internal node ID. Can be used to set the label of a specific node using its key instead of its index. Key has to be unique for a given graph.
  char* label;		// In the case of Scientograms, one of the 263 categories (label)
  char* field;			// In the case of Scientograms, one of the 13 fields
  char* line;			// Original node line (in the case of Pajek file)
  char visible;			// 1 if visible in the output, 0 if not (useful to delete nodes quickly)
  double posx, posy;	// Coordinates indicated in the NET file (if found)
} SNODE;


// Used to follow the edges of the graph
// Valid only for the graph for which first_edge() was called!!
typedef struct _SNEXTEDGE
{
  int i, j;			// node in, out (edge_mode=0)
  int e;			// (edge_mode=1)
} SNEXTEDGE;


class Graph {

	// Const
public:
	static const int ADJENCY_MODE = 0;
	static const int EDGE_LIST_MODE = 1;
	
	// Members
private:

	// This is always filled by the open_*_file() functions
	int num_nodes;	//numero de nodos de la matriz
	vector<SNODE> node_table;	// table of the nodes (node_table[node_number] = SNODE)
	char edge_mode;	// 0: weight is filled, 1: edge_table is filled
  
	// In the case edge_mode = 0, this is filled
	float **weights;	// weight matrix for the edges
  
	// In the case edge_mode = 1, this is filled
	vector<SEDGE> edge_table;	// table of the edges (edge_table[edge_number] = SEDGE)
	
	char* g_label;		// Label of the graph

	// Statistics (computed)
	int num_existing;	// number of existing edges
	int num_different;	// number of different existing edges
	char* graph_type;	// matrix, edges or arcs
	int num_edges;	// number of edges in the matrix
	int num_directed_edges;	// number of directed edges
	int num_non_directed_edges;	// number of non directed edges
	int directed_edges;	// 0: edges are not directed, 1: edges are directed, -1: we do not know (read SEDGE.directed attribute of each edge to know)
	int g_class;	// Class of the graph. Typically: 0: negative graph, 1: positive graph
	
	// Functions
public:
	Graph(int _edge_mode);
	~Graph();
	void add_node(char* key, char* label);
	void add_node(char* label);
	void add_node(int label);
	void add_edge(int n1, int n2, float w, char* label, char directed);
	void add_edge(char* key1, char* key2, float w, char* label, char directed);
	int nodes();
	int edges();
	SNODE get_node(int n);
	SEDGE get_edge(int n);
	float weight(int n1, int n2);
	int find_node(char* key);
	char* node_label(int n);
	char* node_label(char* key);
	void set_node_label(int n, char* label);
	void set_node_label(char* key, char* label);
	char* edge_label(int i, int j);
	char* label(void);
	void set_label(char* _label);
	void set_weight(int n1, int n2, float w);
	void set_visibility(int n, int vis);
	int is_symmetrical(void);
	int compute_num_edges(void);
	int get_num_existing(void);
	int get_num_different(void);
	char* get_graph_type(void);
	void print(SNODE sn);
	void print(SEDGE se);
	void print(void);
	SNEXTEDGE first_edge(void);
	SNEXTEDGE next_edge(SNEXTEDGE old);
	SEDGE get_edge(SNEXTEDGE nxe);
	int find_edge(int i, int j);
	int is_directed(void);
	int get_class(void);
	void set_class(int c);

};

}


#endif // __GRAPH_H__
