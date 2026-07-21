#include "blur_cuda.cuh"
#include <cstdint>

void applyGaussianBlurCUDA(const std::uint8_t* input, std::uint8_t* output, int width, int height) {
    for (int i = 0; i < width * height * 4; ++i) {
        output[i] = input[i];
    }
}
