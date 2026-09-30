#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <random>

#include "scan.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./task1 n" << std::endl;
        return 1;
    }

    std::size_t n = std::strtoul(argv[1], nullptr, 10);

    float *arr = new float[n];
    float *output = new float[n];

    // Fill the input with random floats in [-1, 1]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (std::size_t i = 0; i < n; i++) {
        arr[i] = dist(gen);
    }

    // Time only the scan call itself
    auto start = high_resolution_clock::now();
    scan(arr, output, n);
    auto end = high_resolution_clock::now();
    duration<double, std::milli> elapsed = end - start;

    std::cout << elapsed.count() << std::endl;
    std::cout << output[0] << std::endl;
    std::cout << output[n - 1] << std::endl;

    delete[] arr;
    delete[] output;
    return 0;
}
