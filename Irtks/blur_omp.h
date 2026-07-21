#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
void applyGaussianBlurOMP(const std::uint8_t* input, std::uint8_t* output, int width, int height);
