#pragma once
#include "obstacle_type.hpp"
#include "../../actor.hpp"

class obstacle_actor : public actor {
    matrix matrix_;
    obstacle_type obstacle_type_;

public:
    obstacle_actor(const v2 &center, const uint cell_size, const obstacle_type _obstacle_type)
        : actor("obstacle", center, v2::one() * cell_size), matrix_(v2::one() * cell_size), obstacle_type_(_obstacle_type) {
        redraw();
    }

    obstacle_type get_obstacle_type() {
        return obstacle_type_;
    }

    void redraw() override;
    void update(float delta_time) override;
    matrix render() override;
    void fixed_update(float fixed_delta_time) override;
    ~obstacle_actor() override;
};
