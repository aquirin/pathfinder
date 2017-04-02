#ifndef __INPUT_GASTON_H__
#define __INPUT_GASTON_H__

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "routines.h"
#include "global.h"
#include "graphops.h"
#include "memory.h"
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


// misc.h
typedef unsigned int EdgeLabel; // combined node-edge label of the input file.
typedef unsigned int NodeLabel;
typedef unsigned int NodeId;
typedef unsigned int Depth; // unsigned int is more efficient than short, but requires more memory...
typedef unsigned int Tid;
typedef unsigned int Frequency;

extern Frequency gaston_minfreq;

#define NOTID ((Tid) -1)
#define NOEDGELABEL ((EdgeLabel) -1)
#define MAXEDGELABEL NOEDGELABEL
#define NONODELABEL ((NodeLabel) -1)
#define NODEPTH ((Depth) -1)
#define NOLEG (-1)
#define NONODE ((NodeId) -1)

// this macro can be used when push_back-ing large structures. In stead of first allocating a local
// variable and pushing this, one first pushes and defines a reference to the space in the vector.
// This avoids re-allocation.
#define vector_push_back(_type,_vector,_var) (_vector).push_back ( _type () ); _type &_var = (_vector).back ();

// can be used to obtain a type when inserting into a map
#define map_insert_pair(_type) typedef typeof(_type) T##_type; pair<T##_type::iterator,bool>

// legoccurrence.h
typedef unsigned int OccurrenceId;

struct LegOccurrence {
  Tid tid;
  OccurrenceId occurrenceid;
  NodeId tonodeid, fromnodeid;

  LegOccurrence ( Tid tid, OccurrenceId occurrenceid, NodeId tonodeid, NodeId fromnodeid ):
    tid ( tid ), occurrenceid ( occurrenceid ), tonodeid ( tonodeid ), fromnodeid ( fromnodeid ) { }
  LegOccurrence () {}

  friend ostream &operator<< ( ostream &stream, LegOccurrence &occ );
};

struct LegOccurrences;
typedef LegOccurrences *LegOccurrencesPtr;

struct LegOccurrences {
  vector<LegOccurrence> elements;
  LegOccurrencesPtr parent;
  int number;
  Frequency frequency;
  Frequency selfjoin;
  short unsigned int maxdegree;
  LegOccurrences () : frequency ( 0 ), selfjoin ( 0 ) { }
};



// gaston_database.h
typedef short InputEdgeLabel;
typedef short InputNodeLabel;
typedef short InputNodeId;
typedef unsigned int CombinedInputLabel;
#define combineInputLabels(label1,label2,label3) (label1 | ( ((unsigned int) label2 ) << 16 ) | ( ( (unsigned int) label3 ) << 24 ) )
// maximum 255 node labels for now.


#define NOINPUTEDGELABEL ((InputEdgeLabel) -1)
#define NOINPUTNODELABEL ((InputNodeLabel) -1)

template<class T>
class pvector {
public:
  T *array;
  int _size;
  pvector<T> ( T *array, int _size ): array ( array ), _size ( _size ) { }
  pvector<T> () { }
  inline int size () const { return _size; }
  void resize ( int s ) { _size = s; }
  void clear () { _size = 0; } // cannot remove allocation, as we are not managing that memory here 
  T &operator[] ( int i ) { return array[i]; }
};

struct DatabaseTreeEdge {
  EdgeLabel edgelabel;
  NodeId tonode;

  DatabaseTreeEdge () { }

  friend ostream &operator<< ( ostream &stream, DatabaseTreeEdge &databasetreeedge );
};

struct DatabaseTreeNode {
  NodeLabel nodelabel;
  bool incycle;
  pvector<DatabaseTreeEdge> edges;

  DatabaseTreeNode () { }

  friend ostream &operator<< ( ostream &stream, DatabaseTreeNode &databasetreenode );
};

struct DatabaseTree {
  Tid tid;
  vector<DatabaseTreeNode> nodes;

  DatabaseTreeEdge *edges;

  DatabaseTree ( Tid tid ): tid ( tid ) { }
  DatabaseTree () { }
  
  friend ostream &operator<< ( ostream &stream, DatabaseTree &databasetree );
};

typedef DatabaseTree *DatabaseTreePtr;

struct DatabaseNodeLabel {
  InputNodeLabel inputlabel;
  Frequency frequency;
  Tid lasttid;

  LegOccurrences occurrences;
  vector<EdgeLabel> frequentedgelabels;

  DatabaseNodeLabel (): frequency ( 1 ) { }
};

struct DatabaseEdgeLabel {
  InputEdgeLabel inputedgelabel;
  NodeLabel tonodelabel, fromnodelabel; 
  EdgeLabel edgelabel; // the (order) edge label to which this entry corresponds during the search
  Frequency frequency;
  Tid lasttid;

  DatabaseEdgeLabel (): frequency ( 1 ) { }
};

class Database {
  public:
    Database () { }
    vector<DatabaseTreePtr> trees;
    vector<DatabaseNodeLabel> nodelabels;
    vector<DatabaseEdgeLabel> edgelabels;
    map<InputNodeLabel,NodeLabel> nodelabelmap;
    map<CombinedInputLabel,EdgeLabel> edgelabelmap;
    vector<EdgeLabel> edgelabelsindexes; // given an edge label, returns the index of the element in edgelabels in which
    EdgeLabel frequentEdgeLabelSize () const { return edgelabelsindexes.size (); }
                                         // all information about this edge can be found. Used during the search,
					 // only frequent edge label, node label pairs are stored.

     // NOTE! In the input file, the nodes MUST be listed in pre-order.

     // "read" reads the input file, and determines the frequency counts for node labels and (combined) edge labels
    void read ( FILE *input );

     // after "read", determines the frequency of edges, using DatabaseNodeLabel's edgelasttid/edgelabelfrequency
    void edgecount ();

     // after "edgecount",
     // - removes infrequent data
     // - cleans up the datastructures used until now for counting frequencies
     // - changes the edge label order to optimise the search, fills the gaston_database with order numbers in stead of
     //   the numbers assigned in the previous phases; fills edgelabelsindexes.
    void reorder ();

    void printTrees ();
    ~Database ();
  private:
    void readTree ( FILE *input, Tid tid );
    void determineCycledNodes ( DatabaseTreePtr tree, vector<unsigned int> &nodestack, vector<bool> &visited1, vector<bool> &visited2 );
};

extern Database gaston_database;


// FUNCTIONS
vector<Graph*> open_gaston_file(GLOBAL* gP, FILE* fin);

#endif // __INPUT_GASTON_H__
