#ifndef __OUTPUT_SUBDUE_H__
#define __OUTPUT_SUBDUE_H__

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
void write_subdue_file(GLOBAL* gP, vector<Graph*> DB);
void print_sorted_edge(GLOBAL *gP, Graph *G, FILE *fout);

#endif // __OUTPUT_SUBDUE_H__
