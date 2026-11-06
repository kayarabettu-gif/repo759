#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>
#include <cuda_runtime.h>

#include "vscale.cuh"

#define CUDA_CHECK(call)                                                   \
    do {                                                                   \
        cudaError_t err_ = (call);                                         \
        if (err_ != cudaSuccess) {                                         \
            std::fprintf(stderr, "CUDA error %s at %s:%d\n",               \
                         cudaGetErrorString(err_), __FILE__, __LINE__);    \
            std::exit(EXIT_FAILURE);                                       \
        }                                                                  \
    } while (0)

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " n [threads_per_block]\n";
        return EXIT_FAILURE;
    }

    const unsigned int n =
        static_cast<unsigned int>(std::strtoul(argv[1], nullptr, 10));
    // Default is 512 threads per block as required; an optional second
    // argument (e.g. 16) is used only for the scaling comparison in part c.
    const unsigned int threadsPerBlock =
        (argc > 2) ? static_cast<unsigned int>(std::strtoul(argv[2], nullptr, 10))
                   : 512u;

    if (n == 0 || threadsPerBlock == 0 || threadsPerBlock > 1024) {
        std::cerr << "n must be positive and threads_per_block in [1, 1024]\n";
        return EXIT_FAILURE;
    }

    const std::size_t bytes = static_cast<std::size_t>(n) * sizeof(float);

    // Host arrays filled with random values
    float *hA = new float[n];
    float *hB = new float[n];

    std::random_device entropySource;
    std::mt19937 generator(entropySource());
    std::uniform_real_distribution<float> distA(-10.0f, 10.0f);
    std::uniform_real_distribution<float> distB(0.0f, 1.0f);
    for (unsigned int i = 0; i < n; ++i) {
        hA[i] = distA(generator);
        hB[i] = distB(generator);
    }

    // Device arrays
    float *dA = nullptr;
    float *dB = nullptr;
    CUDA_CHECK(cudaMalloc(reinterpret_cast<void **>(&dA), bytes));
    CUDA_CHECK(cudaMalloc(reinterpret_cast<void **>(&dB), bytes));
    CUDA_CHECK(cudaMemcpy(dA, hA, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(dB, hB, bytes, cudaMemcpyHostToDevice));

    const unsigned int numBlocks = (n + threadsPerBlock - 1) / threadsPerBlock;

    // Warm-up launch with n = 0: does no work, but forces lazy module loading
    // so that one-time overhead is not counted in the timed launch.
    vscale<<<1, 1>>>(dA, dB, 0);
    CUDA_CHECK(cudaDeviceSynchronize());

    cudaEvent_t start;
    cudaEvent_t stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    CUDA_CHECK(cudaEventRecord(start));
    vscale<<<numBlocks, threadsPerBlock>>>(dA, dB, n);
    CUDA_CHECK(cudaEventRecord(stop));
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaEventSynchronize(stop));

    float ms = 0.0f;
    CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));

    CUDA_CHECK(cudaMemcpy(hB, dB, bytes, cudaMemcpyDeviceToHost));

    std::cout << ms << "\n";
    std::cout << hB[0] << "\n";
    std::cout << hB[n - 1] << "\n";

    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));
    CUDA_CHECK(cudaFree(dA));
    CUDA_CHECK(cudaFree(dB));
    delete[] hA;
    delete[] hB;
    return 0;
}
