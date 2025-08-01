#pragma once

#include "../../scene.hpp"
#include "../../engine.hpp"
#include "../../input/input.hpp"
#include "../../color.hpp"
#include "../../v2.hpp"
#include "../../actors/rectangle_actor.hpp"
#include "../../actors/text_actor.hpp"
#include "../../actors/arrow_actor.hpp"
#include "button_icon_actor.hpp"
#include <map>
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <functional>

constexpr float button_size = 5.0f;

struct controls_data {
    std::vector<key> keys;
    std::string text;
};

class controls_scene : public scene {
    bool can_proceed = false;
    std::shared_ptr<rectangle_actor> divider_actor;
    std::vector<std::vector<controls_data> > pages;
    size_t page_index = 0;
    std::function<std::shared_ptr<scene>()> next_scene;
    std::string next_scene_name;

    static std::map<std::string, std::vector<controls_data> > controls;

public:
    explicit controls_scene(std::function<std::shared_ptr<scene>()> next_scene_factory, std::string next_scene_name)
        : next_scene(std::move(next_scene_factory)), next_scene_name(std::move(next_scene_name)) {
    }

    void init() override {
        auto screen = engine::screen_size;
        divider_actor = engine::instantiate<rectangle_actor>(screen / 2, v2(screen.x, 2), color::white(0.5f));

        engine::set_timeout([this]() {
            can_proceed = true;
            auto create_arrow = [](bool is_p1) {
                auto pos = engine::screen_size - v2::one() * 1.5f;
                if (is_p1) pos.y -= engine::screen_size.y / 2;
                auto arrow = engine::instantiate<arrow_actor>(pos, 3.0f, arrow_actor_opts{.blink = true});
                if (is_p1) {
                    auto rot_center = engine::screen_size / 2 - v2::one();
                    rot_center.y -= engine::screen_size.y / 4;
                    arrow->rotate_around(rot_center, 180);
                }
                return arrow;
            };
            create_arrow(true);
            create_arrow(false);
        }, 2000);

        auto it = controls.find(next_scene_name);
        if (it != controls.end())
            pages = paginate(it->second, 2);

        print_page();
    }

    void update(float) override {
        if (can_proceed && input::is_any_key_down()) {
            engine::open_scene(next_scene());
        }
    }

    void fixed_update(float) override {
    }

private:
    static std::vector<std::vector<controls_data> > paginate(const std::vector<controls_data> &items, size_t page_size) {
        std::vector<std::vector<controls_data> > result;
        size_t i = 0;
        while (i < items.size()) {
            result.emplace_back(items.begin() + i, items.begin() + std::min(i + page_size, items.size()));
            i += page_size;
        }
        return result;
    }

    void print_page() {
        if (pages.size() == 0)
            return;

        auto &current = pages[page_index];
        for (int is_p1 = 0; is_p1 <= 1; ++is_p1) {
            std::vector<std::shared_ptr<actor> > current_page_actors;

            for (int i = 0; i < current.size(); ++i) {
                float y = engine::screen_size.y / 2 - (button_size + 1) * (i + 1) + (is_p1 ? 0 : engine::screen_size.y / 2);
                float x = 0;
                for (auto k: current[current.size() - 1 - i].keys) {
                    auto icon = engine::instantiate<button_icon_actor>(v2((button_size / 2) + x, y), button_size, button_icon_actor_opts{k});
                    current_page_actors.push_back(icon);
                    x += (button_size + 1);
                }
                auto text = engine::instantiate<text_actor>(current[current.size() - 1 - i].text, v2(x, y - (button_size / 2)), v2(engine::screen_size.x - x, button_size));
                current_page_actors.push_back(text);
            }
            if (is_p1) {
                auto rot_center = engine::screen_size / 2 - v2::one();
                rot_center.y -= engine::screen_size.y / 4;
                for (auto &actor: current_page_actors)
                    actor->rotate_around(rot_center, 180);
            }
        }

        engine::set_timeout([this]() {
            page_index = (page_index + 1) % pages.size();
            print_page();
        }, 1600);
    }
};
