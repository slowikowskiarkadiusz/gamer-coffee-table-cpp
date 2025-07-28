#include <SFML/Graphics.hpp>

#include "./engine/engine.hpp"
#include "engine/input/keyboard_input_provider.hpp"

sf::RenderWindow window(sf::VideoMode(320, 320), "gamer-coffee-table-cpp");

int main() {
    std::shared_ptr<keyboard_input_provider> input_provider = std::make_shared<keyboard_input_provider>();

    engine engineObj(input_provider);
    engineObj.run();
    while (window.isOpen()) {
        window.clear();

        sf::Event event;
        while (window.pollEvent(event)) {
            input_provider->sfEventQueue.push(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 32; x++) {
                auto matrixPixel = engineObj.screen.pixels()[x][y];
                sf::CircleShape pixel(5);
                pixel.setPosition(x * 10, y * 10);
                pixel.setFillColor(sf::Color(matrixPixel.r / 255, matrixPixel.g / 255, matrixPixel.b / 255, matrixPixel.a / 255));
                window.draw(pixel);
            }
        }

        window.display();
    }

    engineObj.stop();
    return 0;
}
