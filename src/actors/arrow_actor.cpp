#include "arrow_actor.hpp"
#include <cmath>

arrow_actor::arrow_actor(const v2 &center, float size_y, const arrow_actor_opts &opts)
    : actor("arrow", center, calc_size(size_y)), _opts(opts), _is_blinking(opts.blink), _matrix(shape(size().y, _opts.col)) {
    if (_is_blinking) _blink_timer = _blinking_offset;
}

std::shared_ptr<arrow_actor> arrow_actor::instantiate(const v2 &center, float size_y, const arrow_actor_opts &opts) {
    auto arrow_act = std::make_shared<arrow_actor>(center, size_y, opts);
    engine::instance().register_actor(arrow_act);
    return arrow_act;
}

v2 arrow_actor::calc_size(float size_y) {
    return v2(std::ceil(size_y / 2.0f), size_y);
}

void arrow_actor::update(float delta_time) {
    if (_is_blinking) {
        _blink_timer -= delta_time;
        if (_blink_timer <= 0.f) {
            _visible = !_visible;
            _matrix = _visible ? shape(size().y, _opts.col) : shape(size().y, color::none());
            _blink_timer = _blinking_offset;
        }
    }
}

void arrow_actor::fixed_update(float) {
}

matrix arrow_actor::render() {
    return _matrix;
}

void arrow_actor::reset_blinking() {
    _matrix = shape(size().y, _opts.col);
    _visible = true;
    _blink_timer = _blinking_offset;
}

void arrow_actor::redraw() {
    _matrix = shape(size().y, _opts.col);
}

matrix arrow_actor::shape(float size_y, const color &c) {
    int width = static_cast<int>(std::ceil(size_y / 2.0f));
    int height = static_cast<int>(size_y);
    matrix m(width, height);

    for (int y = 0; y < height / 2 + 1; ++y) {
        m.set_pixel(0, y, c);

        for (int x = 1; x <= y; ++x) {
            m.set_pixel(x, y, c);
            m.set_pixel(x, height - y - 1, c);
        }

        m.set_pixel(0, height - y - 1, c);
    }

    return m;
}

arrow_actor::~arrow_actor() {
}
