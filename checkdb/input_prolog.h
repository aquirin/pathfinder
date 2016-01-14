#ifndef __INPUT_PROLOG_H__
#define __INPUT_PROLOG_H__

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "routines.h"
#include "global.h"
#include "graphops.h"
#include "memory.h"
#include <map>
#include <iostream>
#include <vector>

using namespace std;


// FUNCTIONS
vector<Graph*> open_prolog_file(GLOBAL* gP, FILE* fin);

#endif // __INPUT_PROLOG_H__
