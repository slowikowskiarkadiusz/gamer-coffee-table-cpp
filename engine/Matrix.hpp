//
// Created by Arkadiusz on 27/07/2025.
//

#ifndef MATRIX_HPP
#define MATRIX_HPP

#endif //MATRIX_HPP

#pragma once

#include <vector>
#include <cmath>
#include <iostream>
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
                if ((x + origin.x) >= 0 && (x + origin.x) < matrix.size() &&
                    (y + origin.y) >= 0 && (y + origin.y) < matrix[0].size()) {
                    matrix[x + origin.x][y + origin.y] = other.matrix[x][y].copy();
                }
            }
        }
        return *this;
    }

    Matrix &clear() {
        for (auto &col: matrix)
            for (auto &px: col)
                px = Color::none();
        return *this;
    }

    void printToConsole() const {
        std::string result = "\n";
        for (size_t y = 0; y < matrix[0].size(); y++) {
            for (size_t x = 0; x < matrix.size(); x++) {
                result += matrix[x][y].isNone() ? "  " : "O ";
            }
            result += "|\n";
        }
        std::cout << result;
    }

    // Add rotate(), write(), snippet() as needed with similar logic
};
