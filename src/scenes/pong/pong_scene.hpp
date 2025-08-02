#pragma once

#include "../../scene.hpp"
#include "../../actors/text_actor.hpp"
#include "../../actors/rectangle_actor.hpp"

class pong_scene : public scene {
    std::pair<int, int> score{0, 0};
    std::shared_ptr<text_actor> p1ScoreActor, p2ScoreActor;
    std::shared_ptr<rectangle_actor> p1Paddle, p2Paddle, p1ScoreZone, p2ScoreZone, ball;
    v2 ballSpeed = v2::zero();
    float ballSpeedMultiplier = 1;
    float size_factor;
    bool canScore = true;
    bool canCollide[2] = {true, true};
    bool canBounce = true;
    color scoreColor = color::blue(0.7f);
    bool doPlay = true;

    void reset_ball(bool instant = true);
    void print_score();
    void move_paddle(std::shared_ptr<rectangle_actor> &paddle, float transform);
    void check_ball_bounce();
    void check_collisions();
    void check_scoring();
    void handle_input(float deltaTime);

public:
    pong_scene();
    void update(float delta_time) override;
    void fixed_update(float delta_time) override;
    void init() override;
};
