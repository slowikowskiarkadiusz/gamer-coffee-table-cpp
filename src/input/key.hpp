#pragma once

enum class key {
    P1_L_L,
    P1_L_BLUE,
    P1_L,
    P1_R_R,
    P1_R_GREEN,
    P1_R,
    P2_L_L,
    P2_L_BLUE,
    P2_L,
    P2_R_R,
    P2_R_GREEN,
    P2_R,
    START
};

inline uint keys_length = static_cast<uint>(key::START);
