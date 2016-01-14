#ifndef __OUTPUT_DOT_H__
#define __OUTPUT_DOT_H__

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
void write_dot_file(GLOBAL* gP, vector<Graph*> DB);

#endif // __OUTPUT_DOT_H__
