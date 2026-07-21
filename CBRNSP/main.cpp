#include <SFML/Graphics.hpp>
#include <fmt/core.h>
#include <vector>
#include <complex>
#include <cmath>
#include <cstring>
#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "palette.h"

extern "C" {
    void ejecutar_burning_ship_cuda(double x_min, double x_max, double y_min, double y_max,
                                    int max_iteraciones, int width, int height,
                                    uint32_t* texture_buffer, int* global_hist);
}

#ifdef _WIN32
#include <windows.h>
#endif

#define WIDTH 1600
#define HEIGHT 900

int max_iteraciones = 100;
double x_min = -1.8;
double x_max = -1.7;
double y_min = -0.1;
double y_max = 0.05;

class _free_type_initializer {
public:
    FT_Library ft;
    FT_Face face;

    _free_type_initializer() {
        ft = nullptr;
        face = nullptr;
        FT_Error error = FT_Init_FreeType(&ft);
        if(!error) {
            FT_Error err_font = FT_New_Face(ft, "Arial.ttf", 0, &face);
            if (err_font) {
                err_font = FT_New_Face(ft, "../Arial.ttf", 0, &face);
            }
            if (err_font) {
                err_font = FT_New_Face(ft, "../../Arial.ttf", 0, &face);
            }
            if (err_font) {
                err_font = FT_New_Face(ft, "../../../Arial.ttf", 0, &face);
            }
        }
    }

    ~_free_type_initializer() {
        if(face) FT_Done_Face(face);
        if(ft) FT_Done_FreeType(ft);
    }
};

std::shared_ptr<_free_type_initializer> _freetype = nullptr;

void init_freetype() {
    _freetype = std::make_shared<_free_type_initializer>();
}

void draw_text_to_texture(unsigned char* texture, int tex_width, int tex_height,
                          const char* text, int x, int y,
                          int font_size, unsigned char r_fg = 255, unsigned char g_fg = 255, unsigned char b_fg = 255) {
    if (!_freetype || !_freetype->face) return;
    FT_Face face = _freetype->face;
    FT_Set_Pixel_Sizes(face, 0, font_size);
    int pen_x = x;
    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER)) continue;
        FT_Bitmap& bmp = face->glyph->bitmap;
        int bmp_w = bmp.width;
        int bmp_h = bmp.rows;
        for (int j = 0; j < bmp_h; j++) {
            for (int i = 0; i < bmp_w; i++) {
                int tx = pen_x + face->glyph->bitmap_left + i;
                int ty = y - face->glyph->bitmap_top + j;
                if (tx < 0 || tx >= tex_width || ty < 0 || ty >= tex_height) continue;
                unsigned char alpha = bmp.buffer[j * bmp.pitch + i];
                float a = alpha / 255.0f;
                int idx = (ty * tex_width + tx) * 4;
                unsigned char r_bg = texture[idx + 0];
                unsigned char g_bg = texture[idx + 1];
                unsigned char b_bg = texture[idx + 2];
                unsigned char a_bg = texture[idx + 3];
                texture[idx + 0] = (unsigned char)(r_fg * a + r_bg * (1.0f - a));
                texture[idx + 1] = (unsigned char)(g_fg * a + g_bg * (1.0f - a));
                texture[idx + 2] = (unsigned char)(b_fg * a + b_bg * (1.0f - a));
                texture[idx + 3] = a_bg > alpha ? a_bg : alpha;
            }
        }
        pen_x += face->glyph->advance.x >> 6;
    }
}

int main(int argc, char* argv[]) {
    init_freetype();
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Burning Ship Fractal - CUDA");
#ifdef _WIN32
    HWND hwnd = window.getNativeHandle();
    ShowWindow(hwnd, SW_MAXIMIZE);
#endif

    sf::Texture texture({WIDTH, HEIGHT});
    sf::Sprite sprite(texture);

    int frames = 0;
    int fps = 0;
    sf::Clock clock;
    bool running = true;

    uint32_t* texture_buffer = new uint32_t[WIDTH * HEIGHT];
    std::memset(texture_buffer, 0, WIDTH * HEIGHT * sizeof(uint32_t));

    int global_hist[16] = {0};

    while (running) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                running = false;
            } else if (event->is<sf::Event::KeyReleased>()) {
                auto evt = event->getIf<sf::Event::KeyReleased>();
                switch (evt->code) {
                case sf::Keyboard::Key::Up:
                    max_iteraciones += 10;
                    break;
                case sf::Keyboard::Key::Down:
                    max_iteraciones -= 10;
                    if (max_iteraciones < 10) max_iteraciones = 10;
                    break;
                case sf::Keyboard::Key::Escape:
                    running = false;
                    break;
                }
            }
        }
        if (!window.isOpen()) {
            running = false;
        }

        if (!running) break;

        std::memset(global_hist, 0, sizeof(global_hist));
        ejecutar_burning_ship_cuda(x_min, x_max, y_min, y_max, max_iteraciones, WIDTH, HEIGHT, texture_buffer, global_hist);

        frames++;
        if (clock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frames;
            frames = 0;
            clock.restart();
        }

        std::string mode_str = "Modo: CUDA (GPU)";
        std::string iter_str = fmt::format("Iteraciones: {}", max_iteraciones);
        std::string dom_str = fmt::format("Dominio: [{}, {}] x [{}, {}]", x_min, x_max, y_min, y_max);
        
        std::string hist_str = "Histograma: [";
        for (int b = 0; b < 16; ++b) {
            hist_str += fmt::format("{}{}", global_hist[b], (b < 15) ? ", " : "");
        }
        hist_str += "]";

        std::string fps_str = fmt::format("FPS: {}", fps);
        std::string help_str = "Teclas: Up/Down: Iteraciones | Esc: Cerrar";

        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, mode_str.c_str(), 10, 30, 18);
        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, iter_str.c_str(), 10, 60, 18);
        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, dom_str.c_str(), 10, 90, 18);
        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, hist_str.c_str(), 10, 120, 18);
        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, fps_str.c_str(), 10, 150, 18);
        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, help_str.c_str(), 10, HEIGHT - 30, 18);

        texture.update((const uint8_t*)texture_buffer);
        window.clear();
        window.draw(sprite);
        window.display();
    }

    delete[] texture_buffer;
    return 0;
}
