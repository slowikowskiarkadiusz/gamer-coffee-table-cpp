#include <functional>

#include "../../actors/rectangle_actor.hpp"
#include "../../actors/text_actor.hpp"
#include "esp32test_scene.hpp"

esp32test_scene::esp32test_scene() = default;

void esp32test_scene::init() {
  ball = engine::instantiate<rectangle_actor>(engine::screen_size / 2, v2(2, 2),
                                              color::white());
}

void esp32test_scene::update(float delta_time) {
  // std::cout << "aa!" << std::endl;
  if (input::is_key_down(key::P1_DOWN))
    // ESP_LOGI("APP", "key::P1_DOWN DOWN!");
    std::cout << "GPIO_NUM_35 DOWN!" << std::endl;
  if (input::is_key_down(key::P1_UP))
    // ESP_LOGI("APP", "key::P1_UP DOWN!");
    std::cout << "GPIO_NUM_36 DOWN!" << std::endl;
  if (input::is_key_down(key::P1_LEFT))
    // ESP_LOGI("APP", "key::P1_LEFT DOWN!");
    std::cout << "GPIO_NUM_37 DOWN!" << std::endl;
  if (input::is_key_down(key::P1_RIGHT))
    // ESP_LOGI("APP", "key::P1_RIGHT DOWN!");
    std::cout << "GPIO_NUM_39 DOWN!" << std::endl;

  if (input::is_key_up(key::P1_DOWN))
    // ESP_LOGI("APP", "key::P1_DOWN UP!");
    std::cout << "key::P1_DOWN UP!" << std::endl;
  if (input::is_key_up(key::P1_UP))
    // ESP_LOGI("APP", "key::P1_UP UP!");
    std::cout << "key::P1_UP UP!" << std::endl;
  if (input::is_key_up(key::P1_LEFT))
    // ESP_LOGI("APP", "key::P1_LEFT UP!");
    std::cout << "key::P1_LEFT UP!" << std::endl;
  if (input::is_key_up(key::P1_RIGHT))
    // ESP_LOGI("APP", "key::P1_RIGHT UP!");
    std::cout << "key::P1_RIGHT UP!" << std::endl;
}

void esp32test_scene::fixed_update(float fixed_delta_time) {}
