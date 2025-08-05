#include "pong_scene.hpp"
#include "../menu/menu_scene.hpp"

constexpr float paddle_speed = 0.03;
constexpr float max_bounce_speed = 0.03;
constexpr float original_ball_speed = 0.007;

pong_scene::pong_scene() = default;

void pong_scene::init() {
    auto screen = engine::screen_size;
    size_factor = screen.x / 32;
    p1Paddle = engine::instantiate<rectangle_actor>(v2(screen.x / 2, 3 * size_factor), v2(7, 1) * size_factor);
    p2Paddle = engine::instantiate<rectangle_actor>(v2(screen.x / 2, screen.y - 4 * size_factor), v2(7, 1) * size_factor);
    p1ScoreZone = engine::instantiate<rectangle_actor>(v2(screen.x / 2, -4 * size_factor), v2(screen.x, 10), color::none());
    p2ScoreZone = engine::instantiate<rectangle_actor>(v2(screen.x / 2, screen.y + 4 * size_factor), v2(screen.x, 10), color::none());
    ball = engine::instantiate<rectangle_actor>(screen / 2, v2(2, 2) * size_factor, color::white());
    reset_ball();
    print_score();
}

void pong_scene::update(float delta_time) {
    handle_input(delta_time);
}

void pong_scene::fixed_update(float delta_time) {
    ball->move_by(ballSpeed * delta_time);
    check_ball_bounce();
    check_collisions();
    check_scoring();
}

void pong_scene::handle_input(float deltaTime) {
    if (input::is_key_press(key::P1_LEFT) ^ input::is_key_press(key::P1_RIGHT))
        move_paddle(p1Paddle, (input::is_key_press(key::P1_RIGHT) ? 1 : -1) * paddle_speed * deltaTime * size_factor);

    if (input::is_key_press(key::P2_LEFT) ^ input::is_key_press(key::P2_RIGHT))
        move_paddle(p2Paddle, (input::is_key_press(key::P2_RIGHT) ? 1 : -1) * paddle_speed * deltaTime * size_factor);
}

void pong_scene::move_paddle(std::shared_ptr<rectangle_actor> &paddle, float dx) {
    paddle->move_by(v2::right() * dx);
    float left = paddle->get_center().x - paddle->size().x / 2;
    float right = paddle->get_center().x + paddle->size().x / 2;
    float screenWidth = engine::screen_size.x;
    if (left < 0) paddle->move_to(v2(paddle->size().x / 2, paddle->get_center().y));
    if (right > screenWidth) paddle->move_to(v2(screenWidth - paddle->size().x / 2, paddle->get_center().y));
}

// void pong_scene::move_paddle(std::shared_ptr<rectangle_actor> &paddle, float transform) {
//     paddle->move_by(v2::right() * transform);
//
//     if (paddle->center().x - paddle->size().x / 2 < 0)
//         paddle->move_to({paddle->size().x / 2, paddle->center().y});
//
//     if (paddle->center().x + paddle->size().x / 2 > engine::screen_size.x)
//         paddle->move_to({engine::screen_size.x - paddle->size().x / 2, paddle->center().y});
// }

void pong_scene::reset_ball(bool instant) {
    instant = true;
    print_score();
    canScore = true;
    canBounce = true;
    canCollide[0] = canCollide[1] = true;
    ball->move_to(engine::screen_size / 2);
    ballSpeed = v2::zero();
    ball->rename("ball");

    auto setBall = [&]() {
        float x = ((float) rand() / RAND_MAX) * 2 * original_ball_speed - original_ball_speed;
        float y = ((float) rand() / RAND_MAX) >= 0.5f ? original_ball_speed : -original_ball_speed;
        ballSpeed = v2(x, y) * size_factor;
    };

    if (instant) setBall();
    else engine::set_timeout([=]() { setBall(); }, 1000);
}

void pong_scene::check_ball_bounce() {
    auto screen = engine::screen_size;
    if (canBounce) {
        if (ball->get_center().x + ball->size().x / 2 >= screen.x ||
            ball->get_center().x - ball->size().x / 2 <= 0) {
            ballSpeed.x *= -1;
            canBounce = false;
            engine::set_timeout([this]() { canBounce = true; }, 1000);
        }
    }
}

void pong_scene::check_collisions() {
    std::vector<std::shared_ptr<rectangle_actor> > paddles = {p1Paddle, p2Paddle};
    for (int i = 0; i < 2; ++i) {
        if (!canCollide[i]) continue;
        if (paddles[i]->does_overlap(ball->get_center(), ball->size(), ball->rotation())) {
            canCollide[i] = false;
            canBounce = true;
            float offset = (ball->get_center().x - paddles[i]->get_center().x) / paddles[i]->size().x;
            ballSpeed.x = offset * max_bounce_speed * size_factor;
            ballSpeed.y = original_ball_speed * size_factor * ballSpeedMultiplier * (i == 0 ? 1 : -1);
            ballSpeedMultiplier = std::min(ballSpeedMultiplier + 0.1f, 5.0f);
            return;
        }
    }
}

void pong_scene::check_scoring() {
    if (!canScore) return;
    bool scored = false;
    if (ball->get_center().y < p1ScoreZone->get_center().y && p1ScoreZone->does_overlap(ball->get_center(), ball->size(), ball->rotation())) {
        score.second++;
        scored = true;
    } else if (ball->get_center().y > p2ScoreZone->get_center().y && p2ScoreZone->does_overlap(ball->get_center(), ball->size(), ball->rotation())) {
        score.first++;
        scored = true;
    }

    if (scored) {
        canScore = false;
        canBounce = false;
        canCollide[0] = canCollide[1] = false;
        print_score();

        if (score.first == 9 || score.second == 9) {
            doPlay = false;
            std::string winner = score.first > score.second ? "P1 WON" : "P2 WON";
            auto center = engine::screen_size / 2;
            auto winText = engine::instantiate<text_actor>(winner, v2::zero(), v2(engine::screen_size.x, 5));
            winText->move_to(center);
            p1Paddle->kill();
            p2Paddle->kill();
            engine::set_timeout([]() {
                engine::open_scene(std::make_shared<menu_scene>());
            }, 10000);
        } else {
            engine::set_timeout([this]() { reset_ball(); }, 1000);
        }
    }
}

void pong_scene::print_score() {
    if (p1ScoreActor) engine::unregister_actor(p1ScoreActor);
    if (p2ScoreActor) engine::unregister_actor(p2ScoreActor);

    p1ScoreActor = engine::instantiate<text_actor>(std::to_string(score.first), v2::zero(), v2(4 * std::to_string(score.first).length(), 5), text_actor_opts{.col = color::blue(0.5)});
    p1ScoreActor->rotate(-90);
    p1ScoreActor->move_to(v2(5, engine::screen_size.y / 2 - 5));

    p2ScoreActor = engine::instantiate<text_actor>(std::to_string(score.second), v2::zero(), v2(4 * std::to_string(score.second).length(), 5), text_actor_opts{.col = color::blue(0.5)});
    p2ScoreActor->rotate(-90);
    p2ScoreActor->move_to(v2(5, engine::screen_size.y / 2 + 3));
}
