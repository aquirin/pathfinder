#ifndef __MAIN_H__
#define __MAIN_H__

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "routines.h"
#include "global.h"
#include "graphops.h"
#include "memory.h"
#include "graph.h"

#include "input_pajek.h"
#include "input_subdue.h"
#include "input_gaston.h"
#include "input_gspan.h"
#include "input_ffsm.h"
#include "input_prolog.h"

#include "output_pajek.h"
#include "output_subdue.h"
#include "output_dot.h"
#include "output_plain.h"
#include "output_human.h"
#include "output_gaston.h"
#include "output_jxplore.h"
#include "output_gspan.h"


// FUNCTIONS
void Input(int argc, char *argv[]);
static int compedges_min(const void *_e1, const void *_e2);
static int compedges_max(const void *_e1, const void *_e2);
void load_database(GLOBAL *gP);
int is_sfl(GLOBAL *gP, Graph *G, int i, int j);
void load_scored_database(GLOBAL *gP);
float find_score(GLOBAL *gP, char* f1, char* c1, char* f2, char *c2);
int cwl_categ(float S1, float S2, float D1, float D2);
void compute_pesos_predecessors(GLOBAL *gP, Graph *G);
int floyd_warshall(GLOBAL *gP, Graph *G, int type);
void compute_ending(GLOBAL *gP, Graph *G);
void compute_table_size_sp(GLOBAL *gP, Graph *G);
int is_ending(GLOBAL *gP, int n);
int is_isolated(GLOBAL *gP, int n);
int size_shortestpath(GLOBAL *gP, int i, int j);
int size_shortestpath_link(GLOBAL *gP, Graph *G, int i, int l1, int l2);
int measure_cen_i(GLOBAL *gP, int i);
int measure_cen_ij(GLOBAL *gP, Graph *G, int i, int j);
int measure_bor_i(GLOBAL *gP, Graph *G, int i);
int measure_bor_ij(GLOBAL *gP, Graph *G, int i, int j);
int loop_detector(GLOBAL *gP, Graph *G, int _show_cycles);
void print_sorted_edge(GLOBAL *gP, Graph *G);
void load_dico_database(GLOBAL *gP, char* filename);
int find_dico_entry(GLOBAL *gP, char* name);
vector<Graph*> open_generic_file(GLOBAL *gP, char* filename, int type);
void write_generic_file(GLOBAL *gP, vector<Graph*> graph_db);
int main(int argc, char *argv[]);


#endif // __MAIN_H__




/*
Toolbox for scientograms.
See the help (-h)
*/

#include "global.h"

