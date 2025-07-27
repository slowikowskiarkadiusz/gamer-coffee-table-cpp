#pragma once

#include <SFML/Graphics.hpp>

#include "engine/input/keyboard_input_provider.hpp"

extern sf::RenderWindow window(sf::VideoMode(320, 320), "gamer-coffee-table-cpp");

int main() {
    keyboard_input_provider keyboard_input_provider(&window);

    while (window.isOpen()) {
        window.clear();
        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 32; x++) {
                sf::CircleShape pixel(5);
                pixel.setPosition(x * 10, y * 10);
                pixel.setFillColor(x % 2 == 1
                                       ? sf::Color(0.5 * 255, 0.5 * 255, 0.5 * 255)
                                       : sf::Color(0.5 * 255, 0 * 255, 0.5 * 255));
                window.draw(pixel);
            }
        }
        window.display();
    }
}
