#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cstdint>
#include <cmath>
#include "palette.h"

#define PALETTE_SIZE 16

__constant__ uint32_t d_color_ramp[PALETTE_SIZE];

__global__
void burning_ship_cuda_kernel(double x_min, double x_max, double y_min, double y_max,
                               int max_iteraciones, int width, int height,
                               uint32_t* pixel_buffer, int* d_local_hist) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int total_pixels = width * height;

    if (index < total_pixels) {
        int i = index % width;
        int j = index / width;

        double dx = (x_max - x_min) / width;
        double dy = (y_max - y_min) / height;

        double cx = x_min + i * dx;
        double cy = y_min + j * dy;

        double zr = 0.0;
        double zi = 0.0;
        int iter = 0;

        while (iter < max_iteraciones && (zr * zr + zi * zi) <= 4.0) {
            double temp_zr = zr * zr - zi * zi + cx;
            double temp_zi = 2.0 * fabs(zr) * fabs(zi) + cy;
            zr = temp_zr;
            zi = temp_zi;
            iter++;
        }

        if (iter < max_iteraciones) {
            int bin = (iter * 16) / max_iteraciones;
            if (bin < 0) bin = 0;
            if (bin >= 16) bin = 15;
            atomicAdd(&d_local_hist[bin], 1);
            pixel_buffer[index] = d_color_ramp[iter % PALETTE_SIZE];
        } else {
            pixel_buffer[index] = 0xFF000000;
        }
    }
}

extern "C" {
    void ejecutar_burning_ship_cuda(double x_min, double x_max, double y_min, double y_max,
                                    int max_iteraciones, int width, int height,
                                    uint32_t* texture_buffer, int* global_hist) {
        size_t buffer_size = width * height * sizeof(uint32_t);
        uint32_t* d_pixel_buffer = nullptr;
        int* d_local_hist = nullptr;

        cudaMalloc(&d_pixel_buffer, buffer_size);
        cudaMalloc(&d_local_hist, 16 * sizeof(int));
        cudaMemset(d_local_hist, 0, 16 * sizeof(int));

        cudaMemcpyToSymbol(d_color_ramp, color_ramp.data(), PALETTE_SIZE * sizeof(uint32_t));

        int threads_per_block = 256;
        int total_pixels = width * height;
        int blocks_per_grid = (total_pixels + threads_per_block - 1) / threads_per_block;

        burning_ship_cuda_kernel<<<blocks_per_grid, threads_per_block>>>(
            x_min, x_max, y_min, y_max, max_iteraciones, width, height, d_pixel_buffer, d_local_hist);

        cudaMemcpy(texture_buffer, d_pixel_buffer, buffer_size, cudaMemcpyDeviceToHost);
        cudaMemcpy(global_hist, d_local_hist, 16 * sizeof(int), cudaMemcpyDeviceToHost);

        cudaFree(d_pixel_buffer);
        cudaFree(d_local_hist);
    }
}
