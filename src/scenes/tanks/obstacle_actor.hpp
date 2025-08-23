#pragma once
#include "obstacle_type.hpp"
#include "../../actor.hpp"

class obstacle_actor : public actor {
    matrix matrix_;
    uint cell_size;
    std::vector<std::vector<bool> > is_taken;
    std::vector<std::vector<obstacle_type> > taken_by;
    int border_size;

public:
    obstacle_actor(const v2 &center, const uint cell_size, int border_size, int board_size);

    void redraw() override;
    matrix draw_one(v2 size, obstacle_type obstacle_type_);
    void generate_map(int board_size);
    obstacle_type does_collide(v2 other_from, v2 other_to);
    void remove_at(v2 other_from, v2 other_to, std::vector<obstacle_type> impacted_types, int level);
    v2 cell_to_pos(v2 cell);
    void generate_obstacle(int board_size, v2 at, obstacle_type type, int min_extra_rows, int max_extra_rows, int min_continue_for, int max_continue_for, std::vector<obstacle_type> override_types, std::vector<std::vector<obstacle_type> > &taken_by);
    obstacle_type randomize_obstacle_type();
    void print_taken_by_to_console();
    void print_is_taken_to_console();
    void update(float delta_time) override;
    matrix render() override;
    void fixed_update(float fixed_delta_time) override;
    ~obstacle_actor() override;
};
