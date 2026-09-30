#include "convolution.h"

// Returns the image value at (i, j), handling positions outside the image.
// Inside the image     -> the real pixel
// Outside on a corner  -> 0 (both i and j are out of range)
// Outside on an edge   -> 1 (only one of i or j is out of range)
static float pixel_at(const float *image, long i, long j, long n) {
    bool row_ok = (i >= 0 && i < n);
    bool col_ok = (j >= 0 && j < n);

    if (row_ok && col_ok) {
        return image[i * n + j];
    }
    if (!row_ok && !col_ok) {
        return 0.0f;
    }
    return 1.0f;
}

void convolve(const float *image, float *output, std::size_t n, const float *mask, std::size_t m) {
    long N = static_cast<long>(n);
    long M = static_cast<long>(m);
    long half = (M - 1) / 2;  // how far the mask reaches from its center

    for (long x = 0; x < N; x++) {
        for (long y = 0; y < N; y++) {
            float sum = 0.0f;

            // Slide the mask over the pixels around (x, y)
            for (long i = 0; i < M; i++) {
                for (long j = 0; j < M; j++) {
                    sum += mask[i * M + j] * pixel_at(image, x + i - half, y + j - half, N);
                }
            }
            output[x * N + y] = sum;
        }
    }
}
