#pragma once

#include <memory>

#include "tetris_board_actor.h"

class tetris_scene : public scene {
public:
    tetris_scene();
    void init() override;
    void update(float delta_time) override;
    void fixed_update(float fixed_delta_time) override;

private:
    std::shared_ptr<tetris_board_actor> p1Board;
    // std::shared_ptr<tetris_board_actor> p2Board;

    void on_lines_cleared(int count, bool isP1);
    void on_players_death(bool isP1);
};
