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

    matrix &write(const matrix &other, const v2 &otherCenter, float otherRotation = 0.0f,
                  const v2 &otherAnchor = v2::zero(), bool blend_colors = true) {
        struct OverlayCell {
            bool has = false;
            color c{}; // premultiplied accumulator
        };

        auto over_blend = [](const color &dst, const color &src) -> color {
            // standard alpha-over: src over dst
            float sa = src.a, da = dst.a;
            float outA = sa + da * (1.0f - sa);
            if (outA <= 0.0f) return color(0, 0, 0, 0);
            float r = (src.r * sa + dst.r * da * (1.0f - sa)) / outA;
            float g = (src.g * sa + dst.g * da * (1.0f - sa)) / outA;
            float b = (src.b * sa + dst.b * da * (1.0f - sa)) / outA;
            return color(r, g, b, outA);
        };

        grid2d<OverlayCell> overlay(_matrix.width(), _matrix.height());

        int otherWidth = other._matrix.width();
        int otherHeight = otherWidth > 0 ? other._matrix.height() : 0;
        if (otherWidth == 0 || otherHeight == 0) return *this;

        v2 center(otherWidth / 2.0f, otherHeight / 2.0f);

        for (int x = 0; x < otherWidth; x++) {
            for (int y = 0; y < otherHeight; y++) {
                color c = other.pixels().at(x, y);
                if (c.is_none()) continue;

                int dx = x - static_cast<int>(center.x - otherAnchor.x);
                int dy = y - static_cast<int>(center.y - otherAnchor.y);

                int rx = 0, ry = 0;
                int angle = static_cast<int>(otherRotation) % 360;
                if (angle < 0) angle += 360;

                v2 offset = v2::one();

                switch (angle) {
                    case 0: rx = dx;
                        ry = dy;
                        break;
                    case 90: rx = -dy;
                        ry = dx;
                        offset = v2(0, 1);
                        break;
                    case 180: rx = -dx;
                        ry = -dy;
                        offset = v2::zero();
                        break;
                    case 270: rx = dy;
                        ry = -dx;
                        offset = v2(1, 0);
                        break;
                    default: continue;
                }

                int finalX = static_cast<int>(std::floor(rx + otherCenter.x + offset.x * 0.5f));
                int finalY = static_cast<int>(std::floor(ry + otherCenter.y + offset.y * 0.5f));

                if (finalX >= 0 && finalX < static_cast<int>(_matrix.width()) &&
                    finalY >= 0 && finalY < static_cast<int>(_matrix.height())) {
                    OverlayCell &cell = overlay.at(finalX, finalY);
                    if (!cell.has) {
                        cell.has = true;
                        cell.c = c;
                    } else {
                        // kumulacja bez wektorów
                        cell.c = over_blend(cell.c, c);
                    }
                }
            }
        }

        // zastosowanie do bufora docelowego
        for (size_t x = 0; x < overlay.width(); x++) {
            for (size_t y = 0; y < overlay.height(); y++) {
                OverlayCell &cell = overlay.at(x, y);
                if (!cell.has) continue;

                if (blend_colors) {
                    _matrix.at(x, y) = over_blend(_matrix.at(x, y), cell.c);
                } else {
                    _matrix.at(x, y) = cell.c;
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
