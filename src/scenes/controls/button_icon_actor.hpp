#pragma once

#include "../../actor.hpp"
#include "../../matrix.hpp"
#include "../../v2.hpp"
#include "../../color.hpp"
#include "../../input/key.hpp"

struct button_icon_actor_opts {
    key key_;
};

class button_icon_actor : public actor {
    matrix mat;

    static matrix make_matrix(int size, key k) {
        color col = map_key_to_color(k);
        matrix m(size, size, color::none());

        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                if ((x == 0 || x == size - 1) && (y != 0 && y != size - 1)) {
                    m.set_pixel(x, y, col);
                } else if ((y == 0 || y == size - 1) && (x != 0 && x != size - 1)) {
                    m.set_pixel(x, y, col);
                } else if (x > 0 && x < size - 1 && y > 0 && y < size - 1) {
                    m.set_pixel(x, y, col);
                }
            }
        }

        std::vector<key> l = {key::P1_LEFT, key::P2_LEFT};
        std::vector<key> r = {key::P1_RIGHT, key::P2_RIGHT};
        std::vector<key> u = {key::P1_UP, key::P2_UP};
        std::vector<key> d = {key::P1_DOWN, key::P2_DOWN};

        if (std::find(l.begin(), l.end(), k) != l.end() || std::find(r.begin(), r.end(), k) != r.end()) {
            m.set_pixel(2, 1, color::black());
            m.set_pixel(std::find(l.begin(), l.end(), k) != l.end() ? 1 : 3, 2, color::black());
            m.set_pixel(2, 3, color::black());
        }

        if (std::find(u.begin(), u.end(), k) != u.end() || std::find(d.begin(), d.end(), k) != d.end()) {
            m.set_pixel(1, 2, color::black());
            m.set_pixel(std::find(d.begin(), d.end(), k) != d.end() ? 1 : 3, 2, color::black());
            m.set_pixel(3, 2, color::black());
        }

        return m;
    }

    static color map_key_to_color(key k) {
        switch (k) {
            case key::P1_LEFT:
            case key::P1_RIGHT:
            case key::P1_UP:
            case key::P1_DOWN:
            case key::P1_ANY_DIRECTION:
            case key::P2_LEFT:
            case key::P2_RIGHT:
            case key::P2_UP:
            case key::P2_DOWN:
            case key::P2_ANY_DIRECTION:
                return color::red();
            case key::P1_BLUE:
            case key::P2_BLUE:
                return color::blue();
            case key::P1_GREEN:
            case key::P2_GREEN:
                return color::green();
            case key::START:
                return color::white();
            default:
                return color::none();
        }
    }

public:
    explicit button_icon_actor(const v2 &center, int size = 5, button_icon_actor_opts opts = {key::P1_ANY_DIRECTION})
        : actor("button-icon", center, v2::one() * size), mat(make_matrix(size, opts.key_)) {
    }

    matrix *render() override { return &mat; }

    void update(float) override {
    }

    void fixed_update(float) override {
    }

    void redraw() override {
    }
};
