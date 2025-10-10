#pragma once

#include <optional>

#include "tetris_block.hpp"

class hold_logic {
    v2 size;
    matrix _matrix;
    std::optional<tetris_shape> heldPiece;

public:
    static v2 default_size;
    v2 center;

    hold_logic(v2 center) : center(center), size(default_size), _matrix(matrix(size.x, size.y, color::none())) {
    }

    matrix* render() {
        return &_matrix;
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
