#ifndef __INPUT_SUBDUE_H__
#define __INPUT_SUBDUE_H__

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "routines.h"
#include "global.h"
#include "graphops.h"
#include "memory.h"
#include <unistd.h>
#include <sys/times.h>

// DEFINES

#define SUBDUE_VERSION "5.2.1"

// Substructure evaluation methods
#define EVAL_MDL      1
#define EVAL_SIZE     2
#define EVAL_SETCOVER 3

// Starting strings for input files
#define SUB_TOKEN        "S"  // new substructure
#define PREDEF_SUB_TOKEN "PS" // new predefined substructure
#define POS_EG_TOKEN     "XP" // new positive example
#define NEG_EG_TOKEN     "XN" // new negative example

// Vertex and edge labels used for graph compression
#define SUB_LABEL_STRING     "SUB"
#define OVERLAP_LABEL_STRING "OVERLAP"
#define PREDEFINED_PREFIX    "PS"

#define NEG 0
#define POS 1

#define SUBD_FALSE 0
#define SUBD_TRUE 1

// Constants for graph matcher.  Special vertex mappings use the upper few
// unsigned long integers.  This assumes graphs will never have this many
// vertices, which is a pretty safe assumption.  The maximum double is used
// for initial costs.
#define MAX_UNSIGNED_LONG ULONG_MAX  // ULONG_MAX defined in limits.h
#define VERTEX_UNMAPPED   MAX_UNSIGNED_LONG
#define VERTEX_DELETED    MAX_UNSIGNED_LONG - 1
#define MAX_DOUBLE        DBL_MAX    // DBL_MAX from float.h

// Label types
#define STRING_LABEL  0
#define NUMERIC_LABEL 1

// General defines
#define LIST_SIZE_INC  100  // initial size and increment for realloc-ed lists
#define TOKEN_LEN     256  // maximum length of token from input graph file
#define FILE_NAME_LEN 512  // maximum length of file names
#define COMMENT       '%'  // comment character for input graph file
#define NUMERIC_OUTPUT_PRECISION 6
#define LOG_2 0.6931471805599452862 // log_e(2) pre-computed

#define SPACE ' '
#define TAB   '\t'
#define NEWLINE '\n'
#define DOUBLEQUOTE '\"'
#define CARRIAGERETURN '\r'


// STRUCTURES

//---------------------------------------------------------------------------
// Type Definitions
//---------------------------------------------------------------------------

typedef unsigned char UCHAR;
typedef unsigned char BOOLEAN;
typedef unsigned long ULONG;

// Label
typedef struct 
{
   UCHAR labelType;       // one of STRING_LABEL or NUMERIC_LABEL
   union 
   {
      char *stringLabel;
      double numericLabel;
   } labelValue;
   BOOLEAN used;          // flag used to mark labels at various times
} Label;

// Label list
typedef struct 
{
   ULONG size;      // Number of label slots currently allocated in array
   ULONG numLabels; // Number of actual labels stored in list
   Label *labels;   // Array of labels
} LabelList;

// Edge
typedef struct 
{
   ULONG   vertex1;  // source vertex index into vertices array
   ULONG   vertex2;  // target vertex index into vertices array
   ULONG   label;    // index into label list of edge's label
   BOOLEAN directed; // TRUE if edge is directed
   BOOLEAN used;     // flag for marking edge used at various times
                     //   used flag assumed FALSE, so always reset when done
   BOOLEAN spansIncrement;   // TRUE if edge crosses a previous increment
   BOOLEAN validPath;
} Edge;

// Vertex
typedef struct 
{
   ULONG label;    // index into label list of vertex's label
   ULONG numEdges; // number of edges defined using this vertex
   ULONG *edges;   // indices into edge array of edges using this vertex
   ULONG map;      // used to store mapping of this vertex to corresponding
                   //   vertex in another graph
   BOOLEAN used;   // flag for marking vertex used at various times
                   //   used flag assumed FALSE, so always reset when done
} Vertex;

// SubdueGraph
typedef struct 
{
   ULONG  numVertices; // number of vertices in graph
   ULONG  numEdges;    // number of edges in graph
   Vertex *vertices;   // array of graph vertices
   Edge   *edges;      // array of graph edges
} SubdueGraph;

