#ifndef __OUTPUT_GASTON_H__
#define __OUTPUT_GASTON_H__

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
void write_gaston_file(GLOBAL* gP, vector<Graph*> DB);
int find_edge_index(Graph* G, int i, int j);

#endif // __OUTPUT_GASTON_H__
