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

    std::vector<std::vector<std::pair<std::shared_ptr<obstacle_actor>, std::shared_ptr<obstacle_actor> > > > taken_by = std::vector(board_size, std::vector<std::pair<std::shared_ptr<obstacle_actor>, std::shared_ptr<obstacle_actor> > >(board_size / 2, {nullptr, nullptr}));

    std::vector<std::shared_ptr<actor> > return_value;

    for (int x = 0; x < board_size - 1; ++x) {
        for (int y = 0; y < board_size / 2 - 1; ++y) {
            auto obstacle = randomize_obstacle_type();
            switch (obstacle) {
                case obstacle_type::none:
                    break;
                case obstacle_type::grass:
                    generate_grass(v2(x, y), taken_by);
                    break;
                case obstacle_type::brick:
                    generate_brick(v2(x, y), taken_by);
                    break;
                case obstacle_type::steel:
                    generate_steel(v2(x, y), taken_by);
                    break;
                case obstacle_type::water:
                    generate_water(v2(x, y), taken_by);
                    break;
            }
        }
    }

    return return_value;
}

v2 tanks_scene::cell_to_pos(v2 cell) {
    return cell * cell_size + offset;
}

void tanks_scene::generate_grass(v2 at, std::vector<std::vector<std::pair<std::shared_ptr<obstacle_actor>, std::shared_ptr<obstacle_actor> > > > &taken_by) {
    bool do_columns = std::rand() % 2;
    int is_additional_row = 1;
    int additional_rows = std::rand() % 4;
    int continue_for = std::rand() % int(do_columns ? taken_by[0].size() - 1 - at.y : taken_by.size() - 2 - at.x);

    float start = do_columns ? at.y : at.x;

    for (int ii = do_columns ? at.x : at.y; ii < additional_rows + (do_columns ? at.x : at.y) && ii < (do_columns ? taken_by[0].size() - 1 : taken_by.size() - 1); ++ii) {
        for (int i = start; i < continue_for && i < (do_columns ? taken_by[0].size() - 1 : taken_by.size() - 1); ++i) {
            auto x = do_columns ? ii : i;
            auto y = do_columns ? i : ii;
            if (taken_by[x][y].first == nullptr || taken_by[x][y].first->get_obstacle_type() == obstacle_type::brick) {
                if (taken_by[x][y].first != nullptr) {
                    taken_by[x][y].first->kill();
                    taken_by[x][y].first = nullptr;
                    taken_by[x][y].second->kill();
                    taken_by[x][y].second = nullptr;
                }
                taken_by[x][y].first = engine::instantiate<obstacle_actor>(cell_to_pos(v2(x, y)), cell_size, obstacle_type::grass);
                if (y < taken_by[0].size() - 1)
                    taken_by[x][y].second = engine::instantiate<obstacle_actor>(cell_to_pos(v2(board_size - x - 2, board_size - 2 - y)), cell_size, obstacle_type::grass);
            }
        }

        if (!is_additional_row)
            break;
    }
}

void tanks_scene::generate_brick(v2 at, std::vector<std::vector<std::pair<std::shared_ptr<obstacle_actor>, std::shared_ptr<obstacle_actor> > > > &taken_by) {
    bool do_columns = std::rand() % 2;
    int additional_rows = std::rand() % 4;
    int continue_for = std::rand() % int(do_columns ? taken_by[0].size() - 1 - at.y : taken_by.size() - 2 - at.x);

    float start = do_columns ? at.y : at.x;

    for (int ii = do_columns ? at.x : at.y; ii < additional_rows + (do_columns ? at.x : at.y) && ii < (do_columns ? taken_by[0].size() - 1 : taken_by.size() - 1); ++ii) {
        for (int i = start; i < continue_for && i < (do_columns ? taken_by[0].size() - 1 : taken_by.size() - 1); ++i) {
            auto x = do_columns ? ii : i;
            auto y = do_columns ? i : ii;
            if (taken_by[x][y].first == nullptr) {
                taken_by[x][y].first = engine::instantiate<obstacle_actor>(cell_to_pos(v2(x, y)), cell_size, obstacle_type::brick);
                if (y < taken_by[0].size() - 1)
                    taken_by[x][y].second = engine::instantiate<obstacle_actor>(cell_to_pos(v2(board_size - x - 2, board_size - 2 - y)), cell_size, obstacle_type::brick);
            }
        }
    }
}

void tanks_scene::generate_steel(v2 at, std::vector<std::vector<std::pair<std::shared_ptr<obstacle_actor>, std::shared_ptr<obstacle_actor> > > > &taken_by) {
    bool do_columns = std::rand() % 2;
    int additional_rows = std::max(std::rand() % 4, 3);
    int continue_for = std::max(std::rand() % int(do_columns ? taken_by[0].size() - 1 - at.y : taken_by.size() - 2 - at.x), 3);

    float start = do_columns ? at.y : at.x;

    for (int ii = do_columns ? at.x : at.y; ii < additional_rows + (do_columns ? at.x : at.y) && ii < (do_columns ? taken_by[0].size() - 1 : taken_by.size() - 1); ++ii) {
        for (int i = start; i < continue_for && i < (do_columns ? taken_by[0].size() - 1 : taken_by.size() - 1); ++i) {
            auto x = do_columns ? ii : i;
            auto y = do_columns ? i : ii;
            if (taken_by[x][y].first == nullptr) {
                taken_by[x][y].first = engine::instantiate<obstacle_actor>(cell_to_pos(v2(x, y)), cell_size, obstacle_type::brick);
                if (y < taken_by[0].size() - 1)
                    taken_by[x][y].second = engine::instantiate<obstacle_actor>(cell_to_pos(v2(board_size - x - 2, board_size - 2 - y)), cell_size, obstacle_type::brick);
            }
        }
    }
}

void tanks_scene::generate_water(v2 at, std::vector<std::vector<std::pair<std::shared_ptr<obstacle_actor>, std::shared_ptr<obstacle_actor> > > > &taken_by) {
}

obstacle_type tanks_scene::randomize_obstacle_type() {
    std::vector<int> weights = {3, 2, 4, 2, 1};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(weights.begin(), weights.end());

    obstacle_type chosen = static_cast<obstacle_type>(dist(gen));

    // std::cout << int(chosen) << std::endl;

    return chosen;
}
