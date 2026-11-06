#include <cstdio>
#include <cuda_runtime.h>

// Each thread computes the factorial of (threadIdx.x + 1) and prints it.
__global__ void factorialKernel() {
    const int a = static_cast<int>(threadIdx.x) + 1;
    int b = 1;
    for (int i = 2; i <= a; ++i) {
        b *= i;
    }
    std::printf("%d!=%d\n", a, b);
}

int main() {
    factorialKernel<<<1, 8>>>();
    cudaDeviceSynchronize();
    return 0;
}
