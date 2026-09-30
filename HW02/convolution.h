#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include <cstddef>
void convolve(const float *image, float *output, std::size_t n, const float *mask, std::size_t m);
#endif
