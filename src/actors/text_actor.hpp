#pragma once

#include <string>
#include "../actor.hpp"

struct text_actor_opts {
    bool anchor_center = false;
    bool animate_if_outbound = true;
    color col = color::white();
    bool debug_highlight = false;
};

class text_actor : public actor {
    matrix _inbound = matrix(0, 0);
    std::optional<matrix> _outbound;
    int _animation_index_offset = 0;
    bool _do_animate = false;
    float _animation_runtime = 0;
    v2 _top_left;
    v2 _bottom_right;
    text_actor_opts _opts;

public:
    std::string text;

    text_actor(const std::string &text, const v2 &top_left, const v2 &container_size, const text_actor_opts &opts = {});

    static std::shared_ptr<text_actor> instantiate(const std::string &text, const v2 &top_left, const v2 &container_size, const text_actor_opts &opts = {});

    void update(float delta_time) override;

    void fixed_update(float) override {
    }

    matrix render() override;

    void set_animation(bool do_animate);

    void redraw() override {
    }

    ~text_actor() override;

private:
    static std::pair<matrix, std::optional<matrix> > generate_word_matrix(
        const std::string &word_to_write, int max, const color &col, bool anchor_center);

    v2 container_size() const { return _bottom_right - _top_left; }
};
