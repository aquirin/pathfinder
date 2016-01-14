#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#ifdef linux
	#include <unistd.h>
#else
	
#endif
#include "base.h"
#include "routines.h"

#define TRUE	1.0
#define FALSE	0.0
#define CK_INFINITY	10E7

#define DATABASE_NAME		"ISI_Categories.txt"
#define DATABASE_SIZEMAX	300
#define SCORED_DATABASE		"ISI_database_scored.txt"
#define SCORED_DATABASE_SIZEMAX	1000
#define DICO_DATABASE_SIZEMAX	10000
#define ALLOW_CHECK_RECOGNIZED	0		// 1 to activate permanently "-c recognized"
#define TYPE_CENTRAL_NODE	4	/* type for the central node, see floyd_warshall() */

#define SIGN(x)		((x)/fabs(x))
#define IS_NUM(x)	((x)==0?0:1)

/* CONSTANTS */

#define ENTRADA_1 "*vertices %i\n"
#define ENTRADA_2 "*Vertices %i\n"


// Input types
#define INPUT_PAJEK		0	// .net files, pajek files
#define INPUT_SUBDUE	1	// Subdue files
#define INPUT_GASTON	2	// Gaston files
#define INPUT_GSPAN		3	// gSpan files
#define INPUT_FFSM		4	// FFSM files
#define INPUT_PROLOG	5	// Prolog files

// Output types
#define OUTPUT_NONE		0
#define OUTPUT_MATRIX		1
#define OUTPUT_EDGES		2
#define OUTPUT_ARCS		3
#define OUTPUT_HUMAN		4	/* edges values + categories + fields in human readable format */
#define OUTPUT_DOT		5	/* Graphviz input file */
#define OUTPUT_SUBDUE		6	/* Subdue 5.2.1 input file */
#define OUTPUT_PLAIN		7	/* GraphViz plain format */
#define OUTPUT_GASTON		8	/* Gaston graph mining algorithm format */
#define OUTPUT_JXPLORE		9	/* JXplore format */
#define OUTPUT_GSPAN		10	/* gSpan format */

#define OUTPUT_OPT_DIRECTED	1	/* Output directed links */
#define OUTPUT_OPT_UNDIRECTED	2	/* Output undirected links */

#define OUTPUT_OPT_WEIGHTLABEL	1	/* Output the weights of the edge labels */
#define OUTPUT_OPT_EMPTYLABEL	2	/* Output empy edge labels */
#define OUTPUT_OPT_IJLABEL	3	/* Output 'I-J' edge labels (I,J are the indices of the nodes) */
#define OUTPUT_OPT_EDGELABEL	4	/* Output the original edge labels */


#define OUTPUT_OPT_SORTED	1	/* Output sorted edge (see print_sorted_edge()) */
#define OUTPUT_OPT_UNSORTED	2	/* Output unsorted edge */

//#define OUTPUT_SUBDUE_NOWEIGHT	6	/* Subdue 5.2.1 input file, use an empty string for the edge labels */
//#define OUTPUT_SUBDUE_WEIGHT	7	/* Subdue 5.2.1 input file */
//#define OUTPUT_SUBDUE_DIRECTED_EDGENAME	8	/* Subdue 5.2.1 input file, use 'd' (doubled) instead of 'u' for the edges, use I-J for the labels */
//#define OUTPUT_SUBDUE_DIRECTED_EMPTYNAME	9	/* Subdue 5.2.1 input file */

#define FILTER_ALL		0
#define FILTER_LESS		2
#define FILTER_GREATER		3
#define FILTER_EQUAL		4
#define FILTER_SFL		5	/* Same Field Link */
#define FILTER_DFL		6	/* Different Field Link */
#define FILTER_LESS_EQUAL	7	/* less or equal to weight */
#define FILTER_GREATER_EQUAL	8	/* greater or equal to weight */
#define FILTER_FILTER3		9	/* delete L | DFL(L)=1 && w(L) < MIN(w(l)) for all l | SFL(l)=1 */

#define NFILTER_ALL		0
#define NFILTER_UNISOLATED	1

#define STAT_ALL		1
#define STAT_NUM_NODES		2
#define STAT_NUM_EDGES		3
#define STAT_GRAPH_TYPE		4	/* edges, arcs or matrix */
#define STAT_SYMMETRY		5
#define STAT_NUM_DOUBLED	6	/* %d doubled links (should be 0) */
#define STAT_NUM_DIFF_DOUBLED	7	/* %d different doubled links (must be 0) */
#define STAT_LINKS		8	/* stats on links (avg, min, max, median, ...) */
#define STAT_NODES		9	/* stats on nodes (avg inner/outer values, ...) */
#define STAT_CENTRAL		10	/* central nodes (4 types) */
#define STAT_CYCLES		11	/* 0 if no cycles, >0 if cycles */
#define STAT_SHOW_CYCLES	12	/* show the edges that make the cycles */

