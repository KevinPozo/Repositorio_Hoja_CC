#include "fractal-mpi.h"
#include <complex>
#include <cmath>
#include <algorithm>
#include "palette.h"

extern int max_iteraciones;

void newton_mpi(double x_min, double x_max, double y_min, double y_max,
                uint32_t row_start, uint32_t row_end,
                uint32_t width, uint32_t height,
                uint32_t* pixel_buffer, long long* local_iters) {
    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;

    std::complex<double> w0(1.0, 0.0);
    std::complex<double> w1(-0.5, std::sqrt(3.0) / 2.0);
    std::complex<double> w2(-0.5, -std::sqrt(3.0) / 2.0);

    for (uint32_t j = row_start; j < row_end; j++) {
        if (j >= height) {
            for (uint32_t i = 0; i < width; i++) {
                pixel_buffer[(j - row_start) * width + i] = 0xFF000000;
            }
            continue;
        }

        for (uint32_t i = 0; i < width; i++) {
            double cx = x_min + i * dx;
            double cy = y_min + j * dy;

            std::complex<double> z(cx, cy);
            int iter = 0;
            int root = -1;
            bool converged = false;

            while (iter < max_iteraciones) {
                if (std::abs(z) > 2.0) {
                    break;
                }

                if (std::abs(z - w0) < 1e-4) {
                    root = 0;
                    converged = true;
                    break;
                }
                if (std::abs(z - w1) < 1e-4) {
                    root = 1;
                    converged = true;
                    break;
                }
                if (std::abs(z - w2) < 1e-4) {
                    root = 2;
                    converged = true;
                    break;
                }

                std::complex<double> z2 = z * z;
                if (std::abs(z2) < 1e-9) {
                    break;
                }

                z = z - (z * z2 - 1.0) / (3.0 * z2);
                iter++;
            }

            *local_iters += iter;

            if (converged) {
                int color_idx = (root * 5 + iter) % 16;
                pixel_buffer[(j - row_start) * width + i] = color_ramp[color_idx];
            } else {
                pixel_buffer[(j - row_start) * width + i] = 0xFF000000;
            }
        }
    }
}
