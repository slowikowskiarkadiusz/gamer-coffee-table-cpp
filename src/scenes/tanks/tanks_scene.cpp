#include "tanks_scene.hpp"

#include "tank_actor.hpp"
#include "../../engine.hpp"
#include "../../actors/border_actor.hpp"

uint tanks_scene::size = 64;
uint tanks_scene::border_size = 2;
uint tanks_scene::cell_size = 4;
v2 tanks_scene::offset = v2::one() * 3.5;
uint tanks_scene::board_size;

tanks_scene::~tanks_scene() = default;

void tanks_scene::init() {
    engine::instantiate<border_actor>(v2::zero(), engine::screen_size - v2::one(), engine::screen_size, border_size);
    board_size = (size - border_size * 2) / cell_size;

    auto obstacle = engine::instantiate<obstacle_actor>(engine::screen_size / 2, 4, border_size);
    obstacle->generate_map(board_size + 1);

    tank1 = engine::instantiate<tank_actor>(true, obstacle);
    tank1->rotate(180);
    tank1->set_anchor_offset(v2(0, -2));
    tank1->set_center(cell_to_pos(v2(0, 0)));

    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

v2 tanks_scene::cell_to_pos(v2 cell) {
    return cell * cell_size + offset;
}

void tanks_scene::update(float delta_time) {
    // if (gestures::is(key::P1_UP, state::down, gesture::once)) {
    // tank1->level_up();
    // }
    // if (gestures::is(key::P1_DOWN, state::down, gesture::once)) {
    // tank1->level_down();
    // }
}

void tanks_scene::fixed_update(float delta_time) {
}
