#include "matmul.h"

// All matrices are n x n and stored in row-major order,
// so element (r, c) lives at index r * n + c.

// Clears C before we start adding into it
static void zero(double *C, unsigned int n) {
    for (unsigned int i = 0; i < n * n; i++) {
        C[i] = 0.0;
    }
}

// Loop order (i, j, k): the textbook row-times-column version
void mmul1(const double *A, const double *B, double *C, const unsigned int n) {
    zero(C, n);
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            for (unsigned int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

// Loop order (i, k, j): the inner loop walks along rows of B and C
void mmul2(const double *A, const double *B, double *C, const unsigned int n) {
    zero(C, n);
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int k = 0; k < n; k++) {
            for (unsigned int j = 0; j < n; j++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

// Loop order (j, k, i): the inner loop jumps down columns of A and C
void mmul3(const double *A, const double *B, double *C, const unsigned int n) {
    zero(C, n);
    for (unsigned int j = 0; j < n; j++) {
        for (unsigned int k = 0; k < n; k++) {
            for (unsigned int i = 0; i < n; i++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

// Same as mmul1, but A and B come in as std::vector
void mmul4(const std::vector<double> &A, const std::vector<double> &B, double *C, const unsigned int n) {
    zero(C, n);
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            for (unsigned int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}
