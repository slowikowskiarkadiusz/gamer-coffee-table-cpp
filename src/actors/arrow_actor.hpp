#pragma once

#include "../actor.hpp"
#include "../matrix.hpp"
#include "../color.hpp"
#include "../engine.hpp"
#include <memory>

struct arrow_actor_opts {
    color col = color::white();
    bool blink = false;
};

class arrow_actor : public actor {
    matrix _matrix;
    bool _is_blinking = false;
    float _blinking_offset = 0.5f; // in seconds
    arrow_actor_opts _opts;
    float _blink_timer = 0;
    bool _visible = true;

public:
    arrow_actor(const v2 &center, float size_y, const arrow_actor_opts &opts = {});

    static std::shared_ptr<arrow_actor> instantiate(const v2 &center, float size_y, const arrow_actor_opts &opts = {});

    void update(float delta_time) override;
    void fixed_update(float delta_time) override;
    matrix render() override;
    void reset_blinking();

    static v2 calc_size(float size_y);

protected:
    void redraw() override;

private:
    matrix shape(float size_y, const color &c);

public:
    ~arrow_actor() override;
};
