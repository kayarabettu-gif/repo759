#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <omp.h>

#include "convolution.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: ./task2 n t\n";
        return 1;
    }
    const std::size_t n = std::stoul(argv[1]);
    const int t = std::stoi(argv[2]);
    const std::size_t m = 3;

    // Image entries in [-10, 10], mask entries in [-1, 1]
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> imageDist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> maskDist(-1.0f, 1.0f);

    std::vector<float> image(n * n), mask(m * m), output(n * n);
    for (float& x : image) x = imageDist(gen);
    for (float& x : mask) x = maskDist(gen);

    omp_set_num_threads(t);

    const auto start = std::chrono::high_resolution_clock::now();
    convolve(image.data(), output.data(), n, mask.data(), m);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> ms = end - start;

    std::cout << output.front() << "\n" << output.back() << "\n" << ms.count() << "\n";
    return 0;
}
