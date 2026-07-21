#ifndef FRACTAL_SIMD_H
#define FRACTAL_SIMD_H

#include <cstdint>

void newton_simd(double x_min, double x_max, double y_min, double y_max,
                 uint32_t row_start, uint32_t row_end,
                 uint32_t width, uint32_t height,
                 uint32_t* pixel_buffer, long long* total_iters);

#endif
