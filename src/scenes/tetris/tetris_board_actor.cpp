#include "tetris_board_actor.h"

#include <algorithm>
#include <random>
#include <chrono>

tetris_board_actor::tetris_board_actor(v2 center, int seed, bool is_p1,
                                       std::function<void(int, bool)> on_deal_damage,
                                       std::function<void(bool)> on_death)
    : actor("tetris-board", center, v2(engine::instance().screen_size.x / 2, engine::instance().screen_size.y)),
      center(center), seed(seed), is_p1(is_p1),
      on_deal_damage(on_deal_damage), on_death(on_death),
      garbage_bar_logic(v2(board_width + 2, size().y - 1 - board_height / 2), v2(1, board_height), {color::white(0.1f)}),
      hold_logic(v2::zero()),
      static_board_matrix(board_width, board_height),
      borders_matrix(size().x, size().y) {
    is_taken.resize(board_width, std::vector<bool>(board_height, false));
    write_border(v2(0, size().y - 1), v2(board_width + 1, size().y - 1 - board_height - 1));
    write_border(v2(board_width + 1, size().y - 1), v2(board_width + 1 + garbage_bar_logic.size.x + 1, size().y - 1 - board_height - 1));

    hold_logic.center = v2(hold_logic.get_size().x / 2 + 1, size().y - 1 - board_height - 1 - hold_logic.get_size().y / 2);
    write_border(v2(0, size().y - 1 - board_height - 1 - hold_logic.get_size().y - 1),
                 v2(1 + hold_logic.get_size().x, size().y - 1 - board_height - 1));

    spawn();
}

void tetris_board_actor::update(float deltaTime) {
    if (!do_play) return;
    garbage_bar_logic.update(deltaTime);
    fall(deltaTime);
}

matrix tetris_board_actor::render() {
    matrix _matrix(size().x, size().y);
    v2 boardOffset(1 + board_width / 2, size().y - 1 - board_height + board_height / 2);

    _matrix.write_at_origin(borders_matrix, v2::zero());
    _matrix.write(garbage_bar_logic.render(), garbage_bar_logic.center, 0);
    _matrix.write(hold_logic.render(), hold_logic.center, 0);

    matrix boardMatrix(board_width, board_height);
    if (current_agent_shadow)
        boardMatrix.write(current_agent_shadow->render(), current_agent_shadow->center, 0);
    if (current_agent)
        boardMatrix.write(current_agent->render(), current_agent->center, 0);

    _matrix.write(boardMatrix, boardOffset);
    _matrix.write(static_board_matrix, boardOffset, 0);
    return _matrix;
}

void tetris_board_actor::stop() {
    do_play = false;
}

void tetris_board_actor::spawn(std::optional<v2> center, std::optional<tetris_shape> shape) {
    switched_pieces = false;
    tetris_shape block = shape.value_or(generate_block(seed, block_index++));
    v2 agentCenter = center.value_or(v2(board_width / 2, 0));

    current_agent = std::make_unique<tetris_block>(agentCenter, block, false);
    v2 drop_pos = v2(current_agent->center.x, current_agent->center.y + calc_drop());
    current_agent_shadow = std::make_unique<tetris_block>(drop_pos, block, true);
}

void tetris_board_actor::move_block_by(const v2 &by) {
    if (!current_agent) return;

    auto spots = current_agent->get_taken_spots();

    if (!std::ranges::any_of(spots, [&](const v2 &spot) {
        return is_position_taken(spot.x + by.x, spot.y + by.y);
    })) {
        current_agent->center = current_agent->center.add(by);
        current_agent_shadow->center = v2(current_agent->center.x, current_agent->center.y + calc_drop());
    }
}

void tetris_board_actor::rotate_block() {
    if (!current_agent || !current_agent_shadow) return;
    auto kicks = current_agent->get_kicks(current_agent->get_block_rotation() + 90);
    v2 pre = current_agent->center;
    current_agent->rotate_block(-90);
    v2 post = current_agent->center;

    bool didKick = false;
    for (const auto &kick: kicks) {
        current_agent->center = post.add(kick);
        auto spots = current_agent->get_taken_spots();
        if (!std::ranges::any_of(spots, [&](const v2 &spot) {
            return is_position_taken(spot.x, spot.y);
        })) {
            didKick = true;
            break;
        }
    }

    if (!didKick) {
        current_agent->rotate_block(90);
        current_agent->center = pre;
    } else {
        current_agent_shadow->rotate_block(-90);
        current_agent_shadow->center = v2(current_agent->center.x, current_agent->center.y + calc_drop());
    }
}

void tetris_board_actor::deal_damage(int count) {
    int left = garbage_bar_logic.decrease_and_get_left(count);
    if (left > 0) on_deal_damage(left, is_p1);
}

