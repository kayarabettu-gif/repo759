#include "vscale.cuh"

// Element-wise product b[i] = a[i] * b[i]; each thread handles at most one i.
__global__ void vscale(const float *a, float *b, unsigned int n) {
    const unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        b[i] = a[i] * b[i];
    }
}
