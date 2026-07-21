#include "fractal-mpi.h"
#include <cmath>
#include <algorithm>
#include "palette.h"

extern int max_iteraciones;

void burning_ship_mpi(double x_min, double x_max, double y_min, double y_max,
                      uint32_t row_start, uint32_t row_end,
                      uint32_t width, uint32_t height,
                      uint32_t* pixel_buffer, int* local_hist) {
    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;

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
