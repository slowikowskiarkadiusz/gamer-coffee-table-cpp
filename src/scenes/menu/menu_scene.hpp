#pragma once

#include "../../scene.hpp"
#include "../../actors/arrow_actor.hpp"
#include "../../actors/text_actor.hpp"
#include <map>
#include <memory>
#include <string>

class engine;

struct menu_option {
    std::function<std::shared_ptr<scene>()> next_scene;
    std::shared_ptr<text_actor> actor;
    std::string title;
};

class menu_scene : public scene {
    int _cursor_position = 0;
    std::vector<menu_option> _options;
    std::shared_ptr<arrow_actor> _arrow;

public:
    menu_scene();

    void init() override;
    void update(float delta_time) override;
    void fixed_update(float delta_time) override;

    void render();

    ~menu_scene() override {
    }
};
