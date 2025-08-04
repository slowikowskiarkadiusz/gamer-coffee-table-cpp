#include "border_actor.hpp"

void border_actor::write_border(v2 from, v2 to, matrix &matrix_) {
    v2 start(std::min(from.x, to.x), std::min(from.y, to.y));
    v2 end(std::max(from.x, to.x), std::max(from.y, to.y));
    for (int x = start.x; x <= end.x; ++x) {
        for (int y = start.y; y <= end.y; ++y) {
            if (x == start.x || x == end.x || y == start.y || y == end.y)
                for (int i = 0; i < depth_; ++i) {
                    if (x == start.x)
                        matrix_.set_pixel(x + i, y, color::white(0.2f));
                    if (x == end.x)
                        matrix_.set_pixel(x - i, y, color::white(0.2f));
                    if (y == start.y)
                        matrix_.set_pixel(x, y + i, color::white(0.2f));
                    if (y == end.y)
                        matrix_.set_pixel(x, y - i, color::white(0.2f));
                }
        }
    }
}

void border_actor::redraw() {
    write_border(from_, to_, matrix_);
}

void border_actor::update(float delta_time) {
}

matrix border_actor::render() {
    return matrix_;
}

void border_actor::fixed_update(float fixed_delta_time) {
}

border_actor::~border_actor() = default;
