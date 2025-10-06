#include "tank_actor.hpp"
#include "bullet_actor.hpp"
#include "../../engine.hpp"
#include "../../input/gestures.hpp"

constexpr float shoot_cooldown = 250;
constexpr float move_cooldown = 50;
constexpr float blinking_offsets[] = {0, 100, 250, 0};

tank_actor::tank_actor(bool is_p1,
                       std::shared_ptr<obstacle_actor> obstacle): actor("tank", v2::zero(), v2::one() * 4),
                                                                  visible_matrix_(4, 8),
                                                                  not_visible_matrix_(4, 8) {
    tank_actor::redraw();
    this->is_p1 = is_p1;
    this->obstacle = obstacle;
}

bool tank_actor::get_is_blinking() {
    return health < 3;
}

void tank_actor::redraw() {
    visible_matrix_ = {4, 8};
    not_visible_matrix_ = {4, 8};
    for (int x = 0; x < 4; ++x) {
        for (int y = level < 2 ? 2 : 1; y < 4; ++y) {
            visible_matrix_.set_pixel(x, y + 4, color::white());
            not_visible_matrix_.set_pixel(x, y + 4, color::white(0.65));
        }
    }

    if (level < 2) {
        visible_matrix_.set_pixel(1, 5, color::white());
        not_visible_matrix_.set_pixel(1, 5, color::white(0.65));
        visible_matrix_.set_pixel(2, 5, color::white());
        not_visible_matrix_.set_pixel(2, 5, color::white(0.65));
    }
    if (level > 0) {
        visible_matrix_.set_pixel(1, 4, color::white());
        not_visible_matrix_.set_pixel(1, 4, color::white(0.65));
        visible_matrix_.set_pixel(2, 4, color::white());
        not_visible_matrix_.set_pixel(2, 4, color::white(0.65));
    }
}

void tank_actor::move(float delta_time) {
    if (move_timer > 0)
        move_timer -= delta_time;
    if (move_timer <= 0) {
        v2 by = v2::zero();
        int rotation_to = _rotation;
        if (input::is_key_press(is_p1 ? key::P1_UP : key::P2_UP)) {
            rotation_to = ((is_p1 ? 180 : 0) + 0);
            by.y = -1 * (is_p1 ? -1 : 1);
        } else if (input::is_key_press(is_p1 ? key::P1_DOWN : key::P2_DOWN)) {
            rotation_to = ((is_p1 ? 180 : 0) + 180);
            by.y = 1 * (is_p1 ? -1 : 1);
        } else if (input::is_key_press(is_p1 ? key::P1_LEFT : key::P2_LEFT)) {
            rotation_to = ((is_p1 ? 180 : 0) + 270);
            by.x = -1 * (is_p1 ? -1 : 1);
        } else if (input::is_key_press(is_p1 ? key::P1_RIGHT : key::P2_RIGHT)) {
            rotation_to = ((is_p1 ? 180 : 0) + 90);
            by.x = 1 * (is_p1 ? -1 : 1);
        }

        set_rotation(rotation_to);
        if (by != v2::zero()) {
            move_timer = move_cooldown;
            //std::cout << "moving" << std::endl;
            auto passable = {obstacle_type::none, obstacle_type::grass};
            if (std::ranges::find(passable, obstacle->does_collide((by + this->_center - size() / 2 + v2::one() * 0.5), (by + this->_center + size() / 2 + v2::one() * 0.5))) != passable.end()) {
                this->_center = this->_center + by;
            }
        } else {
            //std::cout << "not moving" << std::endl;
        }
    }
}

void tank_actor::shoot(float delta_time) {
    if (shoot_timer > 0)
        shoot_timer -= delta_time;
    if (shoot_timer <= 0 && curr_bullet.expired() && gestures::is(is_p1 ? key::P1_BLUE : key::P2_BLUE, state::down, gesture::once)) {
        shoot_timer = shoot_cooldown;
        v2 offset(0, 0);
        if (get_rotation() == (is_p1 ? 180 : 0) + 0) {
            offset.y = -1 * (is_p1 ? -1 : 1);
        } else if (get_rotation() == ((is_p1 ? 180 : 0) + 180) % 360) {
            offset.y = 1 * (is_p1 ? -1 : 1);
        } else if (get_rotation() == ((is_p1 ? 180 : 0) + 270) % 360) {
            offset.x = -1 * (is_p1 ? -1 : 1);
        } else if (get_rotation() == (is_p1 ? 180 : 0) + 90) {
            offset.x = 1 * (is_p1 ? -1 : 1);
        }

        auto origin = _center + offset * 2;

        curr_bullet = engine::instantiate<bullet_actor>(origin, offset, obstacle, level, this);
    }
}

void tank_actor::update(float delta_time) {
    move(delta_time);

    shoot(delta_time);

    if (get_is_blinking()) {
        blink_timer -= delta_time;
        if (blink_timer <= 0.f) {
            is_visible = !is_visible;
            blink_timer = blinking_offsets[health];
        }
    }
}

matrix tank_actor::render() {
    return is_visible ? visible_matrix_ : not_visible_matrix_;
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

void tank_actor::take_damage() {
    health--;
}

void tank_actor::set_can_move_to(std::function<bool(v2 new_from, v2 new_to)> func) {
    can_move_to = func;
}
