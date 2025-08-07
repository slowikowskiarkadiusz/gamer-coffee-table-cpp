#pragma once
#include "obstacle_actor.hpp"
#include "obstacle_type.hpp"
#include "tank_actor.hpp"
#include "../../scene.hpp"

class tanks_scene : public scene {
    matrix border_matrix;
    matrix board_matrix;
    std::shared_ptr<tank_actor> tank1;
    std::shared_ptr<tank_actor> tank2;
    static uint size;
    static uint border_size;
    static uint cell_size;
    static v2 offset;
    static uint board_size;

    v2 cell_to_pos(v2 cell);
    void generate_obstacle(v2 at, obstacle_type type, int min_extra_rows, int max_extra_rows, int min_continue_for, int max_continue_for, std::vector<obstacle_type> override_types, std::vector<std::vector<std::shared_ptr<obstacle_actor>>> &taken_by);
    obstacle_type randomize_obstacle_type();
    void generate_map();

public:
    tanks_scene(): border_matrix(matrix(size, size)), board_matrix(matrix(size, size)) {
    }

    ~tanks_scene() override;
    void init() override;
    void update(float delta_time) override;
    void fixed_update(float delta_time) override;
};
