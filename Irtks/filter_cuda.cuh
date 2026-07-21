#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
void applyFilterCUDA(const std::uint8_t* input, std::uint8_t* output, int width, int height);
