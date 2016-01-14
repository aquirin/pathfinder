#ifndef __OUTPUT_PAJEK_H__
#define __OUTPUT_PAJEK_H__

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
void write_pajek_file(GLOBAL* gP, vector<Graph*> DB);

#endif // __OUTPUT_PAJEK_H__
