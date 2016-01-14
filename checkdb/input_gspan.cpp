

/*
The gSpan format is also called the DIMACS format.
*/

#include "input_gspan.h"

namespace GSPAN {

template <class T, class Iterator>
void tokenize (const char *str, Iterator iterator)
{
	std::istrstream is (str, std::strlen(str));
	std::copy (std::istream_iterator <T> (is), std::istream_iterator <T> (), iterator);
}

void Graph::buildEdge ()
{
	char buf[512];
	std::map <std::string, unsigned int> tmp;

	unsigned int id = 0;
	for (int from = 0; from < (int)size (); ++from) {
		for (Vertex::edge_iterator it = (*this)[from].edge.begin ();
				it != (*this)[from].edge.end (); ++it)
		{
			if (directed || from <= it->to)
				std::sprintf (buf, "%d %d %d", from, it->to, it->elabel);
			else
				std::sprintf (buf, "%d %d %d", it->to, from, it->elabel);

			// Assign unique id's for the edges.
			if (tmp.find (buf) == tmp.end()) {
				it->id = id;
				tmp[buf] = id;
				++id;
			} else {
				it->id = tmp[buf];
			}
		}
	}

	edge_size_ = id;
}

std::istream &Graph::read (std::istream &is, vector<CHECKDB::Graph*> &DB, std::string * gName)
{
	std::vector <std::string> result;
	char line[1024];

	clear ();

	while (is.peek() != 't') {

	  if (is.peek() == EOF)
	    return is;

	  is.getline (line, 1024);
	  //	  std::cerr << "error: graph entry not starting with 't' line Graph::read:" <<__LINE__ << std::endl;
	  //	  exit(1);
	}
	if (! is.getline (line, 1024)) {
	  std::cerr << "error: no graph entry here at all Graph::read:" <<__LINE__  << std::endl;
	  exit(1);	  
	}
	if (line[1] != ' ' || line[2] != '#') {
	  std::cerr << "error: illegal graph identifier \""<<line<<"\""  << std::endl;
          std::cerr << "       needs DIMACS format: \"t # id\" (Graph::read:" <<__LINE__<<")"  << std::endl;
	  exit(1);	  
	}
	std::string graph_name(line+4);

	if (gName != NULL)
	  *gName = graph_name;
	  
	  		
	// Allocation of a graph
	CHECKDB::Graph *G = new CHECKDB::Graph(1);
	G->set_label((char*)graph_name.c_str());

	while (is.peek() != 't') {


		if (! is.getline (line, 1024))
			break;

		//	  std::cout <<line<<std::endl;

		result.clear();
		tokenize<std::string>(line, std::back_inserter (result));

		if (result.empty()) {
			// do nothing
		} else if (result[0] == "t") {

		  std::cerr << "error: should not arrive here: Graph::read:"<<__LINE__ << std::endl;
		  exit(1);

			if (! empty()) { // use as delimiter

			} else {//
			}
		} else if (result[0] == "v" && result.size() >= 3) { // vertices
			unsigned int id    = atoi (result[1].c_str());
			this->resize (id + 1);
			(*this)[id].label = atoi (result[2].c_str());
			G->add_node((*this)[id].label);
		} else if (result[0] == "e" && result.size() >= 4) { // edges
			int from   = atoi (result[1].c_str());
			int to     = atoi (result[2].c_str());
			int elabel = atoi (result[3].c_str());

			if ((int)size () <= from || (int)size () <= to) {
			  std::cerr << "Format Error:  define vertex lists before edges (graph '"<<graph_name<<"', ["<<from<<", "<<to<<", "<<elabel<<"]" << std::endl;
				exit (-1);
			}

			(*this)[from].push (from, to, elabel);
			if (directed == false) // i.e.: no duplicate edges allowed!
				(*this)[to].push (to, from, elabel);
			
			char buffer[20];
			sprintf(buffer, "%d", elabel);
			//cout << "DEBUG xxx : " << elabel << endl;
			G->add_edge(from, to, 1.0, buffer, directed);
		}
	}

	buildEdge ();
	
	DB.push_back(G);

	return is;
}


}


/**************************************************\
Open a graph DB in the gSpan (DIMACS) format.
Notes:
- Manage properly multiple graphs in the DB.
\*************************************************/
vector<CHECKDB::Graph*> open_gspan_file(GLOBAL* gP, char* filename) {

	ifstream myfile;
	myfile.open(filename);
	GSPAN::Graph g;
	//Graph g(directed);
	std::string gName;
	unsigned int gNameInt;
	std::vector < GSPAN::Graph > TRANS; // the graph dataset (gSpan format)
	vector<CHECKDB::Graph*> DB; // the graph dataset (local format)
	while (true) {
		g.read (myfile, DB, &gName);
		gNameInt = strtoul(gName.c_str(),NULL,10);
		if (g.empty()) break;
		TRANS.push_back (g);
		//cout << "Graph " << gNameInt << ": " << g.vertex_size() << " nodes, " << g.edge_size() << " edges." << endl;
		//DB[DB.size()-1]->print();
	}
	
	return DB;
}
