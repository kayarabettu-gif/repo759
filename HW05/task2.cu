#include <cstdio>
#include <random>
#include <cuda_runtime.h>

// Each thread writes a * threadIdx.x + blockIdx.x into its own entry of dA.
__global__ void axyKernel(int *dA, int a) {
    const int x = static_cast<int>(threadIdx.x);
    const int y = static_cast<int>(blockIdx.x);
    const int idx = blockIdx.x * blockDim.x + threadIdx.x;
    dA[idx] = a * x + y;
}

int main() {
    constexpr int numBlocks = 2;
    constexpr int threadsPerBlock = 8;
    constexpr int N = numBlocks * threadsPerBlock;

    // Random integer a
    std::random_device entropySource;
    std::mt19937 generator(entropySource());
    std::uniform_int_distribution<int> dist(1, 10);
    const int a = dist(generator);

    int *dA = nullptr;
    cudaMalloc(reinterpret_cast<void **>(&dA), N * sizeof(int));

    axyKernel<<<numBlocks, threadsPerBlock>>>(dA, a);

    int hA[N];
    // cudaMemcpy is blocking, so it waits for the kernel to finish
    cudaMemcpy(hA, dA, N * sizeof(int), cudaMemcpyDeviceToHost);

    for (int i = 0; i < N; ++i) {
        std::printf("%d%s", hA[i], (i + 1 < N) ? " " : "\n");
    }

    cudaFree(dA);
    return 0;
}