// VertexMap: vertex to vertex mapping for graph match search
typedef struct 
{
   ULONG v1;
   ULONG v2;
} VertexMap;

// Instance
typedef struct _instance
{
   ULONG numVertices;   // number of vertices in instance
   ULONG numEdges;      // number of edges in instance
   ULONG *vertices;     // ordered indices of instance's vertices in graph
   ULONG *edges;        // ordered indices of instance's edges in graph
   double minMatchCost; // lowest cost so far of matching this instance to
                        // a substructure
   ULONG newVertex;     // index into vertices array of newly added vertex
                        //    (0 means no new vertex added)
   ULONG newEdge;       // index into edges array of newly added edge
   ULONG refCount;      // counter of references to this instance; if zero,
                        //    then instance can be deallocated
   VertexMap *mapping;  // instance mapped to substructure definition
   ULONG mappingIndex1; // index of source vertex of latest mapping
   ULONG mappingIndex2; // index of target vertex of latest mapping
   BOOLEAN used;        // flag indicating instance already associated
                        // with a substructure (for the current iteration)
   struct _instance *parentInstance;  // pointer to parent instance
} Instance;

// InstanceListNode: node in singly-linked list of instances
typedef struct _instance_list_node 
{
   Instance *instance;
   struct _instance_list_node *next;
} InstanceListNode;

// InstanceList: singly-linked list of instances
typedef struct 
{
   InstanceListNode *head;
} InstanceList;

// Substructure
typedef struct 
{
   SubdueGraph  *definition;         // graph definition of substructure
   ULONG  numInstances;        // number of positive instances
   ULONG  numExamples;         // number of unique positive examples
   InstanceList *instances;    // instances in positive graph
   ULONG  numNegInstances;     // number of negative instances
   ULONG  numNegExamples;      // number of unique negative examples
   InstanceList *negInstances; // instances in negative graph
   double value;               // value of substructure
   BOOLEAN recursive;          // is this substructure recursive?
   ULONG recursiveEdgeLabel;   // index into label list of recursive edge label
   double posIncrementValue;   // DL/#Egs value of sub for positive increment
   double negIncrementValue;   // DL/#Egs value of sub for negative increment
} Substructure;

// SubListNode: node in singly-linked list of substructures
typedef struct _sub_list_node 
{
   Substructure *sub;
   struct _sub_list_node *next;
} SubListNode;

// SubList: singly-linked list of substructures
typedef struct 
{
   SubListNode *head;
} SubList;

// MatchHeapNode: node in heap for graph match search queue
typedef struct 
{
   ULONG  depth; // depth of node in search space (number of vertices mapped)
   double cost;  // cost of mapping
   VertexMap *mapping;
} MatchHeapNode;

// MatchHeap: heap of match nodes
typedef struct 
{
   ULONG size;      // number of nodes allocated in memory
   ULONG numNodes;  // number of nodes in heap
   MatchHeapNode *nodes;
} MatchHeap;

// ReferenceEdge
typedef struct
{
   ULONG   vertex1;  // source vertex index into vertices array
   ULONG   vertex2;  // target vertex index into vertices array
   BOOLEAN spansIncrement; //true if edge spans to or from a previous increment
   ULONG   label;    // index into label list of edge's label
   BOOLEAN directed; // TRUE if edge is directed
   BOOLEAN used;     // flag for marking edge used at various times
                     //   used flag assumed FALSE, so always reset when done
   BOOLEAN failed;
   ULONG map;
} ReferenceEdge;

// ReferenceVertex
typedef struct
{
   ULONG label;    // index into label list of vertex's label
   ULONG numEdges; // number of edges defined using this vertex
   ULONG *edges;   // indices into edge array of edges using this vertex
   ULONG map;      // used to store mapping of this vertex to corresponding
                   // vertex in another graph
   BOOLEAN used;   // flag for marking vertex used at various times
                   // used flag assumed FALSE, so always reset when done
   BOOLEAN vertexValid;
} ReferenceVertex;

// ReferenceGraph
typedef struct
{
   ULONG  numVertices; // number of vertices in graph
   ULONG  numEdges;    // number of edges in graph
   ReferenceVertex *vertices;   // array of graph vertices
   ReferenceEdge   *edges;      // array of graph edges
} ReferenceGraph;

