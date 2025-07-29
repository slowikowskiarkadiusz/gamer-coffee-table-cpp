#include "text_actor.hpp"
#include "letter.hpp" // assumes you have a letter(char, opts) -> matrix
#include <algorithm>
#include "../engine.hpp"

text_actor::text_actor(const std::string &text, const v2 &top_left, const v2 &container_size, const text_actor_opts &opts)
    : actor("text", top_left + container_size / 2.0f, container_size),
      text(text),
      _top_left(top_left),
      _bottom_right(top_left + container_size),
      _opts(opts) {
    auto [inbound, outbound] = generate_word_matrix(text, static_cast<int>(container_size.x), opts.col, opts.anchor_center);
    _inbound = std::move(inbound);
    _outbound = std::move(outbound);
}

std::shared_ptr<text_actor> text_actor::instantiate(const std::string &text, const v2 &top_left, const v2 &container_size, const text_actor_opts &opts) {
    auto rect_actor = std::make_shared<text_actor>(text, top_left, container_size, opts);
    engine::instance().register_actor(rect_actor);
    return rect_actor;
}

void text_actor::update(float delta_time) {
    if (_do_animate && _outbound) {
        _animation_runtime += delta_time * 1000.0f;

        int offset_index = 0;
        if (_animation_runtime > 500.0f) {
            float offset_duration = _animation_runtime - 500.0f;
            offset_index = std::max(static_cast<int>(offset_duration / 120.0f), 0);
        }

        int max = static_cast<int>(_bottom_right.x - _top_left.x);
        if (offset_index > static_cast<int>(_outbound->pixels().size()) - max) {
            _animation_runtime = 0;
            return;
        }

        _animation_index_offset = offset_index;
    }
}

matrix text_actor::render() {
    if (!_outbound || !_do_animate) {
        return _inbound;
    }

    return _outbound->snippet(
        v2(_animation_index_offset, 0),
        v2(_animation_index_offset + container_size().x, 5)
    );
}

void text_actor::set_animation(bool do_animate) {
    if (!do_animate) {
        _animation_index_offset = 0;
        _animation_runtime = 0;
    }
    _do_animate = do_animate;
}

std::pair<matrix, std::optional<matrix> > text_actor::generate_word_matrix(
    const std::string &word, int max, const color &col, bool anchor_center) {
    std::vector<matrix> letters;
    for (char c: word) {
        letters.push_back(letter(c, col)); // assumes letter(char, color) -> matrix
    }

    int total_width = 0;
    for (const auto &l: letters) total_width += l.width();
    int height = letters[0].height();

    matrix inbound(max, height);
    matrix outbound(total_width + max, height);

    v2 offset = v2::zero();
    bool exceeds = false;
    int last_index = 0;

    for (const auto &l: letters) {
        if (!exceeds && max > -1 && (offset.x + l.width()) > max) {
            inbound.write_at_origin(letter('-', col), offset);
            exceeds = true;
        }

        if (!exceeds) {
            inbound.write_at_origin(l, offset);
            last_index = offset.x + l.width();
        }

        outbound.write_at_origin(l, offset);
        offset = offset + v2(l.width(), 0);
    }

    if (anchor_center && last_index < max) {
        matrix new_inbound(last_index, height);
        new_inbound.write_at_origin(inbound, v2::zero());
        inbound = new_inbound;
    }

    if (exceeds) return {inbound, outbound};
    return {inbound, std::nullopt};
}

text_actor::~text_actor() {
}
