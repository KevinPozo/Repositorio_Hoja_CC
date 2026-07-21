#include <mpi.h>
#include <SFML/Graphics.hpp>
#include <fmt/core.h>
#include <vector>
#include <complex>
#include <cmath>
#include <cstring>
#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "fractal-mpi.h"
#include "fractal-serial.h"
#include "fractal-omp.h"
#include "fractal-simd.h"
#include "palette.h"

extern "C" {
    void ejecutar_newton_cuda(double x_min, double x_max, double y_min, double y_max,
                              int max_iteraciones, int width, int height,
                              uint32_t* texture_buffer, long long* total_iters);
}

#ifdef _WIN32
#include <windows.h>
#endif

#define WIDTH 1600
#define HEIGHT 900

int max_iteraciones = 50;
double x_min = -1.5;
double x_max = 1.5;
double y_min = -1.0;
double y_max = 1.0;

enum class Mode {
    SERIAL = 0,
    OMP = 1,
    SIMD = 2,
    MPI = 3,
    CUDA = 4
};

Mode current_mode = Mode::MPI;

struct Config {
    int max_iter;
    double c_real;
    double c_imag;
    int running;
    int mode;
};

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

void setup_ui(int nprocs, int delta, int row_start, int row_end, char* all_names, uint32_t* pixel_buffer, uint32_t* texture_buffer) {
    init_freetype();
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Newton Fractal - Multi-Mode Parallel");
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

    double max_time = 0.0;
    long long total_iters = 0;

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
                case sf::Keyboard::Key::Num1:
                case sf::Keyboard::Key::Numpad1:
                    current_mode = Mode::SERIAL;
                    break;
                case sf::Keyboard::Key::Num2:
                case sf::Keyboard::Key::Numpad2:
                    current_mode = Mode::OMP;
                    break;
                case sf::Keyboard::Key::Num3:
                case sf::Keyboard::Key::Numpad3:
                    current_mode = Mode::SIMD;
                    break;
                case sf::Keyboard::Key::Num4:
                case sf::Keyboard::Key::Numpad4:
                    current_mode = Mode::MPI;
                    break;
                case sf::Keyboard::Key::Num5:
                case sf::Keyboard::Key::Numpad5:
                    current_mode = Mode::CUDA;
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

        Config config = { max_iteraciones, 0.0, 0.0, running ? 1 : 0, (int)current_mode };
        MPI_Bcast(&config, sizeof(Config), MPI_BYTE, 0, MPI_COMM_WORLD);
        if (!running) break;

        if (current_mode == Mode::MPI) {
            std::vector<MPI_Request> requests;
            if (nprocs > 1) {
                requests.resize(nprocs - 1);
                for (int i = 1; i < nprocs; i++) {
                    MPI_Irecv(texture_buffer + (i * delta * WIDTH), WIDTH * delta, MPI_UINT32_T, i, 0, MPI_COMM_WORLD, &requests[i - 1]);
                }
            }

            long long local_iters = 0;
            sf::Clock local_clock;
            newton_mpi(x_min, x_max, y_min, y_max, row_start, row_end, WIDTH, HEIGHT, pixel_buffer, &local_iters);
            double local_time = local_clock.getElapsedTime().asMilliseconds();
            std::memcpy(texture_buffer + (row_start * WIDTH), pixel_buffer, WIDTH * delta * sizeof(uint32_t));

            if (nprocs > 1) {
                MPI_Waitall(nprocs - 1, requests.data(), MPI_STATUSES_IGNORE);
            }

            MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
            MPI_Reduce(&local_iters, &total_iters, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
        } else {
            total_iters = 0;
            sf::Clock local_clock;
            if (current_mode == Mode::SERIAL) {
                newton_serial(x_min, x_max, y_min, y_max, 0, HEIGHT, WIDTH, HEIGHT, texture_buffer, &total_iters);
            } else if (current_mode == Mode::OMP) {
                newton_omp(x_min, x_max, y_min, y_max, 0, HEIGHT, WIDTH, HEIGHT, texture_buffer, &total_iters);
            } else if (current_mode == Mode::SIMD) {
                newton_simd(x_min, x_max, y_min, y_max, 0, HEIGHT, WIDTH, HEIGHT, texture_buffer, &total_iters);
            } else if (current_mode == Mode::CUDA) {
                ejecutar_newton_cuda(x_min, x_max, y_min, y_max, max_iteraciones, WIDTH, HEIGHT, texture_buffer, &total_iters);
            }
            max_time = local_clock.getElapsedTime().asMilliseconds();
        }

        frames++;
        if (clock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frames;
            frames = 0;
            clock.restart();
        }

        std::string ranks_str = "Ranks: ";
        for (int i = 0; i < nprocs; i++) {
            std::string p_name(all_names + i * MPI_MAX_PROCESSOR_NAME);
            ranks_str += fmt::format("[R{}: {}] ", i, p_name);
        }

        std::string mode_name = "";
        switch (current_mode) {
        case Mode::SERIAL: mode_name = "Serial (Secuencial)"; break;
        case Mode::OMP:    mode_name = "OpenMP"; break;
        case Mode::SIMD:   mode_name = "SIMD (AVX2)"; break;
        case Mode::MPI:    mode_name = "MPI (Distribuido)"; break;
        case Mode::CUDA:   mode_name = "CUDA (GPU)"; break;
        }

        std::string mode_str = fmt::format("Modo de calculo: {}", mode_name);
        std::string info_str = fmt::format(
            "max_iter: {} | max_compute_ms: {:.2f} ms | total_iters: {} | FPS: {}",
            max_iteraciones, max_time, total_iters, fps
        );

        std::string dom_str = fmt::format("Dominio: [{}, {}] x [{}, {}]", x_min, x_max, y_min, y_max);
        std::string help_str = "Teclas: 1:Serial | 2:OMP | 3:SIMD | 4:MPI | 5:CUDA | Up/Down: Iteraciones | Esc: Cerrar";

        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, ranks_str.c_str(), 10, 30, 18);
        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, mode_str.c_str(), 10, 60, 18);
        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, info_str.c_str(), 10, 90, 18);
        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, dom_str.c_str(), 10, 120, 18);
        draw_text_to_texture((unsigned char*)texture_buffer, WIDTH, HEIGHT, help_str.c_str(), 10, HEIGHT - 30, 18);

        texture.update((const uint8_t*)texture_buffer);
        window.clear();
        window.draw(sprite);
        window.display();
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    char proc_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(proc_name, &name_len);

    char* all_names = nullptr;
    if (rank == 0) {
        all_names = new char[nprocs * MPI_MAX_PROCESSOR_NAME];
    }
    MPI_Gather(proc_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR,
               all_names, MPI_MAX_PROCESSOR_NAME, MPI_CHAR,
               0, MPI_COMM_WORLD);

    int delta = std::ceil(HEIGHT * 1.0 / nprocs);
    int row_start = rank * delta;
    int row_end = row_start + delta;

    uint32_t* pixel_buffer = new uint32_t[WIDTH * delta];
    std::memset(pixel_buffer, 0, WIDTH * delta * sizeof(uint32_t));

    if (rank == 0) {
        uint32_t* texture_buffer = new uint32_t[WIDTH * nprocs * delta];
        std::memset(texture_buffer, 0, WIDTH * nprocs * delta * sizeof(uint32_t));

        setup_ui(nprocs, delta, row_start, row_end, all_names, pixel_buffer, texture_buffer);

        delete[] texture_buffer;
        delete[] all_names;
    } else {
        while (true) {
            Config config;
            MPI_Bcast(&config, sizeof(Config), MPI_BYTE, 0, MPI_COMM_WORLD);
            if (!config.running) {
                break;
            }
            max_iteraciones = config.max_iter;

            if (config.mode == (int)Mode::MPI) {
                long long local_iters = 0;
                sf::Clock local_clock;
                newton_mpi(x_min, x_max, y_min, y_max, row_start, row_end, WIDTH, HEIGHT, pixel_buffer, &local_iters);
                double local_time = local_clock.getElapsedTime().asMilliseconds();

                MPI_Send(pixel_buffer, WIDTH * delta, MPI_UINT32_T, 0, 0, MPI_COMM_WORLD);

                double max_time = 0.0;
                long long total_iters = 0;
                MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
                MPI_Reduce(&local_iters, &total_iters, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
            }
        }
    }

    delete[] pixel_buffer;
    MPI_Finalize();
    return 0;
}
