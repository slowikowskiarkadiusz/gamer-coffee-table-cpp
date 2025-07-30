#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "../../matrix.hpp"
#include "tetris_block_colors.cpp"
#include "tetris_shape.cpp"

class tetris_block {
private:
    matrix _matrix;
    int blockRotation = 0;
    v2 center;
    tetris_shape shape;

public:
    tetris_block(v2 center, tetris_shape shape, bool isShadow)
        : center(center), shape(shape), _matrix(generateShape(shape, 0, isShadow)) {
    }

    tetris_shape getShape() const {
        return shape;
    }

    int getBlockRotation() const {
        return blockRotation;
    }

    matrix render() const {
        return _matrix;
    }

    void rotateBlock(int by) {
        if (by < 0) by = 360 + by;
        blockRotation = (blockRotation + by) % 360;
        _matrix = rotateMatrix(_matrix, by);
    }

    std::vector<v2> getTakenSpots() const {
        v2 start = center.floor().sub(v2(_matrix.width() / 2, _matrix.height() / 2));
        std::vector<v2> coords;

        for (int x = start.x; x < start.x + _matrix.width(); ++x) {
            for (int y = start.y; y < start.y + _matrix.height(); ++y) {
                if (!_matrix.pixels()[x - start.x][y - start.y].is_none())
                    coords.emplace_back(x, y);
            }
        }

        return coords;
    }

    std::vector<v2> getKicks(int to) const {
        std::string kickKey = std::to_string(to % 360) + "->" + std::to_string(blockRotation);
        std::string setKey = (shape == tetris_shape::I) ? "I" : "Others";

        if (shape == tetris_shape::O)
            return {};

        const auto &raw = kicks[setKey][kickKey];
        std::vector<v2> result;
        for (const auto &pair: raw)
            result.emplace_back(pair[0], pair[1]);
        return result;
    }

    static matrix generateShape(tetris_shape shape, int rotation, bool isShadow) {
        v2 size = getSize(shape);
        matrix matrix(size.x, size.y);
        color color = isShadow ? color::white(0.2f) : tetris_block_colors.at(shape);

        switch (shape) {
            case tetris_shape::I:
                for (int i = 0; i < size.x; ++i)
                    matrix.set_pixel(i, 1, color);
                break;
            case tetris_shape::O:
                for (int x = 0; x < 2; ++x)
                    for (int y = 0; y < 2; ++y)
                        matrix.set_pixel(x + 1, y, color);
                break;
            case tetris_shape::T:
                matrix.set_pixel(0, 1, color);
                matrix.set_pixel(1, 1, color);
                matrix.set_pixel(2, 1, color);
                matrix.set_pixel(1, 0, color);
                break;
            case tetris_shape::S:
                matrix.set_pixel(0, 1, color);
                matrix.set_pixel(1, 1, color);
                matrix.set_pixel(1, 0, color);
                matrix.set_pixel(2, 0, color);
                break;
            case tetris_shape::Z:
                matrix.set_pixel(0, 0, color);
                matrix.set_pixel(1, 0, color);
                matrix.set_pixel(1, 1, color);
                matrix.set_pixel(2, 1, color);
                break;
            case tetris_shape::J:
                for (int i = 0; i < 3; ++i)
                    matrix.set_pixel(i, 1, color);
                matrix.set_pixel(0, 0, color);
                break;
            case tetris_shape::L:
                for (int i = 0; i < 3; ++i)
                    matrix.set_pixel(i, 1, color);
                matrix.set_pixel(2, 0, color);
                break;
        }

        matrix.rotate(rotation);
        return matrix;
    }

private:
    matrix rotateMatrix(const matrix &mat, int degrees) {
        matrix newMatrix = (degrees == 90 || degrees == 270)
                               ? matrix(mat.height(), mat.width())
                               : matrix(mat.width(), mat.height());

        for (int i = 0; i < mat.height(); ++i) {
            for (int j = 0; j < mat.width(); ++j) {
                auto pixel = mat.pixels()[j][i];
                if (degrees == 90)
                    newMatrix.set_pixel(i, mat.width() - 1 - j, pixel);
                else if (degrees == 180)
                    newMatrix.set_pixel(mat.width() - 1 - j, mat.height() - 1 - i, pixel);
                else if (degrees == 270)
                    newMatrix.set_pixel(mat.height() - 1 - i, j, pixel);
                else
                    newMatrix.set_pixel(j, i, pixel);
            }
        }

        return newMatrix;
    }

    static v2 getSize(tetris_shape shape) {
        switch (shape) {
            case tetris_shape::I: return v2(4, 4);
            case tetris_shape::O: return v2(4, 3);
            default: return v2(3, 3);
        }
    }

    const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::array<int, 2> > > > tetris_block::kicks = {
        {
            "I", {
                {"0->90", {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}},
                {"90->0", {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}},
                {"90->180", {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}},
                {"180->90", {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}},
                {"180->270", {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}},
                {"270->180", {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}},
                {"270->0", {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}},
                {"0->270", {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}}
            }
        },
        {
            "Others", {
                {"0->90", {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}},
                {"90->0", {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}},
                {"90->180", {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}},
                {"180->90", {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}},
                {"180->270", {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}},
                {"270->180", {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},
                {"270->0", {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},
                {"0->270", {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}}
            }
        }
    };
};
