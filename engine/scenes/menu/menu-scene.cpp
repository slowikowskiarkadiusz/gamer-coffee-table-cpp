//
// Created by Arkadiusz on 27/07/2025.
//

#include "menu-scene.hpp"
#include "../../engine.hpp"

bool actorExists = false;

void menu_scene::update(float delta_time) {
    if (!actorExists) {
        auto rect = rectangle_actor::instantiate(engine::instance().screen_size / 2, v2(2, 2), color::red());
        actorExists = true;
    }
}
