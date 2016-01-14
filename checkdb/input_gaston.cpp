#include "input_gaston.h"

Frequency gaston_minfreq = 1;
Database gaston_database;


void Database::read ( FILE *input ) {
  Tid tid;
  Tid tid2 = 0; 

  char array[100];
  char* ret = fgets ( array, 100, input );
  USE(ret);

  while ( !feof ( input ) ) {
    readTree ( input, tid2 );
    char* ret = fgets ( array, 100, input );
	USE(ret);
    tid2++;
  }
}

int readint ( FILE *input ) {
  char car = fgetc ( input );
  while ( car < '0' || car > '9' ) {
    if ( feof ( input ) )
      return -1;
    car = fgetc ( input );
  }
  int n = car - '0';
  car = fgetc ( input );
  while ( car >= '0' && car <= '9' ) {
    n = n * 10 + car - '0';
    car = fgetc ( input );
  }
  
  return n;
}

char readcommand ( FILE *input ) {
  char car = fgetc ( input );
  while ( car < 'a' || car > 'z' ) {
    if ( feof ( input ) )
      return -1;
    car = fgetc ( input );
  }
  return car;
}

void Database::readTree ( FILE *input, Tid tid ) {
  InputNodeLabel inputnodelabel;

  DatabaseTreePtr tree = new DatabaseTree ( tid );
  trees.push_back ( tree );

  char command;
  int dummy;
  int nodessize = 0, edgessize = 0;
  command = readcommand ( input );
  
  static vector<DatabaseTreeNode> nodes;
  static vector<vector<DatabaseTreeEdge> > edges;
  nodes.resize ( 0 );

  while ( command == 'v' ) {
    dummy = readint ( input );
    inputnodelabel = readint ( input );
    if ( dummy != nodessize ) {
      cerr << "Error reading input file - node number does not correspond to its position." << endl;
      exit ( 1 );
    }
    nodessize++;

    map_insert_pair ( nodelabelmap ) p = nodelabelmap.insert ( make_pair ( inputnodelabel, nodelabels.size () ) );
    if ( p.second ) {
      vector_push_back ( DatabaseNodeLabel, nodelabels, nodelabel );
      nodelabel.inputlabel = inputnodelabel;
      nodelabel.occurrences.parent = NULL;
      nodelabel.occurrences.number = 1;
      nodelabel.lasttid = tid;
    }
    else {
      DatabaseNodeLabel &nodelabel = nodelabels[p.first->second];
      if ( nodelabel.lasttid != tid )
        nodelabel.frequency++;
      nodelabel.lasttid = tid;
    }
    
    vector_push_back ( DatabaseTreeNode, nodes, node );
    node.nodelabel = p.first->second;
    node.incycle = false;

    command = readcommand ( input );
  }

  tree->nodes.reserve ( nodessize );
  if ( edges.size () < nodessize )
    edges.resize ( nodessize );
  for ( int i = 0; i < nodessize; i++ ) {
    edges[i].resize ( 0 );
    tree->nodes.push_back ( nodes[i] );
  }
  
  InputEdgeLabel inputedgelabel;
  InputNodeId nodeid1, nodeid2;

  while ( !feof ( input ) && command == 'e' ) {
    nodeid1 = readint ( input );
    nodeid2 = readint ( input );
    inputedgelabel = readint ( input );
    NodeLabel node2label = tree->nodes[nodeid2].nodelabel;
    NodeLabel node1label = tree->nodes[nodeid1].nodelabel;
    CombinedInputLabel combinedinputlabel;
    if ( node1label > node2label ) {
      NodeLabel temp = node1label;
      node1label = node2label;
      node2label = temp;
    }
    combinedinputlabel = combineInputLabels ( inputedgelabel, node1label, node2label );

    map_insert_pair ( edgelabelmap ) p = edgelabelmap.insert ( make_pair ( combinedinputlabel, edgelabels.size () ) );
    if ( p.second ) {
      vector_push_back ( DatabaseEdgeLabel, edgelabels, edgelabel );
      edgelabel.fromnodelabel = node1label;
      edgelabel.tonodelabel = node2label;
      edgelabel.inputedgelabel = inputedgelabel;
      edgelabel.lasttid = tid;
    }
    else {
      DatabaseEdgeLabel &edgelabel = edgelabels[p.first->second];
      if ( edgelabel.lasttid != tid )
        edgelabel.frequency++;
      edgelabel.lasttid = tid;
    }

    vector_push_back ( DatabaseTreeEdge, edges[nodeid1], edge );
    edge.edgelabel = p.first->second;
    edge.tonode = nodeid2;

    vector_push_back ( DatabaseTreeEdge, edges[nodeid2], edge2 );
    edge2.edgelabel = p.first->second;
    edge2.tonode = nodeid1;

    edgessize++;

    command = readcommand ( input );
  }

  tree->edges = new DatabaseTreeEdge[edgessize * 2];
  int pos = 0;
  for ( int i = 0; i < nodessize; i++ ) {
    int s = edges[i].size ();
    tree->nodes[i].edges._size = s;
    tree->nodes[i].edges.array = tree->edges + pos;
    for ( int j = 0; j < s; j++, pos++ ) {
      tree->edges[pos] = edges[i][j];
    }
  }
  
  static vector<int> nodestack;
  static vector<bool> visited1, visited2;
  nodestack.resize ( 0 );
  visited1.resize ( 0 );
  visited1.resize ( nodessize, false );
  visited2.resize ( 0 );
  visited2.resize ( nodessize, false );
  for ( int i = 0; i < nodessize; i++ ) {
    if ( !visited1[i] ) {
      nodestack.push_back ( i );
      visited1[i] = visited2[i] = true;
      determineCycledNodes ( tree, nodestack, visited1, visited2 );
      visited2[i] = false;
      nodestack.pop_back ();
    }
  }
}

