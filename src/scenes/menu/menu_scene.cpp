#include "menu_scene.hpp"
#include "../pong/pong_scene.hpp"
#include "../tetris/tetris_scene.hpp"
#include "../tanks/tanks_scene.hpp"
#include "../controls/controls_scene.hpp"
#include "../../input/input.hpp"

menu_scene::menu_scene() = default;

void menu_scene::init() {
    auto screen = engine::screen_size;

    std::vector<std::string> titles = {"tanks", "tetris", "pong 1", "pong 2", "music visualizer"};
    std::vector<std::pair<std::function<std::shared_ptr<scene>()>, std::string> > next_scenes = {
        {[] { return std::make_shared<tanks_scene>(); }, "tanks_scene"},
        {[] { return std::make_shared<tetris_scene>(); }, "tetris_scene"},
        {[] { return std::make_shared<pong_scene>(); }, "pong_scene"},
        {[] { return std::make_shared<pong_scene>(); }, "pong_scene"},
        {[] { return std::make_shared<pong_scene>(); }, "pong_scene"},
    };

    for (size_t i = 0; i < titles.size(); ++i) {
        auto pos = v2(4, i * 6);
        auto size = v2(screen.x - 4, 5);
        auto text = engine::instantiate<text_actor>(titles[i], pos, size);
        _options.push_back({next_scenes[i].first, text, titles[i], next_scenes[i].second});
    }

    auto opts = arrow_actor_opts{};
    opts.blink = true;
    _arrow = engine::instantiate<arrow_actor>(v2(1.5f, 2.5f), 5, opts);
    render();
}

void menu_scene::update(float delta_time) {
    bool changed = false;

    if (input::is_key_down(key::P1_L_L) || input::is_key_down(key::P1_R_R)) {
        _cursor_position++;
        changed = true;
    } else if (input::is_key_down(key::P1_L_BLUE) || input::is_key_down(key::P1_R_GREEN)) {
        _cursor_position--;
        changed = true;
    } else if (input::is_key_down(key::START)) {
        auto it = std::next(_options.begin(), _cursor_position);
        engine::open_scene(std::make_shared<controls_scene>(it->next_scene, it->scene_name));
    }

    int option_count = static_cast<int>(_options.size());
    if (_cursor_position < 0) _cursor_position = option_count - 1;
    _cursor_position %= option_count;

    if (changed) {
        int i = 0;
        for (auto &opt: _options) {
            opt.actor->set_animation(i == _cursor_position);
            i++;
        }
        _arrow->reset_blinking();
        render();
    }
}

void menu_scene::fixed_update(float) {
}

void menu_scene::render() {
    _arrow->move_to(v2(1.5f, 2.5f + _cursor_position * 6));
}
