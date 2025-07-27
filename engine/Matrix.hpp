#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include "v2.hpp"
#include "Color.hpp"

class Matrix {
protected:
    std::vector<std::vector<Color> > matrix;

public:
    Matrix(int x, int y, const Color &defaultColor = Color::none()) {
        x = std::round(x);
        y = std::round(y);
        matrix = std::vector<std::vector<Color> >(x, std::vector<Color>(y, defaultColor));
    }

    const std::vector<std::vector<Color> > &pixels() const {
        return matrix;
    }

    void setPixelV2(const v2 &point, const Color &color) {
        matrix[point.x][point.y] = color.copy();
    }

    void setPixel(int x, int y, const Color &color) {
        matrix[x][y] = color.copy();
    }

    Matrix &writeAtOrigin(const Matrix &other, const v2 &origin) {
        for (size_t x = 0; x < other.matrix.size(); x++) {
            for (size_t y = 0; y < other.matrix[x].size(); y++) {
                int tx = x + origin.x;
                int ty = y + origin.y;
                if (tx >= 0 && tx < matrix.size() && ty >= 0 && ty < matrix[0].size()) {
                    matrix[tx][ty] = other.matrix[x][y].copy();
                }
            }
        }
        return *this;
    }

    Matrix &write(const Matrix &other, const v2 &otherCenter, float otherRotation = 0.0f) {
        std::vector<std::vector<std::vector<Color> > > result(matrix.size(),
                                                              std::vector<std::vector<Color> >(matrix[0].size()));

        float radians = (otherRotation * M_PI) / 180.0f;
        float cosA = std::cos(radians);
        float sinA = std::sin(radians);

        int otherWidth = other.matrix.size();
        int otherHeight = otherWidth > 0 ? other.matrix[0].size() : 0;
        if (otherWidth == 0 || otherHeight == 0) return *this;

        v2 center(otherWidth / 2.0f, otherHeight / 2.0f);

        for (int x = 0; x < otherWidth; x++) {
            for (int y = 0; y < otherHeight; y++) {
                Color color = other.pixels()[x][y];
                if (color.isNone()) continue;

                float dx = x - center.x;
                float dy = y - center.y;

                int rx = std::round(cosA * dx - sinA * dy + otherCenter.x);
                int ry = std::round(sinA * dx + cosA * dy + otherCenter.y);

                if (rx >= 0 && rx < matrix.size() && ry >= 0 && ry < matrix[0].size()) {
                    result[rx][ry].push_back(color);
                }
            }
        }

        for (size_t x = 0; x < result.size(); x++) {
            for (size_t y = 0; y < result[0].size(); y++) {
                std::vector<Color> combined = {matrix[x][y]};
                combined.insert(combined.end(), result[x][y].begin(), result[x][y].end());
                matrix[x][y] = Color::blendColors(combined);
            }
        }

        return *this;
    }

    Matrix &rotate(float degrees) {
        float radians = (degrees * M_PI) / 180.0f;
        float sinA = std::abs(std::sin(radians));
        float cosA = std::abs(std::cos(radians));

        int oldWidth = matrix.size();
        int oldHeight = matrix[0].size();
        int newWidth = std::ceil(oldWidth * cosA + oldHeight * sinA);
        int newHeight = std::ceil(oldWidth * sinA + oldHeight * cosA);

        Matrix rotated(newWidth, newHeight, Color::none());

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
                    rotated.matrix[rx][ry] = matrix[x][y];
                }
            }
        }

        matrix = rotated.matrix;
        return *this;
    }

    Matrix snippet(const v2 &from, const v2 &to) const {
        Matrix result(to.x - from.x, to.y - from.y);
        for (int x = from.x; x < to.x; x++) {
            for (int y = from.y; y < to.y; y++) {
                result.matrix[x - from.x][y - from.y] = matrix[x][y];
            }
        }
        return result;
    }

    Matrix &clear() {
        for (auto &col: matrix)
            for (auto &px: col)
                px = Color::none();
        return *this;
    }

    Matrix &printToConsole() {
        std::string result = "\n";
        for (size_t y = 0; y < matrix[0].size(); y++) {
            for (size_t x = 0; x < matrix.size(); x++) {
                result += matrix[x][y].isNone() ? "  " : "O ";
            }
            result += "|\n";
        }
        std::cout << result;
        return *this;
    }
};
