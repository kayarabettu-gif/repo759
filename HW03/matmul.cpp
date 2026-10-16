#include "matmul.h"

// C = A * B for n x n row-major matrices, parallelized over rows of C.
// Uses the cache-friendly i-k-j order of mmul2: the inner loop streams
// through contiguous rows of B and C. Each thread owns whole rows of C,
// so no synchronization is needed.
void mmul(const float* A, const float* B, float* C, const std::size_t n) {
#pragma omp parallel for
    for (std::size_t i = 0; i < n; i++) {
        float* Ci = C + i * n;
        for (std::size_t j = 0; j < n; j++) Ci[j] = 0.0f;

        for (std::size_t k = 0; k < n; k++) {
            const float a = A[i * n + k];
            const float* Bk = B + k * n;
            for (std::size_t j = 0; j < n; j++) Ci[j] += a * Bk[j];
        }
    }
}
