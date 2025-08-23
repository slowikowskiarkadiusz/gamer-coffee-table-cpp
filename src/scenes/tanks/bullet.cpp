//
// Created by Arkadiusz on 12/08/2025.
//

#include "bullet.hpp"
#include "obstacle_actor.hpp"
#include "../../engine.hpp"

bullet::bullet(const v2 &center, const v2 &direction, std::shared_ptr<obstacle_actor> obstacle)
    : actor("bullet", center, engine::screen_size / 32), matrix_(engine::screen_size / 32, color::white()), direction(direction), obstacle(obstacle) {
}

bullet::~bullet() {
}

void bullet::redraw() {
}

void bullet::update(float delta_time) {
    delta_time = delta_time / 1000;
    move_by(direction * speed * delta_time);

    if (obstacle->does_collide(_center - size() / 2, _center + size() / 2))
        kill();
}

matrix bullet::render() {
    return matrix_;
}

void bullet::fixed_update(float fixed_delta_time) {
}
