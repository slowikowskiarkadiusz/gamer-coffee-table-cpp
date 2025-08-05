#pragma once

enum class key {
    P1_DOWN,
    P1_UP,
    P1_LEFT,
    P1_RIGHT,
    P1_ANY_DIRECTION,
    P1_BLUE,
    P1_GREEN,
    P1_ANY,
    P2_DOWN,
    P2_UP,
    P2_LEFT,
    P2_RIGHT,
    P2_ANY_DIRECTION,
    P2_BLUE,
    P2_GREEN,
    P2_ANY,
    START
};

inline int keys_length = static_cast<int>(key::START);