#define CHECK_ALL		1
#define CHECK_RECOGNIZED	2	/* The category '%s' of the node %d was not recognized */
#define CHECK_TOT_RECOGNIZED	3	/* The categories of %d nodes were not recognized */
#define CHECK_DIFF_FIELD	4	/* Node %d (%s:%s) is linked to a diff. field : node %d (%s:%s) */
#define CHECK_ALL_DFL_WEIRD	5	/* num+avg DFL, num+avg non-DFL, num weird, num non_weird */

#define MODIFY_ADD		1	/* W = W + cst */
#define MODIFY_SUB_FROM_CST	2	/* W = cst - W */
#define MODIFY_SET		3	/* W = cst */
#define MODIFY_MULTIPLY		4	/* W = W * cst */
#define MODIFY_COMPARE_WITH	5	/* W = sign(W - cst) */

#define DOT_FONTSIZE		7	/* For OUTPUT_DOT, font size of the node labels */

/* STRUCTURES */
/*typedef struct _SEDGE
{
  int i, j;			// node in, out
  float v;			// weight
  char* label;
  char directed;
} SEDGE;*/


/*typedef struct _SNODE
{
  char* category;		// One of the 263 categories (label)
  char* field;			// One of the 13 fields
  char* line;			// Original node line
  char visible;			// 1 if visible in the output, 0 if not (useful to delete nodes quickly)
  double posx, posy;	// Coordinates indicated in the NET file (if found)
} SNODE;*/

typedef struct _SCATNODE
{
  char* category;		// One of the 263 categories
  char* field;			// One of the 13 fields
} SCATNODE;

typedef struct _SCOREDNODE
{
  char* f1;			// One of the 13 fields
  char* c1;			// One of the 263 categories
  char* f2;			// One of the 13 fields
  char* c2;			// One of the 263 categories
  float score;			// Score to denote the relationship between f1:c1 and f2:c2
} SSCOREDNODE;

typedef struct _DICONODE
{
  char* name;			// A node name
  int index;			// Its new index in the output
} SDICONODE;

// Global parameters
typedef struct _GLOBAL
{
  char* filename_ISI_DB;		// filename of the ISI_DB
  char* filename_output;		// output filename
  int check_type;			// type of network check
  int num_categories;	// number of categories
  SCATNODE *field_table;	// table of the field/category for the database (field_table[category_number] = SNODE)
  SSCOREDNODE *scored_table;	// table of the scored relationships f1:c1 <=> f2:c2 (scored_table[relation_index] = SSCOREDNODE)
  int num_scored_relations;	// number of cells in scored_table
  SDICONODE *dico_table;		// table of the dictionnary relationships index => node_name (dico_table[node_index] = SDICONODE)
  int num_dico_entries;		// number of cells in dico_table
  int num_nodos_visible;	//numero de nodos visible de la matriz
  int *ending_table;		// table of the ending nodes
  int *isolated_table;		// table of the isolated nodes
  float ***pesos;			// table of the shortest weights
  int ***predecessors;		// table of the predecessors
  int **size_sp;			// table of the size of the shortest path
  int central_node;		// the central node (using type TYPE_CENTRAL_NODE), 0-based
  int num_nodes_visible;	// number of visible nodes
  int output_type;
  int output_opt_direction;
  int output_opt_label;
  int output_opt_sorting;

} GLOBAL;

// Graph representation
/*typedef struct _GRAPH
{
  // This is always filled by the open_*_file() functions
  int num_nodes;	//numero de nodos de la matriz
  SNODE *node_table;	// table of the nodes (node_table[node_number] = SNODE)
  char edge_mode;	// 0: weight is filled, 1: edge_table is filled
  
  // In the case edge_mode = 0, this is filled
  float **weights;	// weight matrix for the edges
  
  // In the case edge_mode = 1, this is filled
  SEDGE *edge_table;	// table of the edges (edge_table[edge_number] = SEDGE)

  // Statistics (computed)
  int num_existing;	// number of existing edges
  int num_different;	// number of different existing edges
  char* graph_type;	// matrix, edges or arcs
  int num_edges;	// number of edges in the matrix

} GRAPH;*/

#endif // __GLOBAL_H__
