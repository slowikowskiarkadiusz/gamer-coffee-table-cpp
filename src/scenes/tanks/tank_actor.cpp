//
// Created by Arkadiusz on 04/08/2025.
//

#include "tank_actor.hpp"

#include "../../input/gestures.hpp"

tank_actor::tank_actor(bool is_p1, std::shared_ptr<obstacle_actor> obstacle): actor("tank", v2::zero(), v2::one() * 4), matrix_(4, 4) {
    tank_actor::redraw();
    this->is_p1 = is_p1;
    this->obstacle = obstacle;
}

void tank_actor::redraw() {
    matrix_ = {4, 8};
    for (int x = 0; x < 4; ++x) {
        for (int y = level < 2 ? 2 : 1; y < 4; ++y) {
            matrix_.set_pixel(x, y + 4, color::white());
        }
    }

    if (level < 2) {
        matrix_.set_pixel(1, 5, color::white());
        matrix_.set_pixel(2, 5, color::white());
    }
    if (level > 0) {
        matrix_.set_pixel(1, 4, color::white());
        matrix_.set_pixel(2, 4, color::white());
    }
}

void tank_actor::update(float delta_time) {
    v2 by = v2::zero();
    int rotation_to = _rotation;
    if (gestures::is(is_p1 ? key::P1_UP : key::P2_UP, state::down, gesture::once)
        || gestures::is(is_p1 ? key::P1_UP : key::P2_UP, state::press, gesture::repeating)) {
        rotation_to = ((is_p1 ? 180 : 0) + 0);
        by.y = -1 * (is_p1 ? -1 : 1);
    } else if (gestures::is(is_p1 ? key::P1_DOWN : key::P2_DOWN, state::down, gesture::once)
               || gestures::is(is_p1 ? key::P1_DOWN : key::P2_DOWN, state::press, gesture::repeating)) {
        rotation_to = ((is_p1 ? 180 : 0) + 180);
        by.y = 1 * (is_p1 ? -1 : 1);
    } else if (gestures::is(is_p1 ? key::P1_LEFT : key::P2_LEFT, state::down, gesture::once)
               || gestures::is(is_p1 ? key::P1_LEFT : key::P2_LEFT, state::press, gesture::repeating)) {
        rotation_to = ((is_p1 ? 180 : 0) + 270);
        by.x = -1 * (is_p1 ? -1 : 1);
    } else if (gestures::is(is_p1 ? key::P1_RIGHT : key::P2_RIGHT, state::down, gesture::once)
               || gestures::is(is_p1 ? key::P1_RIGHT : key::P2_RIGHT, state::press, gesture::repeating)) {
        rotation_to = ((is_p1 ? 180 : 0) + 90);
        by.x = 1 * (is_p1 ? -1 : 1);
    }

    set_rotation(rotation_to);
    if (by != v2::zero()) {
        if (!obstacle->does_collide((by + this->_center - size() / 2 + v2::one() * 0.5), (by + this->_center + size() / 2))) {
            this->_center = this->_center + by;
        }
    }
}

matrix tank_actor::render() {
    return matrix_;
}

void tank_actor::fixed_update(float fixed_delta_time) {
}

tank_actor::~tank_actor() {
}

void tank_actor::level_up() {
    level++;
    level = std::min(2, level);
    redraw();
}

void tank_actor::level_down() {
    level--;
    level = std::max(0, level);
    redraw();
}

void tank_actor::set_can_move_to(std::function<bool(v2 new_from, v2 new_to)> func) {
    can_move_to = func;
}
