#include "bullet.hpp"
#include "obstacle_actor.hpp"
#include "../../engine.hpp"

bullet::bullet(const v2 &center, const v2 &direction, std::shared_ptr<obstacle_actor> obstacle, int level)
    : actor("bullet", center, engine::screen_size / 32), matrix_(engine::screen_size / 32, color::white()), direction(direction), obstacle(obstacle), level(level) {
}

bullet::~bullet() {
}

void bullet::redraw() {
}

void bullet::update(float delta_time) {
    delta_time = delta_time / 1000;
    move_by(direction * speed * delta_time);

    auto collides_with = obstacle->does_collide(_center.round() - size() / 2, _center.round() + size() / 2);

    switch (collides_with) {
        case obstacle_type::none:
            break;
        case obstacle_type::grass:
            if (level == 1)
                impact();
            break;
        case obstacle_type::brick:
            impact();
            kill();
            break;
        case obstacle_type::steel:
            impact();
            kill();
            break;
        case obstacle_type::water:
            if (level == 2)
                impact();
            break;
        case obstacle_type::edge:
            impact();
            kill();
            break;
    }
}

void bullet::impact() {
    obstacle->remove_at(_center - size() / 2 - v2::one() * 0.5, _center + size() / 2 + v2::one() * 0.5, {obstacle_type::brick, obstacle_type::grass}, level);
}

matrix bullet::render() {
    return matrix_;
}

void bullet::fixed_update(float fixed_delta_time) {
}
