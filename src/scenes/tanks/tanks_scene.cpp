#include "tanks_scene.hpp"

#include "tank_actor.hpp"
#include "../../engine.hpp"
#include "../../actors/border_actor.hpp"

uint tanks_scene::size = 64;
uint tanks_scene::border_size = 2;

tanks_scene::~tanks_scene() = default;

void tanks_scene::init() {
    engine::instantiate<border_actor>(v2::zero(), engine::screen_size - v2::one(), engine::screen_size, border_size);

    engine::instantiate<tank_actor>();


}

void tanks_scene::update(float delta_time) {
}

void tanks_scene::fixed_update(float delta_time) {
}