// ReferenceInstanceList
typedef struct _ref_inst_list_node
{
   InstanceList *instanceList;
   ReferenceGraph *refGraph;
   ULONG vertexListSize;
   ULONG edgeListSize;
   BOOLEAN firstPass;
   BOOLEAN doExtend;
   struct _ref_inst_list_node *next;
} RefInstanceListNode;

typedef struct
{
   RefInstanceListNode *head;
} RefInstanceList;

typedef struct _avltablenode
{
   struct avl_table *vertexTree;
   Substructure *sub;
   struct _avltablenode *next;
} AvlTableNode;

typedef struct
{
   AvlTableNode *head;
} AvlTreeList;

typedef struct
{
   AvlTreeList *avlTreeList;
} InstanceVertexList;

// Singly-connected linked list of subarrays for each increment
typedef struct _increment
{
   SubList *subList;           // List of subs for an increment
   ULONG incrementNum;         // Increment in which these substructures were 
                               // discovered
   ULONG numPosVertices;       // Number of pos vertices in this increment
   ULONG numPosEdges;          // Number of pos edges in this increment
   ULONG numNegVertices;       // Number of neg vertices in this increment
   ULONG numNegEdges;          // Number of neg edges in this increment
   ULONG startPosVertexIndex;  // Index of the first vertex in this increment
   ULONG startPosEdgeIndex;    // Index of the first edge in this increment
   ULONG startNegVertexIndex;  // Index of the first vertex in this increment
   ULONG startNegEdgeIndex;    // Index of the first edge in this increment
   double numPosEgs;           // Number of pos examples in this increment
   double numNegEgs;           // DNumber of pos examples in this increment
} Increment;

typedef struct _increment_list_node
{
   Increment *increment;
   struct _increment_list_node *next;
} IncrementListNode;

typedef struct
{
   IncrementListNode *head;
} IncrementList;

// Parameters: parameters used throughout SUBDUE system
typedef struct 
{
   char inputFileName[FILE_NAME_LEN];   // main input file
   char psInputFileName[FILE_NAME_LEN]; // predefined substructures input file
   char outFileName[FILE_NAME_LEN];     // file for machine-readable output
   SubdueGraph *posGraph;      // Graph of positive examples
   SubdueGraph *negGraph;      // Graph of negative examples
   double posGraphDL;    // Description length of positive input graph
   double negGraphDL;    // Description length of negative input graph
   ULONG numPosEgs;      // Number of positive examples
   ULONG numNegEgs;      // Number of negative examples
   ULONG *posEgsVertexIndices; // vertex indices of where positive egs begin
   ULONG *negEgsVertexIndices; // vertex indices of where negative egs begin
   LabelList *labelList; // List of unique labels in input graph(s)
   SubdueGraph **preSubs;      // Array of predefined substructure graphs
   ULONG numPreSubs;     // Number of predefined substructures read in
   BOOLEAN predefinedSubs; // TRUE is predefined substructures given
   BOOLEAN outputToFile; // TRUE if file given for machine-readable output
   BOOLEAN directed;     // If TRUE, 'e' edges treated as directed
   ULONG beamWidth;      // Limit on size of substructure queue (> 0)
   ULONG limit;          // Limit on number of substructures expanded (> 0)
   ULONG maxVertices;    // Maximum vertices in discovered substructures
   ULONG minVertices;    // Minimum vertices in discovered substructures
   ULONG numBestSubs;    // Limit on number of best substructures
                         //   returned (> 0)
   BOOLEAN valueBased;   // If TRUE, then queues are trimmed to contain
                         //   all substructures with the top beamWidth
                         //   values; otherwise, queues are trimmed to
                         //   contain only the top beamWidth substructures.
   BOOLEAN prune;        // If TRUE, then expanded substructures with lower
                         //   value than their parents are discarded.
   ULONG outputLevel;    // More screen (stdout) output as value increases
   BOOLEAN allowInstanceOverlap; // Default is FALSE; if TRUE, then instances
                                 // may overlap, but compression costlier
   ULONG evalMethod;     // One of EVAL_MDL (default), EVAL_SIZE or
                         //   EVAL_SETCOVER
   double threshold;     // Percentage of size by which an instance can differ
                         // from the substructure definition according to
                         // graph match transformation costs
   ULONG iterations;     // Number of SUBDUE iterations; if more than 1, then
                         // graph compressed with best sub between iterations
   double *log2Factorial;   // Cache array A[i] = lg(i!); grows as needed
   ULONG log2FactorialSize; // Size of log2Factorial array
   ULONG numPartitions;  // Number of partitions used by parallel SUBDUE
   BOOLEAN recursion;    // If TRUE, recursive graph grammar subs allowed
   BOOLEAN variables;    // If TRUE, variable vertices allowed
   BOOLEAN relations;    // If TRUE, relations between vertices allowed
   BOOLEAN incremental;  // If TRUE, data is processed incrementally
   BOOLEAN compress;     // If TRUE, write compressed graph to file
   IncrementList *incrementList;   // Set of increments
   InstanceVertexList *vertexList; // List of avl trees containing
                                   // instance vertices
   ULONG posGraphVertexListSize;
   ULONG posGraphEdgeListSize;
   ULONG posGraphSize;
   ULONG negGraphVertexListSize;
   ULONG negGraphEdgeListSize;
   ULONG negGraphSize;
} Parameters;

