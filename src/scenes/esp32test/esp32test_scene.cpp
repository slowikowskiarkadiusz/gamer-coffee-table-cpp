#include <functional>

#include "../../actors/rectangle_actor.hpp"
#include "../../actors/text_actor.hpp"
#include "esp32test_scene.hpp"

esp32test_scene::esp32test_scene() = default;

void esp32test_scene::init() {
    ball = engine::instantiate<rectangle_actor>(engine::screen_size / 2, v2(2, 2), color::white());
}

void esp32test_scene::update(float delta_time) {}

void esp32test_scene::fixed_update(float fixed_delta_time) {}
