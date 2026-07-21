#include "fractal-simd.h"
#include <cmath>
#include <algorithm>
#include <immintrin.h>
#include "palette.h"

extern int max_iteraciones;

void newton_simd(double x_min, double x_max, double y_min, double y_max,
                 uint32_t row_start, uint32_t row_end,
                 uint32_t width, uint32_t height,
                 uint32_t* pixel_buffer, long long* total_iters) {
    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;

    __m256d w0x_vec = _mm256_set1_pd(1.0);
    __m256d w0y_vec = _mm256_setzero_pd();
    __m256d w1x_vec = _mm256_set1_pd(-0.5);
    __m256d w1y_vec = _mm256_set1_pd(0.8660254037844386);
    __m256d w2x_vec = _mm256_set1_pd(-0.5);
    __m256d w2y_vec = _mm256_set1_pd(-0.8660254037844386);
    __m256d eps_vec = _mm256_set1_pd(1e-8);
    __m256d eps2_vec = _mm256_set1_pd(1e-18);
    __m256d four_vec = _mm256_set1_pd(4.0);
    __m256d two_thirds = _mm256_set1_pd(2.0 / 3.0);
    __m256d three_vec = _mm256_set1_pd(3.0);
    __m256d zero_vec = _mm256_setzero_pd();
    __m256d one_vec = _mm256_set1_pd(1.0);
    __m256d two_vec = _mm256_set1_pd(2.0);
    __m256d minus_one_vec = _mm256_set1_pd(-1.0);

    __m256d dx_vec = _mm256_set1_pd(dx);
    __m256d x_min_vec = _mm256_set1_pd(x_min);
    __m256d offset_vec = _mm256_setr_pd(0.0, 1.0, 2.0, 3.0);

    long long global_iters_acc = 0;

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
            __m256d x = _mm256_add_pd(x_min_vec, _mm256_mul_pd(i_vec, dx_vec));
            __m256d y = cy;

            __m256d iter = zero_vec;
            __m256d root_vec = minus_one_vec;
            __m256d has_converged = zero_vec;
            __m256d has_escaped = zero_vec;

            for (int step = 0; step < max_iteraciones; step++) {
                __m256d has_stopped = _mm256_or_pd(has_converged, has_escaped);
                __m256d active_mask = _mm256_andnot_pd(has_stopped, one_vec);

                if (_mm256_testz_pd(active_mask, active_mask)) {
                    break;
                }

                __m256d r2_val = _mm256_add_pd(_mm256_mul_pd(x, x), _mm256_mul_pd(y, y));
                __m256d escape_check = _mm256_cmp_pd(r2_val, four_vec, _CMP_LE_OQ);

                __m256d dx0 = _mm256_sub_pd(x, w0x_vec);
                __m256d dy0 = _mm256_sub_pd(y, w0y_vec);
                __m256d d0 = _mm256_add_pd(_mm256_mul_pd(dx0, dx0), _mm256_mul_pd(dy0, dy0));
                __m256d c0_mask = _mm256_cmp_pd(d0, eps_vec, _CMP_LT_OQ);

                __m256d dx1 = _mm256_sub_pd(x, w1x_vec);
                __m256d dy1 = _mm256_sub_pd(y, w1y_vec);
                __m256d d1 = _mm256_add_pd(_mm256_mul_pd(dx1, dx1), _mm256_mul_pd(dy1, dy1));
                __m256d c1_mask = _mm256_cmp_pd(d1, eps_vec, _CMP_LT_OQ);

                __m256d dx2 = _mm256_sub_pd(x, w2x_vec);
                __m256d dy2 = _mm256_sub_pd(y, w2y_vec);
                __m256d d2 = _mm256_add_pd(_mm256_mul_pd(dx2, dx2), _mm256_mul_pd(dy2, dy2));
                __m256d c2_mask = _mm256_cmp_pd(d2, eps_vec, _CMP_LT_OQ);

                __m256d any_conv_step = _mm256_or_pd(c0_mask, _mm256_or_pd(c1_mask, c2_mask));

                __m256d newly_converged = _mm256_andnot_pd(has_stopped, any_conv_step);
                has_converged = _mm256_or_pd(has_converged, newly_converged);

                __m256d new_root = _mm256_blendv_pd(root_vec, zero_vec, c0_mask);
                new_root = _mm256_blendv_pd(new_root, one_vec, c1_mask);
                new_root = _mm256_blendv_pd(new_root, two_vec, c2_mask);
                root_vec = _mm256_blendv_pd(root_vec, new_root, newly_converged);

                __m256d newly_escaped = _mm256_andnot_pd(has_stopped, _mm256_andnot_pd(escape_check, one_vec));
                has_escaped = _mm256_or_pd(has_escaped, newly_escaped);

                __m256d increment = _mm256_blendv_pd(one_vec, zero_vec, has_stopped);
                iter = _mm256_add_pd(iter, increment);

                __m256d z2_re = _mm256_sub_pd(_mm256_mul_pd(x, x), _mm256_mul_pd(y, y));
                __m256d z2_im = _mm256_mul_pd(_mm256_mul_pd(two_vec, x), y);
                __m256d r2 = _mm256_add_pd(_mm256_mul_pd(z2_re, z2_re), _mm256_mul_pd(z2_im, z2_im));

                __m256d denom_ok = _mm256_cmp_pd(r2, eps2_vec, _CMP_GE_OQ);
                __m256d newly_failed_denom = _mm256_andnot_pd(has_stopped, _mm256_andnot_pd(denom_ok, one_vec));
                has_escaped = _mm256_or_pd(has_escaped, newly_failed_denom);

                __m256d three_r2 = _mm256_mul_pd(three_vec, r2);
                __m256d next_x = _mm256_add_pd(_mm256_mul_pd(two_thirds, x), _mm256_div_pd(z2_re, three_r2));
                __m256d next_y = _mm256_sub_pd(_mm256_mul_pd(two_thirds, y), _mm256_div_pd(z2_im, three_r2));

                x = next_x;
                y = next_y;
            }

            double iters[4];
            double roots[4];
            double converged[4];

            _mm256_storeu_pd(iters, iter);
            _mm256_storeu_pd(roots, root_vec);
            _mm256_storeu_pd(converged, has_converged);

            for (int k = 0; k < 4; k++) {
                int it = (int)iters[k];
                global_iters_acc += it;

                if (converged[k] != 0.0) {
                    int r = (int)roots[k];
                    int color_idx = (r * 5 + it) % 16;
                    pixel_buffer[(j - row_start) * width + (i + k)] = color_ramp[color_idx];
                } else {
                    pixel_buffer[(j - row_start) * width + (i + k)] = 0xFF000000;
                }
            }
        }

        for (; i < width; i++) {
            double x = x_min + i * dx;
            double y = cy_val;

            int iter = 0;
            int root = -1;
            bool converged = false;

            while (iter < max_iteraciones) {
                double r2_val = x * x + y * y;
                if (r2_val > 4.0) {
                    break;
                }

                double d0 = (x - 1.0) * (x - 1.0) + y * y;
                if (d0 < 1e-8) {
                    root = 0;
                    converged = true;
                    break;
                }

                double d1 = (x + 0.5) * (x + 0.5) + (y - 0.8660254037844386) * (y - 0.8660254037844386);
                if (d1 < 1e-8) {
                    root = 1;
                    converged = true;
                    break;
                }

                double d2 = (x + 0.5) * (x + 0.5) + (y + 0.8660254037844386) * (y + 0.8660254037844386);
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

            global_iters_acc += iter;

            if (converged) {
                int color_idx = (root * 5 + iter) % 16;
                pixel_buffer[(j - row_start) * width + i] = color_ramp[color_idx];
            } else {
                pixel_buffer[(j - row_start) * width + i] = 0xFF000000;
            }
        }
    }
    *total_iters += global_iters_acc;
}
