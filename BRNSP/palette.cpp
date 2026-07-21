#include "palette.h"

uint32_t bswap32(uint32_t a) {
    return ((a & 0x000000FF) << 24) |
           ((a & 0x0000FF00) << 8)  |
           ((a & 0x00FF0000) >> 8)  |
           ((a & 0xFF000000) >> 24);
}

std::vector<uint32_t> color_ramp = {
    bswap32(0xFFFFD9FF),
    bswap32(0xF5FBC2FF),
    bswap32(0xE8F6B1FF),
    bswap32(0xD2EDB3FF),
    bswap32(0xB2E1B6FF),
    bswap32(0x89D1BAFF),
    bswap32(0x64C3BFFF),
    bswap32(0x41B6C4FF),
    bswap32(0x2CA1C2FF),
    bswap32(0x1E8ABDFF),
    bswap32(0x216DAFFF),
    bswap32(0x2352A2FF),
    bswap32(0x253A97FF),
    bswap32(0x192A7AFF),
    bswap32(0x081D58FF),
    bswap32(0x040a2cFF)
};
