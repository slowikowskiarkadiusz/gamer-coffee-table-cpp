#pragma once
#include "../../scene.hpp"

class tanks_scene : public scene {
    matrix border_matrix;
    matrix board_matrix;
    static uint size;
    static uint border_size;

public:
    tanks_scene(): border_matrix(matrix(size, size)), board_matrix(matrix(size, size)) {
    }

    ~tanks_scene() override;
    void init() override;
    void update(float delta_time) override;
    void fixed_update(float delta_time) override;
};
