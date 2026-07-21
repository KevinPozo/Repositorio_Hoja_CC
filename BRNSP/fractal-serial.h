#ifndef FRACTAL_SERIAL_H
#define FRACTAL_SERIAL_H

#include <cstdint>

void burning_ship_serial(double x_min, double x_max, double y_min, double y_max,
                         uint32_t row_start, uint32_t row_end,
                         uint32_t width, uint32_t height,
                         uint32_t* pixel_buffer, int* local_hist);

#endif
