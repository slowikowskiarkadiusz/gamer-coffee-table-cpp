#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include "v2.hpp"
#include "color.hpp"
#include "grid2d.hpp"

enum class color_blending {
    no_blending,
    light_alike,
    weighted_by_opacity,
};

class matrix {
protected:
    grid2d<color> _matrix;

public:
    matrix(v2 size, const color &default_color = color::none()): matrix(size.x, size.y, default_color) {
    }

    matrix(int x, int y, const color &default_color = color::none()) {
        x = std::round(x);
        y = std::round(y);
        _matrix = grid2d<color>(x, y, default_color);
    }

    const grid2d<color> &pixels() const {
        return _matrix;
    }

    const uint width() const {
        return _matrix.width();
    }

    const uint height() const {
        return _matrix.height();
    }

    void set_pixel_v2(const v2 &point, const color &color) {
        _matrix.at(point.x, point.y) = color.copy();
    }

    void set_pixel(int x, int y, const color &color) {
        _matrix.at(x, y) = color.copy();
    }

    matrix &write_at_origin(const matrix &other, const v2 &origin) {
        for (size_t x = 0; x < other._matrix.width(); x++) {
            for (size_t y = 0; y < other._matrix.height(); y++) {
                int tx = x + origin.x;
                int ty = y + origin.y;
                if (tx >= 0 && tx < _matrix.width() && ty >= 0 && ty < _matrix.height()) {
                    _matrix.at(tx, ty) = other._matrix.at(x, y).copy();
                }
            }
        }
        return *this;
    }

    matrix &write(const matrix &other,
                  const v2 &otherCenter,
                  float otherRotation = 0.0f,
                  const v2 &otherAnchor = v2::zero(),
                  bool blend_colors = true) {
        const int ow = int(other._matrix.width());
        const int oh = int(other._matrix.height());
        if (ow <= 0 || oh <= 0) return *this;

        const int dw = int(_matrix.width());
        const int dh = int(_matrix.height());

        const int angle = ((int(otherRotation) % 360) + 360) % 360;
        const int dst_cx = int(std::floor(otherCenter.x));
        const int dst_cy = int(std::floor(otherCenter.y));

        const int cx = ow / 2 - int(otherAnchor.x);
        const int cy = oh / 2 - int(otherAnchor.y);

        auto map_xy = [&](int x, int y, int &ox, int &oy) {
            const int dx = x - cx;
            const int dy = y - cy;
            switch (angle) {
                case 0: ox = dx;
                    oy = dy;
                    return;
                case 90: ox = -dy;
                    oy = dx;
                    return;
                case 180: ox = -dx;
                    oy = -dy;
                    return;
                case 270: ox = dy;
                    oy = -dx;
                    return;
                default: ox = dx;
                    oy = dy;
                    return;
            }
        };

        for (int sx = 0; sx < ow; ++sx) {
            for (int sy = 0; sy < oh; ++sy) {
                const color src = other._matrix(sx, sy);
                if (src.is_none()) continue;

                int ox, oy;
                map_xy(sx, sy, ox, oy);

                const int dx = dst_cx + ox;
                const int dy = dst_cy + oy;

                if ((unsigned) dx < (unsigned) dw && (unsigned) dy < (unsigned) dh) {
                    if (!blend_colors) {
                        _matrix(dx, dy) = src;
                    } else {
                        color &dst = _matrix(dx, dy);
                        if (dst.is_none()) {
                            dst = src;
                        } else {
                            dst = src;
                        }
                    }
                }
            }
        }

        return *this;
    }

    matrix &rotate(float degrees) {
        float radians = (degrees * M_PI) / 180.0f;
        float sinA = std::abs(std::sin(radians));
        float cosA = std::abs(std::cos(radians));

        int old_width = _matrix.width();
        int old_height = _matrix.height();
        int new_width = std::ceil(old_width * cosA + old_height * sinA);
        int new_height = std::ceil(old_width * sinA + old_height * cosA);

        matrix rotated(new_width, new_height, color::none());

        float cxOld = old_width / 2.0f;
        float cyOld = old_height / 2.0f;
        float cxNew = new_width / 2.0f;
        float cyNew = new_height / 2.0f;

        for (int x = 0; x < old_width; x++) {
            for (int y = 0; y < old_height; y++) {
                float dx = x - cxOld;
                float dy = y - cyOld;

                int rx = std::round(std::cos(radians) * dx - std::sin(radians) * dy + cxNew);
                int ry = std::round(std::sin(radians) * dx + std::cos(radians) * dy + cyNew);

                if (rx >= 0 && rx < new_width && ry >= 0 && ry < new_height) {
                    rotated._matrix.at(rx, ry) = _matrix(x, y);
                }
            }
        }

        _matrix = rotated._matrix;
        return *this;
    }

    matrix &scale(const float factor) {
        int old_width = _matrix.width();
        int old_height = _matrix.height();
        int new_width = std::round(old_width * factor);
        int new_height = std::round(old_height * factor);

        matrix scaled(new_width, new_height, color::none());

        for (int x = 0; x < new_width; x++) {
            for (int y = 0; y < new_height; y++) {
                int srcX = std::floor(x / factor);
                int srcY = std::floor(y / factor);
                if (srcX < old_width && srcY < old_height) {
                    scaled._matrix(x, y) = _matrix(srcX, srcY);
                }
            }
        }

        _matrix = scaled._matrix;
        return *this;
    }

    matrix snippet(const v2 &from, const v2 &to) const {
        matrix result(to.x - from.x, to.y - from.y);
        for (int x = from.x; x < to.x; x++) {
            for (int y = from.y; y < to.y; y++) {
                result._matrix.at(x - from.x, y - from.y) = _matrix.at(x, y);
            }
        }
        return result;
    }

    matrix &clear() {
        for (size_t y = 0; y < _matrix.height(); y++) {
            for (size_t x = 0; x < _matrix.width(); x++)
                _matrix.at(x, y) = color::none();
        }
        return *this;
    }

    matrix &print_to_console() {
        std::string result = "\n";
        for (size_t y = 0; y < _matrix.height(); y++) {
            result += "|";
            for (size_t x = 0; x < _matrix.width(); x++) {
                result += _matrix.at(x, y).is_none() ? "  " : "O ";
            }
            result += "|\n";
        }
        std::cout << result;
        return *this;
    }
};
