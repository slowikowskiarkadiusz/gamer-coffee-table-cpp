#pragma once
#include "../../actor.hpp"

class tank_actor : public actor {
    matrix matrix_;
    std::function<bool(v2 new_from, v2 new_to)> can_move_to;
    int level;

public:
    tank_actor();

    void redraw() override;
    void update(float delta_time) override;
    matrix render() override;
    void fixed_update(float fixed_delta_time) override;
    ~tank_actor() override;

    void level_up();
    void level_down();
};
