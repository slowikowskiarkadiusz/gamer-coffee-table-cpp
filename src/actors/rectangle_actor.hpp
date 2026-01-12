#pragma once

#include <vector>
#include <algorithm>
#include "../actor.hpp"

class rectangle_actor : public actor {
    matrix _matrix;
    color _color;
    bool is_initialized = false;

public:
    rectangle_actor(const v2 &center, const v2 &size, const color &col = color::white())
        : actor("rectangle", center, size), _matrix(std::abs(size.x), std::abs(size.y), col, "make_rectangle"), _color(col) {
        // is_initialized = true;
    }

    void update(float a) override {
    }

    void fixed_update(float a) override {
    }

    matrix *render() override {
        return &_matrix;
    }

    bool does_rectangle_overlap(const rectangle_actor &other) {
        return does_overlap(other.get_center(), other.size(), other.get_rotation());
    }

    bool does_overlap(const v2 &other_center, const v2 &other_size, float other_rotation) {
        std::vector<v2> vertices = {
            get_center() + v2(size().x / 2, size().y / 2),
            get_center() + v2(size().x / 2, -size().y / 2),
            get_center() + v2(-size().x / 2, -size().y / 2),
            get_center() + v2(-size().x / 2, size().y / 2)
        };

        std::vector<v2> other_vertices = {
            other_center + v2(other_size.x / 2, other_size.y / 2),
            other_center + v2(other_size.x / 2, -other_size.y / 2),
            other_center + v2(-other_size.x / 2, -other_size.y / 2),
            other_center + v2(-other_size.x / 2, other_size.y / 2)
        };

        for (const auto &ov: other_vertices) {
            float min_x = std::min_element(vertices.begin(), vertices.end(), [](const v2 &a, const v2 &b) {
                return a.x < b.x;
            })->x - 1;

            int intersects = 0;
            for (int i = 0; i < vertices.size(); ++i) {
                int j = (i + 1) % vertices.size();
                if (do_intersect(v2(min_x, ov.y), ov, vertices[i], vertices[j]))
                    intersects++;
            }

            if (intersects > 0 && intersects % 2 == 1)
                return true;
        }

        return false;
    }

    void change_color_to(const color &new_color) {
        _color = new_color;
        redraw();
    }

protected:
    void redraw() override {
        _matrix = matrix(std::abs(size().x), std::abs(size().y), _color, "make_rectangle");
    }

private:
    // matrix make_rectangle(const v2 &size, const color &c) {
    //     if (!is_initialized || (size.x != this->size().x || size.y != this->size().y))
    //         return matrix(std::abs(size.x), std::abs(size.y), c, "make_rectangle");

    //     if (!is_initialized || !c.equals(_color))
    //         return _matrix.fill(this->_color);

    //     return _matrix;
    // }

    static bool ccw(const v2 &A, const v2 &B, const v2 &C) {
        return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);
    }

    static bool do_intersect(const v2 &A, const v2 &B, const v2 &C, const v2 &D) {
        return ccw(A, C, D) != ccw(B, C, D) && ccw(A, B, C) != ccw(A, B, D);
    }

public:
    ~rectangle_actor() override;
};
