#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <random>

#include "convolution.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./task2 n m" << std::endl;
        return 1;
    }

    std::size_t n = std::strtoul(argv[1], nullptr, 10);
    std::size_t m = std::strtoul(argv[2], nullptr, 10);

    // Both matrices are stored as flat 1D arrays in row-major order
    float *image = new float[n * n];
    float *mask = new float[m * m];
    float *output = new float[n * n];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> image_dist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> mask_dist(-1.0f, 1.0f);

    for (std::size_t i = 0; i < n * n; i++) {
        image[i] = image_dist(gen);
    }
    for (std::size_t i = 0; i < m * m; i++) {
        mask[i] = mask_dist(gen);
    }

    // Time only the convolution
    auto start = high_resolution_clock::now();
    convolve(image, output, n, mask, m);
    auto end = high_resolution_clock::now();
    duration<double, std::milli> elapsed = end - start;

    std::cout << elapsed.count() << std::endl;
    std::cout << output[0] << std::endl;
    std::cout << output[n * n - 1] << std::endl;

    delete[] image;
    delete[] mask;
    delete[] output;
    return 0;
}
