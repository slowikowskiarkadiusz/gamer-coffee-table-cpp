//
// Created by Arkadiusz on 04/08/2025.
//

#include "tank_actor.hpp"

tank_actor::tank_actor(): actor("tank", v2::zero(), v2::one() * 4), matrix_(4, 8) {
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            matrix_.set_pixel(x, y + 4, color::white());
        }
    }

    matrix_.set_pixel(1, 3, color::white());
    matrix_.set_pixel(2, 3, color::white());
    matrix_.set_pixel(1, 2, color::white());
    matrix_.set_pixel(2, 2, color::white());
}

void tank_actor::redraw() {
}

void tank_actor::update(float delta_time) {
}

matrix tank_actor::render() {
    return matrix_;
}

void tank_actor::fixed_update(float fixed_delta_time) {
}

tank_actor::~tank_actor() {
}
