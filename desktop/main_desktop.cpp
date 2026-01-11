#include <SFML/Graphics.hpp>

#include "../src/input/keyboard_input_provider.hpp"
#include "engine.hpp"

int main() {
  std::shared_ptr<keyboard_input_provider> input_provider =
      std::make_shared<keyboard_input_provider>();

  engine engineObj(input_provider);
  engineObj.run();

  sf::RenderWindow window(
      sf::VideoMode(engine::screen_size.x * 10, engine::screen_size.y * 10),
      "gamer-coffee-table-cpp");
  std::vector<std::vector<sf::CircleShape>> circles;
  for (int x = 0; x < engine::screen_size.x; x++) {
    circles.push_back({});
    for (int y = 0; y < engine::screen_size.y; y++) {
      circles[x].push_back(sf::CircleShape(5));
      circles[x][y].setPosition(x * 10, y * 10);
    }
  }

  int frame_count = 0;

  auto set_on_frame_finished_action = [&circles, &window,
                                       &frame_count](grid2d<color>* frame) {
    frame_count++;
    frame_count %= 100;
    std::cout << "desktop set_on_frame_finished start "
              << std::to_string(frame_count) << std::endl;
    window.clear();

    for (int y = 0; y < engine::screen_size.y; y++) {
      for (int x = 0; x < engine::screen_size.x; x++) {
        auto matrixPixel = frame->at(x, y);
        circles[x][y].setFillColor(
            sf::Color(matrixPixel.r * 255, matrixPixel.g * 255,
                      matrixPixel.b * 255, matrixPixel.a * 255));
        window.draw(circles[x][y]);
      }
    }

    window.display();

    // std::ifstream statm("/proc/self/statm");
    // long size, resident;
    // statm >> size >> resident;
    // long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
    // std::cout << "Memory (MB): " << (resident * page_size_kb) / 1024.0 <<
    // std::endl;
    std::cout << "desktop set_on_frame_finished end "
              << std::to_string(frame_count) << std::endl;
  };

  engineObj.set_on_frame_finished(set_on_frame_finished_action);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      input_provider->sfEventQueue.push(event);
      if (event.type == sf::Event::Closed) window.close();
    }
  }

  engineObj.stop();
  return 0;
}
