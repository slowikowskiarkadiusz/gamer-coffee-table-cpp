#include <SFML/Graphics.hpp>

#include "./src/engine.hpp"
#include "src/input/keyboard_input_provider.hpp"


int main() {
    std::shared_ptr<keyboard_input_provider> input_provider = std::make_shared<keyboard_input_provider>();

    engine engineObj(input_provider);
    engineObj.run();

    sf::RenderWindow window(sf::VideoMode(engine::screen_size.x * 10, engine::screen_size.y * 10), "gamer-coffee-table-cpp");

    while (window.isOpen()) {
        window.clear();

        sf::Event event;
        while (window.pollEvent(event)) {
            input_provider->sfEventQueue.push(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (int y = 0; y < engine::screen_size.y; y++) {
            for (int x = 0; x < engine::screen_size.x; x++) {
                auto matrixPixel = engineObj.screen.pixels()[x][y];
                sf::CircleShape pixel(5);
                pixel.setPosition(x * 10, y * 10);
                pixel.setFillColor(sf::Color(matrixPixel.r * 255, matrixPixel.g * 255, matrixPixel.b * 255, matrixPixel.a * 255));
                window.draw(pixel);
            }
        }

        window.display();
    }

    engineObj.stop();
    return 0;
}
