#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <omp.h>

#include "msort.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "usage: ./task3 n t ts\n";
        return 1;
    }
    const std::size_t n = std::stoul(argv[1]);
    const int t = std::stoi(argv[2]);
    const std::size_t ts = std::stoul(argv[3]);

    // Random integers in [-1000, 1000]
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(-1000, 1000);
    std::vector<int> arr(n);
    for (int& x : arr) x = dist(gen);

    omp_set_num_threads(t);

    const auto start = std::chrono::high_resolution_clock::now();
    msort(arr.data(), n, ts);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> ms = end - start;

    std::cout << arr.front() << "\n" << arr.back() << "\n" << ms.count() << "\n";
    return 0;
}
