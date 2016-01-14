/*
General operations on graphs.

*/

#include "graphops.h"


/***
Return 1 if the matrix encoded in weights is symmetrical, 0 if not.
***/
/*int is_symmetrical(Graph *G)
{
  int i, j;
  for(i=0; i<G->nodes(); i++)
    for(j=i+1; j<G->nodes(); j++)
    {
      if(G->weight(i,j) != G->weight(j,i))
        return 0;
    }
  return 1;
}*/

// Compute the number of edges in the graph
// Is the matrix is symmetrical, each link is an edge, so M[i][j] and
// M[j][i] is counted once, but only if not on the diagonal.
/*int get_num_edges(Graph *G)
{
  int ne = 0;
  int is_sim = is_symmetrical(G);
  int i, j;
  for(i=0; i<G->nodes(); i++)
	for(j=0; j<G->nodes(); j++)
	{
		if(G->weight(i,j) != 0.0)
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
  return ne;
}*/

// Find the first field corresponding to the given category.
// Return NULL if nothing was found.
// WARNING! Use strdup(find_field(...)) to copy the memory !
char* find_field(GLOBAL *gP, char* category)
{
	int i;
	for(i=0;i<gP->num_categories;i++)
	{
		if(!strcmp(category, gP->field_table[i].category))
			return gP->field_table[i].field;
	}
	return NULL;
}

// Find the category corresponding to the given field.
// Return NULL if nothing was found.
char* find_category(GLOBAL *gP, char* field)
{
	int i;
	for(i=0;i<gP->num_categories;i++)
	{
		if(!strcmp(field, gP->field_table[i].field))
			return gP->field_table[i].category;
	}
	return NULL;
}

// Find the index of the node corresponding to the given category in the given node_table.
// Return -1 if nothing was found.
int find_category_in_table(char* category, int _num_nodes, Graph *G)
{
	int i;
	for(i=0;i<G->nodes();i++)
	{
		if(!strcmp(category, G->get_node(i).label))
			return i;
	}
	return -1;
}

// Translate the internal index to the dictionnary index if a dictionnary is provided (-t)
// If not, return the given index.
// SHOULD BE ONLY USED FOR OUTPUT (inside a printf(), fprintf(), ...)
// The produced file will in ALL the cases be incompatible with checkdb (you will not
// be able to open it again without a beautiful segfault).
int translated_dico_index(GLOBAL *gP, Graph *G, int int_index)
{
	if(gP->num_dico_entries==0) return 1+int_index;
	else
	{
		char* name = G->get_node(int_index).label;
		if(name==NULL)
		{
			fprintf(stderr, "[WARNING] The node '%d' has not valid entry in node_table[] !\n", int_index);
			return -1;
		}
		return find_dico_entry(gP, name);
	}
}
// Translate the internal index to the dictionnary index if a dictionnary is provided (-t)
// If not, return the given index.
// SHOULD BE ONLY USED FOR OUTPUT (inside a printf(), fprintf(), ...)
// The produced file will in ALL the cases be incompatible with checkdb (you will not
// be able to open it again without a beautiful segfault).
int translated_dico_index_edge(GLOBAL *gP, Graph *G, int int_index)
{
	if(gP->num_dico_entries==0) return 1+int_index;
	else
	{
		char* name = G->get_edge(int_index).label;
		if(name==NULL)
		{
			fprintf(stderr, "[WARNING] The edge '%d' has not valid entry in edge_table[] !\n", int_index);
			return -1;
		}
		return find_dico_entry(gP, name);
	}
}


// Replace, in str, all the "search" strings by the "replace" strings.
// Return a new allocated string.
char* str_replace(char* str, const char* search, const char* replace)
{
  char* pos;
  char* buffer = (char*)calloc(2*strlen(str),sizeof(char)), *save;
  int len_search = strlen(search);
  int len_replace = strlen(replace);
  save = buffer;
  while((pos=strstr(str, search)) != NULL)
  {
    strncpy(buffer, str, pos-str);
    buffer += pos-str;
    strncpy(buffer, replace, len_replace);
    buffer += len_replace;
    str = pos;
    str += len_search;
  }
  strcpy(buffer, str);
  return save;
}

// Find the index corresponding to the given node name
int find_dico_entry(GLOBAL *gP, char* name)
{
	int i;
	if(!name)
		return -1;
	for(i=0;i<gP->num_dico_entries;i++)
	{
		//printf("#%s#%s#\n", name, gP->dico_table[i].name);
		if(!strcmp(name, gP->dico_table[i].name))
			return gP->dico_table[i].index;
	}
	return -1;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
