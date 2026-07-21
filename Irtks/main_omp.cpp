#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>
#include <optional>
#include <cstdint>
#include "blur_omp.h"

int main() {
    sf::Image originalImage;
    if (!originalImage.loadFromFile("image.png")) {
        if (!originalImage.loadFromFile("../../image.png")) {
            originalImage.resize({800, 600});
        }
    }
    
    unsigned int width = originalImage.getSize().x;
    unsigned int height = originalImage.getSize().y;
    
    sf::Image processedImage = originalImage;
    
    sf::Texture texture;
    (void)texture.loadFromImage(processedImage);
    sf::Sprite sprite(texture);
    
    sf::RenderWindow window(sf::VideoMode({width, height}), "Gaussian Blur - OpenMP");
    window.setFramerateLimit(60);
    
    sf::Font font;
    (void)font.openFromFile("C:/Windows/Fonts/arial.ttf");
    
    sf::Text infoText(font);
    infoText.setCharacterSize(18);
    infoText.setPosition({10.f, 10.f});
    
    std::string currentMode = "Original";
    float processTime = 0.0f;
    
    sf::Clock fpsClock;
    int frames = 0;
    int fps = 0;
    
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::B) {
                    currentMode = "OpenMP";
                    auto start = std::chrono::high_resolution_clock::now();
                    applyGaussianBlurOMP(originalImage.getPixelsPtr(), const_cast<std::uint8_t*>(processedImage.getPixelsPtr()), width, height);
                    auto end = std::chrono::high_resolution_clock::now();
                    processTime = std::chrono::duration<float, std::milli>(end - start).count();
                    (void)texture.loadFromImage(processedImage);
                } else if (keyEvent->code == sf::Keyboard::Key::R) {
                    currentMode = "Original";
                    processedImage = originalImage;
                    (void)texture.loadFromImage(processedImage);
                    processTime = 0.0f;
                }
            }
        }
        
        frames++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frames;
            frames = 0;
            fpsClock.restart();
        }
        
        infoText.setString("Mode: " + currentMode + "\n" +
                           "Time: " + std::to_string(processTime) + " ms\n" +
                           "Size: " + std::to_string(width) + "x" + std::to_string(height) + "\n" +
                           "FPS: " + std::to_string(fps) + "\n" +
                           "Press B (Blur), R (Reset)");
                           
        window.clear();
        window.draw(sprite);
        window.draw(infoText);
        window.display();
    }
    
    return 0;
}
