#ifndef __INPUT_PAJEK_H__
#define __INPUT_PAJEK_H__

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "routines.h"
#include "global.h"
#include "graphops.h"
#include "memory.h"

// FUNCTIONS
int get_num_nodes(FILE* fich);
void open_net(FILE* fich, Graph* G);
//float ** open_new_net(FILE* fich, int* __num_nodos, SNODE **__node_table);
//void save_net(Graph *G, FILE* fich, int type);
vector<Graph*> open_pajek_file(GLOBAL* gP, FILE* fin);

#endif // __INPUT_PAJEK_H__
