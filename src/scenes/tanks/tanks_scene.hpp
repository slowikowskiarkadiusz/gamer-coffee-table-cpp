#pragma once
#include "obstacle_actor.hpp"
#include "obstacle_type.hpp"
#include "../../scene.hpp"

class tanks_scene : public scene {
    matrix border_matrix;
    matrix board_matrix;
    static uint size;
    static uint border_size;
    static uint cell_size;
    static v2 offset;
    static uint board_size;

    v2 cell_to_pos(v2 cell);
    void generate_grass(v2 at, std::vector<std::vector<std::pair<std::shared_ptr<obstacle_actor>, std::shared_ptr<obstacle_actor> > > > &taken_by);
    void generate_brick(v2 at, std::vector<std::vector<std::pair<std::shared_ptr<obstacle_actor>, std::shared_ptr<obstacle_actor> > > > &taken_by);
    void generate_steel(v2 at, std::vector<std::vector<std::pair<std::shared_ptr<obstacle_actor>, std::shared_ptr<obstacle_actor> > > > &taken_by);
    void generate_water(v2 at, std::vector<std::vector<std::pair<std::shared_ptr<obstacle_actor>, std::shared_ptr<obstacle_actor> > > > &taken_by);
    obstacle_type randomize_obstacle_type();
    std::vector<std::shared_ptr<actor> > generate_half();

public:
    tanks_scene(): border_matrix(matrix(size, size)), board_matrix(matrix(size, size)) {
    }

    ~tanks_scene() override;
    void init() override;
    void update(float delta_time) override;
    void fixed_update(float delta_time) override;
};
