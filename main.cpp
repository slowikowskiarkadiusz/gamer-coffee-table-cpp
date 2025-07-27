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
                sf::RectangleShape pixel(sf::Vector2f(10, 10));
                pixel.setPosition(x * 10, y * 10);
                pixel.setFillColor(sf::Color::Blue);
                window.draw(pixel);
            }
        }
        window.display();
    }
}
