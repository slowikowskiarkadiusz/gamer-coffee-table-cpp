#pragma once
#include "obstacle_actor.hpp"
#include "../../actor.hpp"

class bullet_actor : public actor {
    matrix matrix_;
    v2 direction;
    std::shared_ptr<obstacle_actor> obstacle;
    const float speed = 20;
    int level;
    tank_actor * creator;

public:
    bullet_actor(const v2 &center, const v2 &direction, std::shared_ptr<obstacle_actor> obstacle, int level, tank_actor* creator);

    ~bullet_actor() override;
    void redraw() override;
    void update(float delta_time) override;
    void impact();
    matrix render() override;
    void fixed_update(float fixed_delta_time) override;
};
