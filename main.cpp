#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(320, 320), "LED Matrix");
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e))
            if (e.type == sf::Event::Closed)
                window.close();

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
