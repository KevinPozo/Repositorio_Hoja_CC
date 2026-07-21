#include "blur_omp.h"
#include <omp.h>
#include <cstdint>

void applyGaussianBlurOMP(const std::uint8_t* input, std::uint8_t* output, int width, int height) {
    const int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
    
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        
        int rows_per_thread = height / num_threads;
        int start_y = thread_id * rows_per_thread;
        int end_y = (thread_id == num_threads - 1) ? height : start_y + rows_per_thread;

        for (int y = start_y; y < end_y; ++y) {
            for (int x = 0; x < width; ++x) {
                int sum[4] = {0, 0, 0, 0};
                int weight_sum = 0;

                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        int ny = y + ky;
                        int nx = x + kx;

                        if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                            int weight = kernel[ky + 1][kx + 1];
                            int index = (ny * width + nx) * 4;
                            
                            sum[0] += input[index] * weight;
                            sum[1] += input[index + 1] * weight;
                            sum[2] += input[index + 2] * weight;
                            sum[3] += input[index + 3] * weight;
                            weight_sum += weight;
                        }
                    }
                }

                int out_index = (y * width + x) * 4;
                output[out_index] = sum[0] / weight_sum;
                output[out_index + 1] = sum[1] / weight_sum;
                output[out_index + 2] = sum[2] / weight_sum;
                output[out_index + 3] = sum[3] / weight_sum;
            }
        }
    }
}
