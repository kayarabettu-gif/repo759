#include "convolution.h"

// Image value at (i, j), padded as in HW02:
// inside the image -> pixel, edge (one index out) -> 1, corner (both out) -> 0.
static float pixel(const float* image, long n, long i, long j) {
    const bool rowIn = 0 <= i && i < n;
    const bool colIn = 0 <= j && j < n;
    if (rowIn && colIn) return image[i * n + j];
    return (rowIn || colIn) ? 1.0f : 0.0f;
}

// output = image (n x n) convolved with mask (m x m, m odd).
// Every output pixel is independent, so the (x, y) loops are split across threads.
void convolve(const float* image, float* output, std::size_t n, const float* mask, std::size_t m) {
    const long N = static_cast<long>(n);
    const long M = static_cast<long>(m);
    const long r = (M - 1) / 2;  // mask radius

#pragma omp parallel for collapse(2)
    for (long x = 0; x < N; x++) {
        for (long y = 0; y < N; y++) {
            float sum = 0.0f;
            for (long i = 0; i < M; i++)
                for (long j = 0; j < M; j++)
                    sum += mask[i * M + j] * pixel(image, N, x + i - r, y + j - r);
            output[x * N + y] = sum;
        }
    }
}