void Database::determineCycledNodes ( DatabaseTreePtr tree, vector<int> &nodestack, vector<bool> &visited1, vector<bool> &visited2 ) {
  int node = nodestack.back ();
  pvector<DatabaseTreeEdge> &edges = tree->nodes[node].edges;

  for ( int i = 0; i < edges.size (); i++ ) {
    if ( !visited1[edges[i].tonode] ) {
      nodestack.push_back ( edges[i].tonode );
      visited1[edges[i].tonode] = visited2[edges[i].tonode] = true;
      determineCycledNodes ( tree, nodestack, visited1, visited2 );
      nodestack.pop_back ();
      visited2[edges[i].tonode] = false;
    }
    else {
      if ( visited2[edges[i].tonode] && ( nodestack.size () == 1 || nodestack[nodestack.size () - 2] != edges[i].tonode ) ) {
        int j = nodestack.size () - 1;
        while ( nodestack[j] != edges[i].tonode ) {
          tree->nodes[nodestack[j]].incycle = true;
          j--;
        }
        tree->nodes[nodestack[j]].incycle = true;
      }
    }
  }
}

void Database::edgecount () {
  for ( int i = 0; i < edgelabels.size (); i++ ) {
    if ( edgelabels[i].frequency >= gaston_minfreq ) {
      nodelabels[edgelabels[i].tonodelabel].frequentedgelabels.push_back ( i );
      if ( edgelabels[i].fromnodelabel != edgelabels[i].tonodelabel )
        nodelabels[edgelabels[i].fromnodelabel].frequentedgelabels.push_back ( i );
    }
  }
}

class EdgeLabelsIndexesSort:public std::binary_function<int,int,bool> {
    const vector<DatabaseEdgeLabel> &edgelabels;
  public:
    EdgeLabelsIndexesSort ( const vector<DatabaseEdgeLabel> &edgelabels ) : edgelabels ( edgelabels ) { }
    bool operator () ( int a, int b ) const {
      return edgelabels[a].frequency < edgelabels[b].frequency;
    }
};

bool operator< ( const DatabaseTreeEdge &a, const DatabaseTreeEdge &b ) {
  return a.edgelabel < b.edgelabel;
}

