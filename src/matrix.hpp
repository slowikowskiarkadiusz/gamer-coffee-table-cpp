#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include "v2.hpp"
#include "color.hpp"

class matrix {
protected:
    std::vector<std::vector<color> > _matrix;

public:
    matrix(int x, int y, const color &defaultColor = color::none()) {
        x = std::round(x);
        y = std::round(y);
        _matrix = std::vector<std::vector<color> >(x, std::vector<color>(y, defaultColor));
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

    matrix &write(const matrix &other, const v2 &otherCenter, float otherRotation = 0.0f, bool debuuuuug = false) {
        std::vector<std::vector<std::vector<color> > > result(_matrix.size(),
                                                              std::vector<std::vector<color> >(_matrix[0].size()));

        float radians = (otherRotation * M_PI) / 180.0f;
        float cosA = std::cos(radians);
        float sinA = std::sin(radians);

        int otherWidth = other._matrix.size();
        int otherHeight = otherWidth > 0 ? other._matrix[0].size() : 0;
        if (otherWidth == 0 || otherHeight == 0) return *this;

        v2 center(otherWidth / 2.0f, otherHeight / 2.0f);

        for (int x = 0; x < otherWidth; x++) {
            for (int y = 0; y < otherHeight; y++) {
                color color = other.pixels()[x][y];
                if (color.is_none()) continue;

                float dx = x - center.x;
                float dy = y - center.y;

                float rx = std::floor(cosA * dx - sinA * dy + otherCenter.x + 0.5);
                float ry = std::floor(sinA * dx + cosA * dy + otherCenter.y + 0.5);

                if (rx >= 0 && rx < _matrix.size() && ry >= 0 && ry < _matrix[0].size()) {
                    result[rx][ry].push_back(color);
                }
            }
        }

        for (size_t x = 0; x < result.size(); x++) {
            for (size_t y = 0; y < result[0].size(); y++) {
                std::vector<color> combined = {_matrix[x][y]};
                combined.insert(combined.end(), result[x][y].begin(), result[x][y].end());
                _matrix[x][y] = color::blend_colors(combined);
            }
        }

        return *this;
    }

    matrix &rotate(float degrees) {
        float radians = (degrees * M_PI) / 180.0f;
        float sinA = std::abs(std::sin(radians));
        float cosA = std::abs(std::cos(radians));

        int oldWidth = _matrix.size();
        int oldHeight = _matrix[0].size();
        int newWidth = std::ceil(oldWidth * cosA + oldHeight * sinA);
        int newHeight = std::ceil(oldWidth * sinA + oldHeight * cosA);

        matrix rotated(newWidth, newHeight, color::none());

        float cxOld = oldWidth / 2.0f;
        float cyOld = oldHeight / 2.0f;
        float cxNew = newWidth / 2.0f;
        float cyNew = newHeight / 2.0f;

        for (int x = 0; x < oldWidth; x++) {
            for (int y = 0; y < oldHeight; y++) {
                float dx = x - cxOld;
                float dy = y - cyOld;

                int rx = std::round(std::cos(radians) * dx - std::sin(radians) * dy + cxNew);
                int ry = std::round(std::sin(radians) * dx + std::cos(radians) * dy + cyNew);

                if (rx >= 0 && rx < newWidth && ry >= 0 && ry < newHeight) {
                    rotated._matrix[rx][ry] = _matrix[x][y];
                }
            }
        }

        _matrix = rotated._matrix;
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
