#include "getsmallestcellgpu.cuh"
#include <iostream>

__global__ void find_min(PDistCell *secVec, vector<PDistCellMin> *min)
{
    /* insert code to calculate the index properly using blockIdx.x, blockDim.x, threadIdx.x */
	unsigned int i = blockDim.x * blockIdx.x + threadIdx.x; // Y - ID
	unsigned int j = blockDim.y * blockIdx.y + threadIdx.y; // X - ID

	float dist = secVec[i][j].dist;

	if (dist < min.dist) {
		mins.clear();
		mins.push_back(secVec[i][j]);
	}
}