#include "fractal-serial.h"
#include <cmath>
#include <algorithm>
#include "palette.h"

extern int max_iteraciones;

void newton_serial(double x_min, double x_max, double y_min, double y_max,
                   uint32_t row_start, uint32_t row_end,
                   uint32_t width, uint32_t height,
                   uint32_t* pixel_buffer, long long* total_iters) {
    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;

    double w0x = 1.0, w0y = 0.0;
    double w1x = -0.5, w1y = std::sqrt(3.0) / 2.0;
    double w2x = -0.5, w2y = -std::sqrt(3.0) / 2.0;

    long long iters_acc = 0;

    for (uint32_t j = row_start; j < row_end; j++) {
        if (j >= height) {
            for (uint32_t i = 0; i < width; i++) {
                pixel_buffer[(j - row_start) * width + i] = 0xFF000000;
            }
            continue;
        }

        for (uint32_t i = 0; i < width; i++) {
            double x = x_min + i * dx;
            double y = y_min + j * dy;

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

            iters_acc += iter;

            if (converged) {
                int color_idx = (root * 5 + iter) % 16;
                pixel_buffer[(j - row_start) * width + i] = color_ramp[color_idx];
            } else {
                pixel_buffer[(j - row_start) * width + i] = 0xFF000000;
            }
        }
    }
    *total_iters += iters_acc;
}
