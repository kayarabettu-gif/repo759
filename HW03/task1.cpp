#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <omp.h>

#include "matmul.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: ./task1 n t\n";
        return 1;
    }
    const std::size_t n = std::stoul(argv[1]);
    const int t = std::stoi(argv[2]);

    // Random n x n matrices with entries in [-1, 1]
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    std::vector<float> A(n * n), B(n * n), C(n * n);
    for (float& x : A) x = dist(gen);
    for (float& x : B) x = dist(gen);

    omp_set_num_threads(t);

    const auto start = std::chrono::high_resolution_clock::now();
    mmul(A.data(), B.data(), C.data(), n);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> ms = end - start;

    std::cout << C.front() << "\n" << C.back() << "\n" << ms.count() << "\n";
    return 0;
}
