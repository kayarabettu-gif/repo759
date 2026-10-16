#include "msort.h"

#include <algorithm>
#include <vector>

// Serial, non-recursive sort for small sub-arrays.
static void insertionSort(int* a, std::size_t n) {
    for (std::size_t i = 1; i < n; i++) {
        const int key = a[i];
        std::size_t j = i;
        for (; j > 0 && a[j - 1] > key; j--) a[j] = a[j - 1];
        a[j] = key;
    }
}

// Sort a[0, n) using buf[0, n) as scratch.
// The two halves are independent, so each becomes an OpenMP task.
static void mergeSort(int* a, int* buf, std::size_t n, std::size_t threshold) {
    if (n <= threshold || n < 2) {
        insertionSort(a, n);
        return;
    }

    const std::size_t mid = n / 2;
#pragma omp task
    mergeSort(a, buf, mid, threshold);
#pragma omp task
    mergeSort(a + mid, buf + mid, n - mid, threshold);
#pragma omp taskwait

    std::merge(a, a + mid, a + mid, a + n, buf);
    std::copy(buf, buf + n, a);
}

// Sorts arr in place, ascending. One thread starts the recursion;
// the rest of the team picks up tasks as they are created.
void msort(int* arr, const std::size_t n, const std::size_t threshold) {
    std::vector<int> buf(n);
#pragma omp parallel
#pragma omp single
    mergeSort(arr, buf.data(), n, threshold);
}
