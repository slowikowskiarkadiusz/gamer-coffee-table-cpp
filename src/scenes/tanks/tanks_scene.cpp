#include "tanks_scene.hpp"

#include "tank_actor.hpp"
#include "../../engine.hpp"
#include "../../actors/border_actor.hpp"
#include "scenes/menu/menu_scene.hpp"

uint tanks_scene::size = 64;
uint tanks_scene::border_size = 10;
uint tanks_scene::cell_size = 4;
v2 tanks_scene::offset = v2::one() * 3.5;
uint tanks_scene::board_size;

tanks_scene::~tanks_scene() = default;

void tanks_scene::init() {
    engine::instantiate<border_actor>(v2::zero(), engine::screen_size - v2::one(), engine::screen_size, border_size);
    board_size = (size - border_size * 2) / cell_size;

    auto obstacle = engine::instantiate<obstacle_actor>(engine::screen_size / 2, 4, border_size, board_size + 1);
    obstacle->set_render_importance(1);

    auto on_tank_killed_func = [this](tank_actor *killed_tank) { on_tank_killed(killed_tank); };;

    tank1 = engine::instantiate<tank_actor>(true, obstacle);
    tank1->rotate(180);
    tank1->set_anchor_offset(v2(0, -2));
    tank1->set_center(cell_to_pos(v2(0, 0)));
    tank1->on_tank_killed = on_tank_killed_func;

    tank2 = engine::instantiate<tank_actor>(false, obstacle);
    tank2->set_anchor_offset(v2(0, -2));
    tank2->set_center(cell_to_pos(v2(board_size - 1, board_size - 1)));
    tank2->on_tank_killed = on_tank_killed_func;

    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

v2 tanks_scene::cell_to_pos(v2 cell) {
    return cell * cell_size + offset;
}

void tanks_scene::update(float delta_time) {
}

void tanks_scene::fixed_update(float delta_time) {
}

void tanks_scene::on_tank_killed(tank_actor *killed_tank) {
    std::string winner = killed_tank == tank1.get() ? "P1 WON" : "P2 WON";
    auto center = engine::screen_size / 2;
    auto winText = engine::instantiate<text_actor>(winner, v2::zero(), center);
    winText->set_render_importance(2);
    winText->move_to(center);
    engine::set_timeout([]() {
        engine::open_scene(std::make_shared<menu_scene>());
    }, 5000);
}
