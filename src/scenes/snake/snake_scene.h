#pragma once

#include "../../scene.hpp"

class snake_scene : public scene {
public:
    void init() override;
    void update(float delta_time) override;
    void fixed_update(float delta_time) override;
};