// FUNCTIONS

// graphops.c

void ReadInputFile(Parameters *);
ULONG *AddVertexIndex(ULONG *, ULONG, ULONG);
void ReadPredefinedSubsFile(Parameters *);
SubdueGraph **ReadSubGraphsFromFile(const char *, const char *, ULONG *, Parameters *);
SubdueGraph *ReadGraph(char *, LabelList *, BOOLEAN);
void ReadVertex(SubdueGraph *, FILE *, LabelList *, ULONG *, ULONG *, ULONG);
void AddVertex(SubdueGraph *, ULONG, ULONG *);
void ReadEdge(SubdueGraph *, FILE *, LabelList *, ULONG *, ULONG *, BOOLEAN,
              ULONG);
void AddEdge(SubdueGraph *, ULONG, ULONG, BOOLEAN, ULONG, ULONG *, BOOLEAN);
void StoreEdge(Edge *, ULONG, ULONG, ULONG, ULONG, BOOLEAN, BOOLEAN);
void AddEdgeToVertices(SubdueGraph *, ULONG);
int ReadToken(char *, FILE *, ULONG *);
ULONG ReadLabel(FILE *, LabelList *, ULONG *);
ULONG ReadInteger(FILE *, ULONG *);
SubdueGraph *AllocateGraph(ULONG, ULONG);
SubdueGraph *CopyGraph(SubdueGraph *);
void FreeGraph(SubdueGraph *);
void PrintGraph(SubdueGraph *, LabelList *);
void PrintVertex(SubdueGraph *, ULONG, LabelList *);
void PrintEdge(SubdueGraph *, ULONG, LabelList *);
void WriteGraphToFile(FILE *, SubdueGraph *, LabelList *, ULONG, ULONG, ULONG,
                      BOOLEAN);

// utility.c

void OutOfMemoryError(const char *context);
void PrintBoolean(BOOLEAN boolean);

// labels.c

LabelList *AllocateLabelList(void);
ULONG StoreLabel(Label *, LabelList *);
ULONG GetLabelIndex(Label *, LabelList *);
ULONG SubLabelNumber(ULONG, LabelList *);
double LabelMatchFactor(ULONG, ULONG, LabelList *);
void PrintLabel(ULONG, LabelList *);
void PrintLabelList(LabelList *);
void FreeLabelList(LabelList *);
void WriteLabelToFile(FILE *, ULONG, LabelList *, BOOLEAN);

// main.c

int subdue_main(int argc, char **argv);
Parameters *GetParameters(int argc, char *argv[]);
void PrintParameters(Parameters *parameters);
void FreeParameters(Parameters *parameters);

// user function

void clear_quotes(char* src, char* dst);
int CycleDetector(SubdueGraph *graph, LabelList *labelList);
vector<Graph*> open_subdue_file(GLOBAL* gP, char* filename);

#endif // __INPUT_SUBDUE_H__




