//
// Created by Arkadiusz on 27/07/2025.
//

#pragma once

#include "../../scene.hpp"
#include "../../actors/rectangle-actor.hpp"

class menu_scene : public scene {
public:
    menu_scene() {
    }

    void update(float delta_time) override;

    void fixed_update(float delta_time) override {
        auto a = delta_time + 1;
    }
};