void Database::reorder () {
  edgelabelsindexes.reserve ( edgelabels.size () );

  for ( int i = 0; i < edgelabels.size (); i++ ) {
    if ( edgelabels[i].frequency >= gaston_minfreq )
      edgelabelsindexes.push_back ( i );
  }

  std::sort ( edgelabelsindexes.begin (), edgelabelsindexes.end (), EdgeLabelsIndexesSort ( edgelabels ) );
   // however, this does not seem to make a lot of difference on the databases that we tried.
   // We don't know yet why - in similar other algorithms it seemed a good idea...

  for ( int i = 0; i < edgelabelsindexes.size (); i++ ) {
    edgelabels[edgelabelsindexes[i]].edgelabel = i; // fill in the final edge labels
#ifdef DEBUG
    DatabaseEdgeLabel &label = edgelabels[edgelabelsindexes[i]];
    cout << (int) nodelabels[label.tonodelabel].inputlabel 
         << "[" << (int) label.inputedgelabel << "]" 
	 << (int) nodelabels[label.fromnodelabel].inputlabel <<"-->" << i <<endl;
#endif
  }

  for ( Tid i = 0; i < trees.size (); i++ ) {
    DatabaseTree &tree = * (trees[i]);
    for ( NodeId j = 0; j < tree.nodes.size (); j++ ) {
      DatabaseTreeNode &node = tree.nodes[j];
      if ( nodelabels[node.nodelabel].frequency >= gaston_minfreq ) {
        DatabaseNodeLabel &nodelabel = nodelabels[node.nodelabel];
	nodelabel.occurrences.elements.push_back (
          LegOccurrence (
            tree.tid,
            (OccurrenceId) nodelabel.occurrences.elements.size (),
            j,
            NONODE
          )
        );
        int k = 0;
	for ( int l = 0; l < node.edges.size (); l++ ) {
          EdgeLabel lab = node.edges[l].edgelabel;
	  if ( edgelabels[lab].frequency >= gaston_minfreq ) {
	    node.edges[k].edgelabel = edgelabels[node.edges[l].edgelabel].edgelabel; // translate old into new edge labels
	    node.edges[k].tonode = node.edges[l].tonode;
	    k++;
	  }
	}
	node.edges.resize ( k );
      }
      else
        node.edges.clear ();
    }
  }
}

void Database::printTrees () {
  for ( int i = 0; i < trees.size (); i++ )
    cout << trees[i];
}

Database::~Database () {
  for ( int i = 0; i < trees.size (); i++ )
    delete trees[i];
}


/**************************************************\
Open a graph DB in the Gaston format.
Notes:
- Manage properly multiple graphs in the DB.
\*************************************************/

vector<Graph*> open_gaston_file(GLOBAL* gP, FILE* fin) {

	gaston_database.read ( fin );
	fclose(fin);
	
	/*printf("nodelabels.size: %d\n", gaston_database.nodelabels.size());
	printf("\n");
	
	for(int i=0; i<gaston_database.nodelabels.size(); i++) {
		printf("nodelabels %d: %d freq:%d\n", i, gaston_database.nodelabels[i].inputlabel, gaston_database.nodelabels[i].frequency);
	}
	printf("\n");
	
	for(int i=0; i<gaston_database.edgelabels.size(); i++) {
		printf("edgelabels %d: %d-%d %d\n", i, gaston_database.edgelabels[i].inputedgelabel, gaston_database.edgelabels[i].fromnodelabel, gaston_database.edgelabels[i].tonodelabel);
	}
	printf("\n");*/
	
	/*for(int t=0; t<gaston_database.trees.size(); t++) {
		for(int n=0; n<gaston_database.trees[t]->nodes.size(); n++) {
			int nlabel = gaston_database.trees[t]->nodes[n].nodelabel;
			for(int e=0; e<gaston_database.trees[t]->nodes[n].edges.size(); e++) {
				EdgeLabel edgelabel = gaston_database.trees[t]->nodes[n].edges[e].edgelabel;
				NodeId tonode = gaston_database.trees[t]->nodes[n].edges[e].tonode;
				int tonlabel = gaston_database.trees[t]->nodes[tonode].nodelabel;
				printf("Tree %d, Edge #%d L%d (#%d L%d) - (#%d L%d)\n", t, e, edgelabel, n, nlabel, tonode, tonlabel);
				// Note: the edge label is not the same one as the GAS file: numbers are reaffected.
			}
		}
	}*/
	
	// Start the conversion
	
	vector<Graph*> DB;
	char buffer[100];
	
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
	}
	
	return DB;
}

