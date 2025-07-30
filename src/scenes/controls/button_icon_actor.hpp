#pragma once

#include "../../actor.hpp"
#include "../../matrix.hpp"
#include "../../v2.hpp"
#include "../../color.hpp"
#include "../../input/key.hpp"

struct button_icon_actor_opts {
    key key;
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

        std::vector<key> l = {key::P1_L_L, key::P1_L, key::P2_L_L, key::P2_L};
        std::vector<key> r = {key::P1_R_R, key::P1_R, key::P2_R_R, key::P2_R};

        if (std::find(l.begin(), l.end(), k) != l.end() || std::find(r.begin(), r.end(), k) != r.end()) {
            m.set_pixel(2, 1, color::black());
            m.set_pixel(std::find(l.begin(), l.end(), k) != l.end() ? 1 : 3, 2, color::black());
            m.set_pixel(2, 3, color::black());
        }

        return m;
    }

    static color map_key_to_color(key k) {
        switch (k) {
            case key::P1_L_L:
            case key::P1_L:
            case key::P2_L_L:
            case key::P2_L:
            case key::P1_R_R:
            case key::P1_R:
            case key::P2_R_R:
            case key::P2_R:
                return color::red();
            case key::P1_L_BLUE:
            case key::P2_L_BLUE:
                return color::blue();
            case key::P1_R_GREEN:
            case key::P2_R_GREEN:
                return color::green();
            case key::START:
                return color::white();
            default:
                return color::none();
        }
    }

public:
    explicit button_icon_actor(const v2 &center, int size = 5, button_icon_actor_opts opts = {key::P1_L})
        : actor("button-icon", center, v2::one() * size), mat(make_matrix(size, opts.key)) {
    }

    matrix render() override { return mat; }

    void update(float) override {
    }

    void fixed_update(float) override {
    }

    void redraw() override {
    }
};
