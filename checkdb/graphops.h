#ifndef __GRAPHOPS_H__
#define __GRAPHOPS_H__

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "routines.h"
#include "global.h"
#include "graph.h"
#include <string>
#include <sstream>

using namespace CHECKDB;

// FUNCTIONS
//int is_symmetrical(Graph *G);
//int get_num_edges(Graph *G);
char* find_field(GLOBAL *gP, char* category);
char* find_category(GLOBAL *gP, char* field);
int find_category_in_table(char* category, int _num_nodes, Graph *G);
int translated_dico_index(GLOBAL *gP, Graph *G, int int_index);
int translated_dico_index_edge(GLOBAL *gP, Graph *G, int int_index);
char* str_replace(char* str, const char* search, const char* replace);
int find_dico_entry(GLOBAL *gP, char* name);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif // __GRAPHOPS_H__
