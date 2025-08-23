#include "obstacle_actor.hpp"
#include "../../engine.hpp"
#include <random>

void obstacle_actor::redraw() {
}

matrix obstacle_actor::draw_one(v2 size, obstacle_type obstacle_type_) {
    matrix matrix_ = matrix(size.x, size.y);
    auto grass = color(168.0 / 255.0, 230.0 / 255.0, 29.0 / 255.0);
    auto light_brick = color(156.0 / 255.0, 90.0 / 255.0, 60.0 / 255.0);
    auto dark_brick = color(138.0 / 255.0, 63.0 / 255.0, 30.0 / 255.0);
    auto third_brick = color(138.0 / 255.0, 87.0 / 255.0, 65.0 / 255.0);
    auto light_steel = color(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
    auto dark_steel = color(70.0 / 255.0, 70.0 / 255.0, 70.0 / 255.0);
    auto light_water = color(153.0 / 255.0, 217.0 / 255.0, 234.0 / 255.0);
    auto dark_water = color(77.0 / 255.0, 109.0 / 255.0, 243.0 / 255.0);

    switch (obstacle_type_) {
        case obstacle_type::none:
            break;
        case obstacle_type::grass:
            for (int y = 0; y < matrix_.height(); ++y) {
                auto is_y_odd = y % 2 == 1;
                for (int x = 0; x < matrix_.width(); ++x) {
                    auto is_x_odd = x % 2 == 1;
                    if (is_y_odd == is_x_odd)
                        matrix_.set_pixel(x, y, grass);
                }
            }
            break;
        case obstacle_type::brick:
            for (int y = 0; y < matrix_.height(); ++y) {
                auto is_odd = y % 2 == 1;
                for (int x = 0; x < matrix_.width(); ++x) {
                    if (is_odd && x == matrix_.width() - 1 || !is_odd && x == 0)
                        matrix_.set_pixel(x, y, third_brick);
                    else
                        matrix_.set_pixel(x, y, is_odd ? light_brick : dark_brick);
                }
            }
            break;
        case obstacle_type::steel:
            for (int y = 0; y < matrix_.height(); ++y) {
                for (int x = 0; x < matrix_.width(); ++x) {
                    if (x == 0 || x == matrix_.width() - 1 || y == 0 || y == matrix_.height() - 1)
                        matrix_.set_pixel(x, y, light_steel);
                    else
                        matrix_.set_pixel(x, y, dark_steel);
                }
            }
            break;
        case obstacle_type::water:
            for (int y = 0; y < matrix_.height(); ++y) {
                auto is_y_odd = y % 2 == 1;
                for (int x = 0; x < matrix_.width(); ++x) {
                    auto is_x_odd = x % 2 == 1;
                    if (is_y_odd == is_x_odd)
                        matrix_.set_pixel(x, y, light_water);
                    else
                        matrix_.set_pixel(x, y, dark_water);
                }
            }
            break;
    }

    return matrix_;
}


void obstacle_actor::generate_map(int board_size, int border_size) {
    // (std::find(excluded_cells.begin(), excluded_cells.end(), v2(x, y)) == excluded_cells.end())

    taken_by = std::vector(board_size, std::vector<obstacle_type>(board_size, obstacle_type::none));

    for (int x = 0; x < board_size; ++x) {
        for (int y = 0; y < board_size / 2; ++y) {
            auto obstacle = randomize_obstacle_type();
            switch (obstacle) {
                case obstacle_type::none:
                    break;
                case obstacle_type::grass:
                    generate_obstacle(board_size, v2(x, y), obstacle_type::grass, 1, 999, 1, 999, {obstacle_type::brick}, taken_by);
                    break;
                case obstacle_type::brick:
                    generate_obstacle(board_size, v2(x, y), obstacle_type::brick, 1, 999, 1, 999, {}, taken_by);
                    break;
                case obstacle_type::steel:
                    generate_obstacle(board_size, v2(x, y), obstacle_type::steel, 0, 3, 0, 3, {obstacle_type::brick, obstacle_type::grass}, taken_by);
                    break;
                case obstacle_type::water:
                    generate_obstacle(board_size, v2(x, y), obstacle_type::water, 1, 3, 1, 3, {obstacle_type::brick, obstacle_type::grass, obstacle_type::steel}, taken_by);
                    break;
            }
        }
    }

    const std::vector<v2> excluded_cells = {
        v2(0, 0), v2(1, 0), v2(1, 1), v2(0, 1),
        v2(board_size / 2, 0),
        v2(board_size / 2 - 1, 0), v2(board_size / 2 + 1, 0),
        v2(board_size / 2 + 2, 0), v2(board_size / 2 - 2, 0),
        v2(board_size / 2, 1),
        v2(board_size / 2 - 1, 1), v2(board_size / 2 + 1, 1),
        v2(board_size / 2 + 2, 1), v2(board_size / 2 - 2, 1),
        // v2(board_size / 2 + 3, 0), v2(board_size / 2 - 3, 0)
    };

    for (auto pos: excluded_cells) {
        taken_by[pos.x][pos.y] = obstacle_type::none;
        taken_by[board_size - 2 - pos.x][board_size - 2 - pos.y] = obstacle_type::none;
    }

    matrix_ = matrix(taken_by.size() * cell_size, taken_by[0].size() * cell_size);
    for (int x = 0; x < taken_by.size(); ++x) {
        for (int y = 0; y < taken_by[0].size(); ++y) {
            matrix_.write(draw_one(v2::one() * 4, taken_by[x][y]), cell_to_pos(v2(x, y)), 0, v2::zero());
        }
    }

    is_taken = std::vector(matrix_.width(), std::vector(matrix_.height(), false));

    for (int x = 0; x < matrix_.width(); ++x)
        for (int y = 0; y < matrix_.height(); ++y) {
            if (x < border_size || x > (matrix_.width() - border_size) || y < border_size || y > (matrix_.height() - border_size))
                is_taken[x][y] = true;
            else
                is_taken[x][y] = !matrix_.pixels()[x - border_size][y - border_size].is_none();
        }
}

bool obstacle_actor::does_collide(v2 other_from, v2 other_to) {
    for (int x = other_from.x; x < other_to.x; ++x) {
        for (int y = other_from.y; y < other_to.y; ++y) {
            if (is_taken[x][y])
                return true;
        }
    }

    return false;
}

v2 obstacle_actor::cell_to_pos(v2 cell) {
    return (cell + v2::one() / 2) * cell_size;
}

void obstacle_actor::generate_obstacle(int board_size, v2 at, obstacle_type type, int min_extra_rows, int max_extra_rows, int min_continue_for, int max_continue_for, std::vector<obstacle_type> override_types, std::vector<std::vector<obstacle_type> > &taken_by) {
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

            if (taken_by[x][y] != obstacle_type::none && std::ranges::find(override_types, taken_by[x][y]) != override_types.end()) {
                taken_by[x][y] = obstacle_type::none;
                if (taken_by[board_size - 2 - x][board_size - 2 - y] != obstacle_type::none) {
                    taken_by[board_size - 2 - x][board_size - 2 - y] = obstacle_type::none;
                }
            }
            if (taken_by[x][y] == obstacle_type::none) {
                taken_by[x][y] = type;
                if (y < (taken_by[0].size() / 2) - 1) {
                    if (taken_by[board_size - 2 - x][board_size - 2 - y] != obstacle_type::none) {
                        taken_by[board_size - 2 - x][board_size - 2 - y] = obstacle_type::none;
                    }
                    taken_by[board_size - 2 - x][board_size - 2 - y] = type;
                }
            }
        }

        if (!is_additional_row)
            break;
    }
}

obstacle_type obstacle_actor::randomize_obstacle_type() {
    // std::vector<float> weights = {3, 0.07, 0.7, 0.3, 0.1};
    std::vector<float> weights = {0, 0.07, 5, 0.3, 0.1};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(weights.begin(), weights.end());

    obstacle_type chosen = static_cast<obstacle_type>(dist(gen));

    return chosen;
}


void obstacle_actor::update(float delta_time) {
}

matrix obstacle_actor::render() {
    return matrix_;
}

void obstacle_actor::fixed_update(float fixed_delta_time) {
}

obstacle_actor::~obstacle_actor() = default;
