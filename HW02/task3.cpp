#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "matmul.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;

int main() {
    const unsigned int n = 1024;  // assignment asks for at least 1000 x 1000
    const unsigned int size = n * n;

    // Raw arrays for mmul1-3, vectors for mmul4 (same values in both)
    double *A = new double[size];
    double *B = new double[size];
    double *C = new double[size];
    std::vector<double> A_vec(size);
    std::vector<double> B_vec(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (unsigned int i = 0; i < size; i++) {
        A[i] = dist(gen);
        B[i] = dist(gen);
        A_vec[i] = A[i];
        B_vec[i] = B[i];
    }

    std::cout << n << std::endl;

    // Run one version, then print its time (ms) and the last element of C
    auto start = high_resolution_clock::now();
    mmul1(A, B, C, n);
    auto end = high_resolution_clock::now();
    duration<double, std::milli> elapsed = end - start;
    std::cout << elapsed.count() << std::endl;
    std::cout << C[size - 1] << std::endl;

    start = high_resolution_clock::now();
    mmul2(A, B, C, n);
    end = high_resolution_clock::now();
    elapsed = end - start;
    std::cout << elapsed.count() << std::endl;
    std::cout << C[size - 1] << std::endl;

    start = high_resolution_clock::now();
    mmul3(A, B, C, n);
    end = high_resolution_clock::now();
    elapsed = end - start;
    std::cout << elapsed.count() << std::endl;
    std::cout << C[size - 1] << std::endl;

    start = high_resolution_clock::now();
    mmul4(A_vec, B_vec, C, n);
    end = high_resolution_clock::now();
    elapsed = end - start;
    std::cout << elapsed.count() << std::endl;
    std::cout << C[size - 1] << std::endl;

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}
