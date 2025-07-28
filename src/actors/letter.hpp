#pragma once

#include "../matrix.hpp"
#include "../color.hpp"
#include <string>

matrix letter(char c, const color& col = color::white(), bool reverse = false);
