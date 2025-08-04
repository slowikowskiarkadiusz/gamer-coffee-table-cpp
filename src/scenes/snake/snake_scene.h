#pragma once

#include "../../scene.hpp"

class snake_scene : public scene {
    matrix board;

public:
    explicit snake_scene()
        : board(matrix(32, 32)) {

    }

    void init() override;
    void update(float delta_time) override;
    void fixed_update(float delta_time) override;

    ~snake_scene() override;
};
