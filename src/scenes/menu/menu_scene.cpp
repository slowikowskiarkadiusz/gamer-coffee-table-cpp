#include "menu_scene.hpp"
#include "../pong/pong_scene.hpp"
#include "../tetris/tetris_scene.hpp"
#include "../../input/input.hpp"

menu_scene::menu_scene() {
}

void menu_scene::init() {
    auto screen = engine::instance().screen_size;

    std::vector<std::string> titles = {"tetris", "pong 1", "pong 2", "music visualizer"};
    std::vector<std::function<std::shared_ptr<scene>()> > next_scenes = {
        [] { return std::make_shared<tetris_scene>(); },
        [] { return std::make_shared<pong_scene>(); },
        [] { return std::make_shared<pong_scene>(); },
        [] { return std::make_shared<pong_scene>(); }
    };

    for (size_t i = 0; i < titles.size(); ++i) {
        auto pos = v2(4, i * 6);
        auto size = v2(screen.x - 4, 5);
        auto text = text_actor::instantiate(titles[i], pos, size);
        _options.push_back({next_scenes[i], text, titles[i]});
    }

    auto opts = arrow_actor_opts{};
    opts.blink = true;
    _arrow = arrow_actor::instantiate(v2(1.5f, 2.5f), 5, opts);
    render();
}

void menu_scene::update(float delta_time) {
    bool changed = false;

    if (input::instance().is_key_down(key::P1_L_L) || input::instance().is_key_down(key::P1_R_R)) {
        _cursor_position++;
        changed = true;
    } else if (input::instance().is_key_down(key::P1_L_BLUE) || input::instance().is_key_down(key::P1_R_GREEN)) {
        _cursor_position--;
        changed = true;
    } else if (input::instance().is_key_down(key::START)) {
        std::cout << "HALO" << std::endl;
        // auto it = std::next(_options.begin(), _cursor_position);
        // engine::instance().open_scene(std::make_shared<controls_scene>(), it->second.next_scene());
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
