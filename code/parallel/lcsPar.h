#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <limits.h>
#include <omp.h>
#include <string.h>
#include <time.h>
#include "../sequential/lcsSeq.h"
#include "../utils/ctimer.h"
#include "../utils/generator.h"
#include "../utils/input.h"

int getInput(int* argc, char* argv, char * inputString, char** stringArray, int* MAX_SEQUENCES);
int run(int argc,char *argv[]);

