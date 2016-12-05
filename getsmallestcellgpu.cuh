#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>


/* experiment with N */
/* how large can it be? */
#define N (2048*2048)
#define THREADS_PER_BLOCK 512

__global__ void vector_add(int *a, int *b, int *c);
int parallelize();