#pragma once

#include <memory>
#include "../../engine.hpp"

class esp32test_scene : public scene {
public:
    esp32test_scene();
    void init() override;
    void update(float delta_time) override;
    void fixed_update(float fixed_delta_time) override;

private:
    std::shared_ptr<rectangle_actor> ball;
};