void tetris_board_actor::take_damage(int count) {
    garbage_bar_logic.add_lines(count);
}

void tetris_board_actor::write_border(v2 from, v2 to) {
    v2 start(std::min(from.x, to.x), std::min(from.y, to.y));
    v2 end(std::max(from.x, to.x), std::max(from.y, to.y));
    for (int x = start.x; x <= end.x; ++x) {
        for (int y = start.y; y <= end.y; ++y) {
            if (x == start.x || x == end.x || y == start.y || y == end.y)
                borders_matrix.set_pixel(x, y, color::white(0.2f));
        }
    }
}

int tetris_board_actor::calc_drop(int i) {
    auto spots = current_agent->get_taken_spots();
    if (!std::ranges::any_of(spots, [&](const v2 &spot) {
        return is_position_taken(spot.x, spot.y + i);
    })) {
        int next = calc_drop(i + 1);
        return next ? next : i;
    } else {
        return i - 1;
    }
}

bool tetris_board_actor::is_position_taken(int x, int y) {
    auto a = x < 0;
    auto b = x >= board_width;
    auto c = y >= board_height;
    auto d = a || b || c || is_taken[x][y];

    auto result = a || b || c || d;
    if (result)
        auto e = "";

    return result;
}

tetris_shape tetris_board_actor::generate_block(int seed, int index) {
    if (spawn_bag.empty()) {
        for (int i = 0; i < 7; ++i) spawn_bag.push_back(static_cast<tetris_shape>(i));
        std::shuffle(spawn_bag.begin(), spawn_bag.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    }
    tetris_shape next = spawn_bag.back();
    spawn_bag.pop_back();
    return next;
}

void tetris_board_actor::fall(float deltaTime) {
    if ((continue_dropping && current_agent) || lock_delay_timer > 0) {
        bool dropped = false;

        auto spots = current_agent->get_taken_spots();

        bool is_any_taken = std::ranges::any_of(spots, [&](const v2 &spot) {
            return is_position_taken(spot.x, spot.y + 1);
        });

        if (is_any_taken) {
            lock_delay_timer += deltaTime;
            if (lock_delay_timer > lock_delay) {
                drop();
                lock_delay_timer = 0;
                dropped = true;
            }
        } else {
            current_agent->center.y++;
        }

        if (!dropped)
            continue_dropping = false;
    } else {
        drop_timer += deltaTime;
        if (drop_timer > dropping_delay_value) {
            drop_timer = 0;
            continue_dropping = true;
        }
    }
}

void tetris_board_actor::drop() {
    if (!can_drop_again) return;

    current_agent->center = current_agent_shadow->center;
    for (const auto &spot: current_agent->get_taken_spots()) {
        is_taken[spot.x][spot.y] = true;
        static_board_matrix.set_pixel(spot.x, spot.y, tetris_block_colors[static_cast<int>(current_agent->get_shape())]);

        if (spot.y == 0)
            on_death(is_p1);
    }

    current_agent.reset();
    current_agent_shadow.reset();
    can_drop_again = false;

    clear_lines();
    pop_garbage_lines();

    engine::instance().set_timeout([this]() {
        spawn();
        can_drop_again = true;
    }, after_drop_delay);
}

void tetris_board_actor::clear_lines() {
    std::vector<int> lines;
    for (int y = 0; y < board_height; ++y) {
        if (std::all_of(is_taken.begin(), is_taken.end(), [y](const std::vector<bool> &col) { return col[y]; }))
            lines.push_back(y);
    }
    if (lines.empty()) return;
    deal_damage(static_cast<int>(lines.size()));

    for (int line: lines) {
        for (int x = 0; x < board_width; ++x)
            static_board_matrix.set_pixel(x, line, color::none());
    }

    for (int line: lines) {
        for (int y = line; y > 0; --y) {
            for (int x = 0; x < board_width; ++x) {
                is_taken[x][y] = is_taken[x][y - 1];
                static_board_matrix.set_pixel(x, y, static_board_matrix.pixels()[x][y - 1]);
            }
        }
    }
}

void tetris_board_actor::pop_garbage_lines() {
    int hole = std::rand() % board_width;
    while (garbage_bar_logic.pop()) {
        for (int y = 0; y < board_height - 1; ++y) {
            for (int x = 0; x < board_width; ++x) {
                is_taken[x][y] = is_taken[x][y + 1];
                static_board_matrix.set_pixel(x, y, static_board_matrix.pixels()[x][y + 1]);
            }
        }

        for (int x = 0; x < board_width; ++x) {
            is_taken[x][board_height - 1] = (x != hole);
            static_board_matrix.set_pixel(x, board_height - 1, x != hole ? color::white(0.5f) : color::none());
        }
    }
}
