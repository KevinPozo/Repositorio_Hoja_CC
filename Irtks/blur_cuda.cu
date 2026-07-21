#include "blur_cuda.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cstdint>

__constant__ int d_kernel[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1};

__global__ void blurKernel1D(const unsigned char* input, unsigned char* output, int width, int height) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int total_pixels = width * height;

    if (idx < total_pixels) {
        int x = idx % width;
        int y = idx / width;

        int sum[4] = {0, 0, 0, 0};
        int weight_sum = 0;

        for (int ky = -1; ky <= 1; ++ky) {
            for (int kx = -1; kx <= 1; ++kx) {
                int ny = y + ky;
                int nx = x + kx;

                if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                    int weight = d_kernel[(ky + 1) * 3 + (kx + 1)];
                    int neighbor_idx = (ny * width + nx) * 4;

                    sum[0] += input[neighbor_idx] * weight;
                    sum[1] += input[neighbor_idx + 1] * weight;
                    sum[2] += input[neighbor_idx + 2] * weight;
                    sum[3] += input[neighbor_idx + 3] * weight;
                    weight_sum += weight;
                }
            }
        }

        int out_idx = idx * 4;
        output[out_idx] = sum[0] / weight_sum;
        output[out_idx + 1] = sum[1] / weight_sum;
        output[out_idx + 2] = sum[2] / weight_sum;
        output[out_idx + 3] = sum[3] / weight_sum;
    }
}

void applyGaussianBlurCUDA(const std::uint8_t* input, std::uint8_t* output, int width, int height) {
    int total_pixels = width * height;
    size_t size = total_pixels * 4 * sizeof(unsigned char);

    unsigned char *d_input, *d_output;
    cudaMalloc(&d_input, size);
    cudaMalloc(&d_output, size);

    cudaMemcpy(d_input, input, size, cudaMemcpyHostToDevice);

    int blockSize = 256;
    int gridSize = (total_pixels + blockSize - 1) / blockSize;

    blurKernel1D<<<gridSize, blockSize>>>(d_input, d_output, width, height);
    cudaDeviceSynchronize();

    cudaMemcpy(output, d_output, size, cudaMemcpyDeviceToHost);

    cudaFree(d_input);
    cudaFree(d_output);
}
