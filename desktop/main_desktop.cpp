#include <SFML/Graphics.hpp>

#include "engine.hpp"
#include "../src/input/keyboard_input_provider.hpp"

int main() {
    std::shared_ptr<keyboard_input_provider> input_provider = std::make_shared<keyboard_input_provider>();

    engine engineObj(input_provider);
    engineObj.run();

    sf::RenderWindow window(sf::VideoMode(engine::screen_size.x * 10, engine::screen_size.y * 10), "gamer-coffee-table-cpp");
    std::vector<std::vector<sf::CircleShape> > circles;
    for (int x = 0; x < engine::screen_size.x; x++) {
        circles.push_back({});
        for (int y = 0; y < engine::screen_size.y; y++) {
            circles[x].push_back(sf::CircleShape(5));
            circles[x][y].setPosition(x * 10, y * 10);
        }
    }

    engineObj.set_on_frame_finished([&circles, &window](grid2d<color> *frame) {
        std::cout << "desktop set_on_frame_finished start" << std::endl;
        window.clear();

        for (int y = 0; y < engine::screen_size.y; y++) {
            for (int x = 0; x < engine::screen_size.x; x++) {
                auto matrixPixel = frame->at(x, y);
                circles[x][y].setFillColor(sf::Color(matrixPixel.r * 255, matrixPixel.g * 255, matrixPixel.b * 255, matrixPixel.a * 255));
                window.draw(circles[x][y]);
            }
        }

        window.display();
        std::cout << "desktop set_on_frame_finished end" << std::endl;
    });


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            input_provider->sfEventQueue.push(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    engineObj.stop();
    return 0;
}
