#pragma once
#include "../actor.hpp"

class border_actor : public actor {
private:
    matrix matrix_;
    color color_;
    v2 from_;
    v2 to_;
    int depth_;

    void write_border(v2 from, v2 to, matrix &matrix_);

public:
    border_actor(v2 from, v2 to, v2 matrix_size, uint depth = 1, ::color color = color::white(0.5))
        : actor("border", matrix_size / 2, matrix_size), from_(from), to_(to), matrix_(matrix(matrix_size.x, matrix_size.y)), color_(color), depth_(depth) {
        redraw();
    }

    void redraw() override;
    void update(float delta_time) override;
    matrix render() override;
    void fixed_update(float fixed_delta_time) override;
    ~border_actor() override;
};
