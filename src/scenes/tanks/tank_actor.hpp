#pragma once
#include "bullet_actor.hpp"
#include "obstacle_actor.hpp"
#include "../../actor.hpp"
#include <functional>

class tank_actor : public actor {
    matrix visible_matrix_;
    matrix not_visible_matrix_;
    std::function<bool(v2 new_from, v2 new_to)> can_move_to;
    int health = 3;
    int level = 1;
    bool is_p1;
    std::shared_ptr<obstacle_actor> obstacle;
    float shoot_timer;
    float move_timer;
    std::weak_ptr<bullet_actor> curr_bullet;
    int is_visible = true;
    float blink_timer;

    bool get_is_blinking();

public:
    std::function<void(tank_actor *killed_tank)> on_tank_killed;

    tank_actor(bool is_p1, std::shared_ptr<obstacle_actor> obstacle);

    void redraw() override;
    void move(float delta_time);
    void shoot(float delta_time);
    void update(float delta_time) override;
    matrix render() override;
    void fixed_update(float fixed_delta_time) override;
    ~tank_actor() override;

    void level_up();
    void level_down();
    void take_damage();
    void set_can_move_to(std::function<bool(v2 new_from, v2 new_to)> func);
};
