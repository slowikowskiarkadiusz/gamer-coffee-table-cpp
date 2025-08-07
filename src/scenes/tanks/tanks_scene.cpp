#include "tanks_scene.hpp"

#include <numeric>
#include <random>

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

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    generate_half();

    // engine::set_timeout([]() {
    // engine::open_scene(std::make_shared<tanks_scene>());
    // }, 1000);
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

    // (std::find(excluded_cells.begin(), excluded_cells.end(), v2(x, y)) == excluded_cells.end())

    std::vector<std::vector<std::shared_ptr<obstacle_actor> > > taken_by = std::vector(board_size, std::vector<std::shared_ptr<obstacle_actor> >(board_size, nullptr));

    std::vector<std::shared_ptr<actor> > return_value;

    for (int x = 0; x < board_size - 1; ++x) {
        for (int y = 0; y < board_size / 2 - 1; ++y) {
            auto obstacle = randomize_obstacle_type();
            switch (obstacle) {
                case obstacle_type::none:
                    break;
                case obstacle_type::grass:
                    generate_obstacle(v2(x, y), obstacle_type::grass, 1, 999, 1, 999, {obstacle_type::brick}, taken_by);
                    break;
                case obstacle_type::brick:
                    generate_obstacle(v2(x, y), obstacle_type::brick, 1, 999, 1, 999, {}, taken_by);
                    break;
                case obstacle_type::steel:
                    generate_obstacle(v2(x, y), obstacle_type::steel, 0, 3, 0, 3, {obstacle_type::brick, obstacle_type::grass}, taken_by);
                    break;
                case obstacle_type::water:
                    generate_obstacle(v2(x, y), obstacle_type::water, 1, 3, 1, 3, {obstacle_type::brick, obstacle_type::grass, obstacle_type::steel}, taken_by);
                    break;
            }
        }
    }

    return return_value;
}

v2 tanks_scene::cell_to_pos(v2 cell) {
    return cell * cell_size + offset;
}

void tanks_scene::generate_obstacle(v2 at, obstacle_type type, int min_extra_rows, int max_extra_rows, int min_continue_for, int max_continue_for, std::vector<obstacle_type> override_types, std::vector<std::vector<std::shared_ptr<obstacle_actor> > > &taken_by) {
    if (at.x > 2 && at.y > 2 && type == obstacle_type::steel)
        auto a = 0;
    bool do_columns = std::rand() % 2;
    bool is_additional_row = max_extra_rows > 0 ? 1 : 0;
    int additional_rows = std::min(max_extra_rows, std::rand() % 4 + min_extra_rows);
    int continue_for = std::min(max_continue_for, std::rand() % int(do_columns ? (taken_by[0].size() / 2) - 1 - at.y : taken_by.size() - 2 - at.x) + min_continue_for);

    for (int ii = 0; ii < additional_rows; ++ii) {
        for (int i = 0; i < continue_for; ++i) {
            auto x = (do_columns ? ii : i) + at.x;
            auto y = (do_columns ? i : ii) + at.y;

            if (x >= taken_by.size() - 1 || y >= taken_by[0].size() - 1)
                continue;

            if (taken_by[x][y] != nullptr && std::ranges::find(override_types, taken_by[x][y]->get_obstacle_type()) != override_types.end()) {
                taken_by[x][y]->kill();
                taken_by[x][y] = nullptr;
                if (taken_by[board_size - 2 - x][board_size - 2 - y] != nullptr) {
                    taken_by[board_size - 2 - x][board_size - 2 - y]->kill();
                    taken_by[board_size - 2 - x][board_size - 2 - y] = nullptr;
                }
            }
            if (taken_by[x][y] == nullptr) {
                taken_by[x][y] = engine::instantiate<obstacle_actor>(cell_to_pos(v2(x, y)), cell_size, type);
                if (y < (taken_by[0].size() / 2) - 1) {
                    if (taken_by[board_size - 2 - x][board_size - 2 - y] != nullptr) {
                        taken_by[board_size - 2 - x][board_size - 2 - y]->kill();
                        taken_by[board_size - 2 - x][board_size - 2 - y] = nullptr;
                    }
                    taken_by[board_size - 2 - x][board_size - 2 - y] = engine::instantiate<obstacle_actor>(cell_to_pos(v2(board_size - 2 - x, board_size - 2 - y)), cell_size, type);
                }
            }
        }

        if (!is_additional_row)
            break;
    }
}

obstacle_type tanks_scene::randomize_obstacle_type() {
    std::vector<float> weights = {0.2, 0.1, 0.9, 0.2, 0.1};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(weights.begin(), weights.end());

    obstacle_type chosen = static_cast<obstacle_type>(dist(gen));

    return chosen;
}
