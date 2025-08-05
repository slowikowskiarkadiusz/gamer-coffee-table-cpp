#include "tanks_scene.hpp"

#include "obstacle_actor.hpp"
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

    board_size = (size - board_size * 2) / cell_size;

    auto tank = engine::instantiate<tank_actor>();
    tank->set_anchor_offset(v2(0, -2));
    tank->set_center(v2(((board_size - 1) / 2 - 1) * cell_size, (board_size - 1) * cell_size) + offset);

    generate_half();
}

void tanks_scene::update(float delta_time) {
}

void tanks_scene::fixed_update(float delta_time) {
}

std::vector<std::shared_ptr<actor> > tanks_scene::generate_half() {
    const std::vector<v2> excluded_cells = {
        v2(board_size / 2, 0),
        v2(board_size / 2 - 1, 0), v2(board_size / 2 + 1, 0),
        v2(board_size / 2 + 2, 0), v2(board_size / 2 - 2, 0),
        v2(board_size / 2 + 3, 0), v2(board_size / 2 - 3, 0)
    };

    auto no_row_corridors = std::rand() % 5;
    auto no_column_corridors = std::rand() % 5;

    std::vector<std::shared_ptr<actor> > return_value;

    for (int x = 0; x < board_size - 1; ++x) {
        for (int y = 1; y < board_size / 2 - 1; ++y) {
            return_value.push_back(engine::instantiate<obstacle_actor>(cell_to_pos(v2(x, y)), cell_size, obstacle_type::brick));
        }
    }

    return return_value;
}

v2 tanks_scene::cell_to_pos(v2 cell) {
    return cell * cell_size + offset;
}
