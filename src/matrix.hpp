#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include "v2.hpp"
#include "color.hpp"

enum class color_blending {
    no_blending,
    light_alike,
    weighted_by_opacity,
};

class matrix {
protected:
    std::vector<std::vector<color> > _matrix;

public:
    matrix(v2 size, const color &default_color = color::none()): matrix(size.x, size.y, default_color) {
    }

    matrix(int x, int y, const color &default_color = color::none()) {
        x = std::round(x);
        y = std::round(y);
        _matrix = std::vector<std::vector<color> >(x, std::vector<color>(y, default_color));
    }

    const std::vector<std::vector<color> > &pixels() const {
        return _matrix;
    }

    const uint width() const {
        return _matrix.size();
    }

    const uint height() const {
        return _matrix[0].size();
    }

    void set_pixel_v2(const v2 &point, const color &color) {
        _matrix[point.x][point.y] = color.copy();
    }

    void set_pixel(int x, int y, const color &color) {
        _matrix[x][y] = color.copy();
    }

    matrix &write_at_origin(const matrix &other, const v2 &origin) {
        for (size_t x = 0; x < other._matrix.size(); x++) {
            for (size_t y = 0; y < other._matrix[x].size(); y++) {
                int tx = x + origin.x;
                int ty = y + origin.y;
                if (tx >= 0 && tx < _matrix.size() && ty >= 0 && ty < _matrix[0].size()) {
                    _matrix[tx][ty] = other._matrix[x][y].copy();
                }
            }
        }
        return *this;
    }

    matrix &write(const matrix &other, const v2 &otherCenter, float otherRotation = 0.0f, const v2 &otherAnchor = v2::zero(), bool blend_colors = true) {
        std::vector<std::vector<std::vector<color> > > result(_matrix.size(),
                                                              std::vector<std::vector<color> >(_matrix[0].size()));

        int otherWidth = other._matrix.size();
        int otherHeight = otherWidth > 0 ? other._matrix[0].size() : 0;
        if (otherWidth == 0 || otherHeight == 0) return *this;

        v2 center(otherWidth / 2.0f, otherHeight / 2.0f);

        for (int x = 0; x < otherWidth; x++) {
            for (int y = 0; y < otherHeight; y++) {
                color c = other.pixels()[x][y];
                if (c.is_none()) continue;

                int dx = x - static_cast<int>(center.x - otherAnchor.x);
                int dy = y - static_cast<int>(center.y - otherAnchor.y);

                int rx = 0, ry = 0;

                int angle = static_cast<int>(otherRotation) % 360;
                if (angle < 0) angle += 360;

                v2 offset = v2::one();

                switch (angle) {
                    case 0:
                        rx = dx;
                        ry = dy;
                        break;
                    case 90:
                        rx = -dy;
                        ry = dx;
                        offset = v2(0, 1);
                        break;
                    case 180:
                        rx = -dx;
                        ry = -dy;
                        offset = v2::zero();
                        break;
                    case 270:
                        rx = dy;
                        ry = -dx;
                        offset = v2(1, 0);
                        break;
                    default:
                        continue;
                }

                int finalX = static_cast<int>(std::floor(rx + otherCenter.x + offset.x * 0.5f));
                int finalY = static_cast<int>(std::floor(ry + otherCenter.y + offset.y * 0.5f));

                if (finalX >= 0 && finalX < static_cast<int>(_matrix.size()) &&
                    finalY >= 0 && finalY < static_cast<int>(_matrix[0].size())) {
                    result[finalX][finalY].push_back(c);
                }
            }
        }

        for (size_t x = 0; x < result.size(); x++) {
            for (size_t y = 0; y < result[0].size(); y++) {
                if (blend_colors) {
                    std::vector<color> combined = {_matrix[x][y]};
                    combined.insert(combined.end(), result[x][y].begin(), result[x][y].end());
                    _matrix[x][y] = color::blend_colors(combined);
                } else {
                    for (color color_: result[x][y])
                        _matrix[x][y] = color_;
                }
            }
        }

        return *this;
    }

    matrix &rotate(float degrees) {
        float radians = (degrees * M_PI) / 180.0f;
        float sinA = std::abs(std::sin(radians));
        float cosA = std::abs(std::cos(radians));

        int old_width = _matrix.size();
        int old_height = _matrix[0].size();
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
                    rotated._matrix[rx][ry] = _matrix[x][y];
                }
            }
        }

        _matrix = rotated._matrix;
        return *this;
    }

    matrix &scale(const float factor) {
        int old_width = _matrix.size();
        int old_height = _matrix[0].size();
        int new_width = std::round(old_width * factor);
        int new_height = std::round(old_height * factor);

        matrix scaled(new_width, new_height, color::none());

        for (int x = 0; x < new_width; x++) {
            for (int y = 0; y < new_height; y++) {
                int srcX = std::floor(x / factor);
                int srcY = std::floor(y / factor);
                if (srcX < old_width && srcY < old_height) {
                    scaled._matrix[x][y] = _matrix[srcX][srcY];
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
                result._matrix[x - from.x][y - from.y] = _matrix[x][y];
            }
        }
        return result;
    }

    matrix &clear() {
        for (auto &col: _matrix)
            for (auto &px: col)
                px = color::none();
        return *this;
    }

    matrix &print_to_console() {
        std::string result = "\n";
        for (size_t y = 0; y < _matrix[0].size(); y++) {
            result += "|";
            for (size_t x = 0; x < _matrix.size(); x++) {
                result += _matrix[x][y].is_none() ? "  " : "O ";
            }
            result += "|\n";
        }
        std::cout << result;
        return *this;
    }
};
