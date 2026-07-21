#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cstdint>
#include <cmath>
#include "palette.h"

#define PALETTE_SIZE 16

__constant__ uint32_t d_color_ramp[PALETTE_SIZE];

__global__
void newton_cuda_kernel(double x_min, double x_max, double y_min, double y_max,
                        int max_iteraciones, int width, int height,
                        uint32_t* pixel_buffer, unsigned long long* d_total_iters) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int total_pixels = width * height;

    if (index < total_pixels) {
        int i = index % width;
        int j = index / width;

        double dx = (x_max - x_min) / width;
        double dy = (y_max - y_min) / height;

        double x = x_min + i * dx;
        double y = y_min + j * dy;

        double w0x = 1.0, w0y = 0.0;
        double w1x = -0.5, w1y = 0.8660254037844386;
        double w2x = -0.5, w2y = -0.8660254037844386;

        int iter = 0;
        int root = -1;
        bool converged = false;

        while (iter < max_iteraciones) {
            double r2_val = x * x + y * y;
            if (r2_val > 4.0) {
                break;
            }

            double d0 = (x - w0x) * (x - w0x) + (y - w0y) * (y - w0y);
            if (d0 < 1e-8) {
                root = 0;
                converged = true;
                break;
            }

            double d1 = (x - w1x) * (x - w1x) + (y - w1y) * (y - w1y);
            if (d1 < 1e-8) {
                root = 1;
                converged = true;
                break;
            }

            double d2 = (x - w2x) * (x - w2x) + (y - w2y) * (y - w2y);
            if (d2 < 1e-8) {
                root = 2;
                converged = true;
                break;
            }

            double z2_re = x * x - y * y;
            double z2_im = 2.0 * x * y;
            double r2 = z2_re * z2_re + z2_im * z2_im;

            if (r2 < 1e-18) {
                break;
            }

            double next_x = (2.0 / 3.0) * x + z2_re / (3.0 * r2);
            double next_y = (2.0 / 3.0) * y - z2_im / (3.0 * r2);

            x = next_x;
            y = next_y;
            iter++;
        }

        atomicAdd(d_total_iters, (unsigned long long)iter);

        if (converged) {
            int color_idx = (root * 5 + iter) % 16;
            pixel_buffer[index] = d_color_ramp[color_idx];
        } else {
            pixel_buffer[index] = 0xFF000000;
        }
    }
}

extern "C" {
    void ejecutar_newton_cuda(double x_min, double x_max, double y_min, double y_max,
                              int max_iteraciones, int width, int height,
                              uint32_t* texture_buffer, long long* total_iters) {
        size_t buffer_size = width * height * sizeof(uint32_t);
        uint32_t* d_pixel_buffer = nullptr;
        unsigned long long* d_total_iters = nullptr;

        cudaMalloc(&d_pixel_buffer, buffer_size);
        cudaMalloc(&d_total_iters, sizeof(unsigned long long));
        
        unsigned long long zero_val = 0;
        cudaMemcpy(d_total_iters, &zero_val, sizeof(unsigned long long), cudaMemcpyHostToDevice);

        cudaMemcpyToSymbol(d_color_ramp, color_ramp.data(), PALETTE_SIZE * sizeof(uint32_t));

        int threads_per_block = 256;
        int total_pixels = width * height;
        int blocks_per_grid = (total_pixels + threads_per_block - 1) / threads_per_block;

        newton_cuda_kernel<<<blocks_per_grid, threads_per_block>>>(
            x_min, x_max, y_min, y_max, max_iteraciones, width, height, d_pixel_buffer, d_total_iters);

        cudaMemcpy(texture_buffer, d_pixel_buffer, buffer_size, cudaMemcpyDeviceToHost);
        
        unsigned long long h_total_iters = 0;
        cudaMemcpy(&h_total_iters, d_total_iters, sizeof(unsigned long long), cudaMemcpyDeviceToHost);
        *total_iters += h_total_iters;

        cudaFree(d_pixel_buffer);
        cudaFree(d_total_iters);
    }
}
