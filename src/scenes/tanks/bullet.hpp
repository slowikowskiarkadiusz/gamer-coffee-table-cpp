#pragma once
#include "obstacle_actor.hpp"
#include "../../actor.hpp"

class bullet : public actor {
    matrix matrix_;
    v2 direction;
    std::shared_ptr<obstacle_actor> obstacle;
    const float speed = 20;

public:
    bullet(const v2 &center, const v2 &direction, std::shared_ptr<obstacle_actor> obstacle);

    ~bullet() override;
    void redraw() override;
    void update(float delta_time) override;
    matrix render() override;
    void fixed_update(float fixed_delta_time) override;
};
