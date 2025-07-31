#include "tetris_scene.hpp"
#include <functional>

#include "../../actors/text_actor.hpp"

tetris_scene::tetris_scene() = default;

void tetris_scene::init() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int seed = std::rand() % 10000;
    v2 center = engine::instance().screen_size.div(2);

    p1Board = tetris_board_actor::instantiate(
        center.sub(v2(engine::instance().screen_size.x / 4, 0)),
        seed,
        true,
        std::bind(&tetris_scene::on_lines_cleared, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&tetris_scene::on_players_death, this, std::placeholders::_1)
    );

    // p2Board = tetris_board_actor::instantiate(
    //     center.add(v2(engine::instance().screen_size.x / 4, 0)),
    //     seed,
    //     false,
    //     std::bind(&tetris_scene::on_lines_cleared, this, std::placeholders::_1, std::placeholders::_2),
    //     std::bind(&tetris_scene::on_players_death, this, std::placeholders::_1)
    // );
    //
    // p2Board->rotate(180);
    // p2Board->move_by(v2::one().mul(-1));
}

void tetris_scene::on_lines_cleared(int count, bool isP1) {
    auto *target =
        // isP1
    // ? p2Board.get() :
    p1Board.get();
    if (target)
        target->take_damage(count);
}

void tetris_scene::on_players_death(bool isP1) {
    if (p1Board) p1Board->stop();
    // if (p2Board) p2Board->stop();

    auto printText = [](bool isP1) {
        std::string text = "P" + std::to_string(isP1 ? 1 : 2) + " WON";
        auto actor = std::make_unique<text_actor>(text, v2::zero(), v2(engine::instance().screen_size.x, 5), text_actor_opts{true});
        v2 newCenter = engine::instance().screen_size.div(2);
        newCenter.y += (isP1 ? 1 : -1) * engine::instance().screen_size.y / 4;
        actor->move_to(newCenter);
        return actor;
    };

    v2 rotateAround = engine::instance().screen_size.div(2).sub(v2::one());
    rotateAround.y -= engine::instance().screen_size.y / 4;

    printText(true);
    printText(false)->rotate_around(rotateAround, 180);
}

void tetris_scene::update(float delta_time) {
    if (p1Board) p1Board->update(delta_time);
    // if (p2Board) p2Board->update(delta_time);
}

void tetris_scene::fixed_update(float fixed_delta_time) {
    if (p1Board) p1Board->fixed_update(fixed_delta_time);
    // if (p2Board) p2Board->fixed_update(fixed_delta_time);
}
