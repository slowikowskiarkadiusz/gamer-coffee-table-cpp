#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "garbage_bar_logic.hpp"
#include "tetris_shape.hpp"
#include "tetris_block.hpp"
#include "hold_logic.hpp"

class tetris_board_actor : public actor {
public:
    tetris_board_actor(v2 center, int seed, bool isP1,
                       std::function<void(int, bool)> onDealDamage,
                       std::function<void(bool)> onDeath);

    void update(float deltaTime) override;
    matrix render() override;
    void stop();

private:
    static constexpr int board_width = 10;
    static constexpr int board_height = 20;
    static constexpr int after_drop_delay = 250;
    static constexpr int line_clearing_animation_factor = 75;
    static constexpr int dropping_delay = 1000;
    static constexpr int faster_dropping_delay = 300;
    static constexpr int lock_delay = 1000;

    std::vector<std::vector<bool> > is_taken;
    int block_index = 0;
    std::unique_ptr<tetris_block> current_agent = nullptr;
    std::unique_ptr<tetris_block> current_agent_shadow = nullptr;
    garbage_bar_logic garbage_bar_logic;
    hold_logic hold_logic;
    bool can_drop_again = true;
    matrix static_board_matrix;
    matrix borders_matrix;
    bool continue_dropping = true;
    float drop_timer = 0;
    float lock_delay_timer = 0;
    float dropping_delay_value = dropping_delay;
    bool switched_pieces = false;
    std::vector<tetris_shape> spawn_bag;
    bool do_play = true;

    v2 center;
    int seed;
    bool is_p1;
    std::function<void(int, bool)> on_deal_damage;
    std::function<void(bool)> on_death;

    void spawn(std::optional<v2> center = std::nullopt, std::optional<tetris_shape> shape = std::nullopt);
    void move_block_by(const v2 &by);
    void rotate_block();
    void deal_damage(int count);
    void write_border(v2 from, v2 to);
    int calc_drop(int i = 0);
    bool is_position_taken(int x, int y);
    void clear_lines();
    void pop_garbage_lines();
    void drop();
    void fall(float deltaTime);
    tetris_shape generate_block(int seed, int index);
};
