#pragma once

#include <optional>

#include "tetris_block.hpp"

class hold_logic {
private:
    matrix _matrix;
    std::optional<tetris_shape> heldPiece;
    v2 size;

public:
    v2 center;

    hold_logic(v2 center) : center(center), size(v2::one().mul(4)), _matrix(matrix(size.x, size.y, color::none())) {
    }

    matrix render() const {
        return _matrix;
    }

    std::optional<tetris_shape> use(tetris_shape shape) {
        std::optional<tetris_shape> prevPiece = heldPiece;
        heldPiece = shape;

        _matrix = tetris_block::generate_shape(*heldPiece, 0, false);
        return prevPiece;
    }

    v2 get_size() const {
        return size;
    }
};
