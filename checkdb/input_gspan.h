#ifndef __INPUT_GSPAN_H__
#define __INPUT_GSPAN_H__

// INCLUDES
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <assert.h>
#include <string>
#include <iterator>
#include <strstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "global.h"
#include "graph.h"

namespace GSPAN {

template <class T> inline void _swap (T &x, T &y) { T z = x; x = y; y = z; }

struct Edge {
	int from;
	int to;
	int elabel;
	unsigned int id;
	Edge(): from(0), to(0), elabel(0), id(0) {};
};

class Vertex
{
public:
	typedef std::vector<Edge>::const_iterator const_edge_iterator;
	typedef std::vector<Edge>::iterator edge_iterator;

	int label;
	std::vector<Edge> edge;

	void push (int from, int to, int elabel)
	{
		edge.resize (edge.size()+1);
		edge[edge.size()-1].from = from;
		edge[edge.size()-1].to = to;
		edge[edge.size()-1].elabel = elabel;
		return;
	}
	Vertex() {
    label = -1;
	}
	Vertex(const Vertex & v) {
		label = v.label;
		edge.resize(v.edge.size());
		for (unsigned int i = 0; i < v.edge.size(); ++i) {
			edge[i].from = v.edge[i].from;
			edge[i].to = v.edge[i].to;
			edge[i].elabel = v.edge[i].elabel;
	    edge[i].id = v.edge[i].id;
		}
	}
	Vertex& operator=(const Vertex & v) {
		if (&v != this) {
			label = v.label;
			edge.resize(v.edge.size());
			for (unsigned int i = 0; i < v.edge.size(); ++i) {
				edge[i].from = v.edge[i].from;
				edge[i].to = v.edge[i].to;
				edge[i].elabel = v.edge[i].elabel;
	  		edge[i].id = v.edge[i].id;
			}
		}
		return *this;
	}
};

class Graph: public std::vector<Vertex> {
private:
	unsigned int edge_size_;
public:
	typedef std::vector<Vertex>::const_iterator const_vertex_iterator;
	typedef std::vector<Vertex>::iterator vertex_iterator;

	Graph (bool _directed)
	{
		directed = _directed;
	};
	Graph(const Graph & graph) {
		directed = graph.directed;
		edge_size_ = graph.edge_size_;
		resize(graph.size());
		std::copy(graph.begin(), graph.end(), begin());
	}
	Graph& operator=(const Graph & graph) {
		if (this != &graph) {
			directed = graph.directed;
			edge_size_ = graph.edge_size_;
			resize(graph.size());
			std::copy(graph.begin(), graph.end(), begin());
		}
		return *this;
	}
	bool directed;

	unsigned int edge_size ()  const { return edge_size_; }
	unsigned int vertex_size () const { return (unsigned int)size(); } // wrapper
	void buildEdge ();
	std::istream &read (std::istream &, vector<CHECKDB::Graph*> &DB, std::string * gName = NULL); // read
	void check (void);

	Graph* copyGraph() const {
		Graph * graph = new Graph(directed);
		graph->edge_size_ = edge_size_;
		unsigned int i;
		for (i = 0; i < vertex_size(); ++i) {
			graph->push_back(at(i));
		}
		return graph;
	}
  
	Graph(): edge_size_(0), directed(false) {};
};


}

vector<CHECKDB::Graph*> open_gspan_file(GLOBAL* gP, char* filename);

#endif // __INPUT_GSPAN_H__
