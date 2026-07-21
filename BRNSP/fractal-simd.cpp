#include "fractal-simd.h"
#include <cmath>
#include <algorithm>
#include <immintrin.h>
#include "palette.h"

extern int max_iteraciones;

void burning_ship_simd(double x_min, double x_max, double y_min, double y_max,
                       uint32_t row_start, uint32_t row_end,
                       uint32_t width, uint32_t height,
                       uint32_t* pixel_buffer, int* local_hist) {
    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;

    __m256d abs_mask = _mm256_castsi256_pd(_mm256_set1_epi64x(0x7FFFFFFFFFFFFFFF));
    __m256d two_vec = _mm256_set1_pd(2.0);
    __m256d four_vec = _mm256_set1_pd(4.0);
    __m256d zero_vec = _mm256_setzero_pd();
    __m256d one_vec = _mm256_set1_pd(1.0);
    __m256d max_iter_vec = _mm256_set1_pd((double)max_iteraciones);
    __m256d dx_vec = _mm256_set1_pd(dx);
    __m256d x_min_vec = _mm256_set1_pd(x_min);
    
    __m256d offset_vec = _mm256_setr_pd(0.0, 1.0, 2.0, 3.0);

    for (uint32_t j = row_start; j < row_end; j++) {
        if (j >= height) {
            for (uint32_t i = 0; i < width; i++) {
                pixel_buffer[(j - row_start) * width + i] = 0xFF000000;
            }
            continue;
        }

        double cy_val = y_min + j * dy;
        __m256d cy = _mm256_set1_pd(cy_val);

        uint32_t i = 0;
        for (; i + 3 < width; i += 4) {
            __m256d i_vec = _mm256_add_pd(_mm256_set1_pd((double)i), offset_vec);
            __m256d cx = _mm256_add_pd(x_min_vec, _mm256_mul_pd(i_vec, dx_vec));

            __m256d zr = zero_vec;
            __m256d zi = zero_vec;
            __m256d iter = zero_vec;

            for (int step = 0; step < max_iteraciones; step++) {
                __m256d r2 = _mm256_mul_pd(zr, zr);
                __m256d i2 = _mm256_mul_pd(zi, zi);
                __m256d dist2 = _mm256_add_pd(r2, i2);

                __m256d escape_mask = _mm256_cmp_pd(dist2, four_vec, _CMP_LE_OQ);
                __m256d iter_mask = _mm256_cmp_pd(iter, max_iter_vec, _CMP_LT_OQ);
                __m256d active_mask = _mm256_and_pd(escape_mask, iter_mask);

                if (_mm256_testz_pd(active_mask, active_mask)) {
                    break;
                }

                __m256d increment = _mm256_blendv_pd(zero_vec, one_vec, active_mask);
                iter = _mm256_add_pd(iter, increment);

                __m256d temp_zr = _mm256_add_pd(_mm256_sub_pd(r2, i2), cx);
                __m256d zr_abs = _mm256_and_pd(zr, abs_mask);
                __m256d zi_abs = _mm256_and_pd(zi, abs_mask);
                __m256d prod = _mm256_mul_pd(zr_abs, zi_abs);
                __m256d temp_zi = _mm256_add_pd(_mm256_mul_pd(two_vec, prod), cy);

                zr = temp_zr;
                zi = temp_zi;
            }

            double iters[4];
            _mm256_storeu_pd(iters, iter);

            for (int k = 0; k < 4; k++) {
                int it = (int)iters[k];
                if (it < max_iteraciones) {
                    int bin = (it * 16) / max_iteraciones;
                    if (bin < 0) bin = 0;
                    if (bin >= 16) bin = 15;
                    local_hist[bin]++;
                    pixel_buffer[(j - row_start) * width + (i + k)] = color_ramp[it % 16];
                } else {
                    pixel_buffer[(j - row_start) * width + (i + k)] = 0xFF000000;
                }
            }
        }

        for (; i < width; i++) {
            double cx = x_min + i * dx;
            double cy = cy_val;

            double zr = 0.0;
            double zi = 0.0;
            int iter = 0;

            while (iter < max_iteraciones && (zr * zr + zi * zi) <= 4.0) {
                double temp_zr = zr * zr - zi * zi + cx;
                double temp_zi = 2.0 * std::abs(zr) * std::abs(zi) + cy;
                zr = temp_zr;
                zi = temp_zi;
                iter++;
            }

            if (iter < max_iteraciones) {
                int bin = (iter * 16) / max_iteraciones;
                if (bin < 0) bin = 0;
                if (bin >= 16) bin = 15;
                local_hist[bin]++;
                pixel_buffer[(j - row_start) * width + i] = color_ramp[iter % 16];
            } else {
                pixel_buffer[(j - row_start) * width + i] = 0xFF000000;
            }
        }
    }
}
