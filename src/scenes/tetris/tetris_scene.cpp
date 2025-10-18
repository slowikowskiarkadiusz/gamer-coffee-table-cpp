#include "tetris_scene.hpp"
#include <functional>

#include "../../actors/rectangle_actor.hpp"
#include "../../actors/text_actor.hpp"

tetris_scene::tetris_scene() = default;

void tetris_scene::init()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int seed = std::rand() % 10000;
    v2 center = engine::screen_size.div(2);

    float size_factor = engine::screen_size.x / 32;

    p1Board = engine::instantiate<tetris_board_actor>(
        center,
        seed,
        true,
        [this](auto &&PH1, auto &&PH2)
        { on_lines_cleared(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); },
        [this](auto &&PH1)
        { on_players_death(std::forward<decltype(PH1)>(PH1)); });

    p1Board->set_center(v2(p1Board->size().x, engine::screen_size.y - p1Board->size().y));

    p2Board = engine::instantiate<tetris_board_actor>(
        center.add(v2(engine::screen_size.x / 4, 0)),
        seed,
        false,
        [this](auto &&PH1, auto &&PH2)
        { on_lines_cleared(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); },
        [this](auto &&PH1)
        { on_players_death(std::forward<decltype(PH1)>(PH1)); });

    p2Board->set_center(v2(engine::screen_size.x - p2Board->size().x, p2Board->size().y));

    p2Board->rotate(180);
    p2Board->move_by(v2::one().mul(-1));
}

void tetris_scene::on_lines_cleared(int count, bool isP1)
{
    auto *target = isP1 ? p2Board.get() : p1Board.get();
    if (target)
        target->take_damage(count);
}

void tetris_scene::on_players_death(bool isP1)
{
    if (p1Board)
        p1Board->stop();
    if (p2Board)
        p2Board->stop();

    auto background = engine::instantiate<rectangle_actor>(engine::screen_size / 2, engine::screen_size, color::black(0.5));

    auto printText = [](bool isP1)
    {
        std::string text = "P" + std::to_string(isP1 ? 1 : 2) + " WON";
        auto actor = engine::instantiate<text_actor>(text, v2::zero(), v2(engine::screen_size.x, 5), text_actor_opts{true});
        v2 newCenter = engine::screen_size.div(2);
        newCenter.y += (isP1 ? 1 : -1) * engine::screen_size.y / 4;
        actor->move_to(newCenter);
        return actor;
    };

    v2 rotateAround = engine::screen_size.div(2).sub(v2::one());
    rotateAround.y -= engine::screen_size.y / 4;

    printText(true);
    printText(false)->rotate_around(rotateAround, 180)->move_by(v2(0, 0.5));
}

void tetris_scene::update(float delta_time)
{
}

void tetris_scene::fixed_update(float fixed_delta_time)
{
}
