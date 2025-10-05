#include "letter.hpp"
#include <stdexcept>

matrix letter(char c, const color &col, bool reverse) {
    char upper = std::toupper(c);
    matrix m(4, 5, reverse ? col : color::none());
    color fg = reverse ? color::none() : col;

    auto set = [&](int x, int y) { m.set_pixel(x, y, fg); };

    switch (upper) {
        case 'A':
            set(1, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(1, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(2, 4);
            break;
        case 'B':
            set(0, 0);
            set(1, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(1, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(1, 4);
            break;
        case 'C':
            set(1, 0);
            set(2, 0);
            set(0, 1);
            set(0, 2);
            set(0, 3);
            set(1, 4);
            set(2, 4);
            break;
        case 'D':
            set(0, 0);
            set(1, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(1, 4);
            break;
        case 'E':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(0, 1);
            set(0, 2);
            set(1, 2);
            set(0, 3);
            set(0, 4);
            set(1, 4);
            set(2, 4);
            break;
        case 'F':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(0, 1);
            set(0, 2);
            set(1, 2);
            set(0, 3);
            set(0, 4);
            break;
        case 'G':
            set(1, 0);
            set(2, 0);
            set(0, 1);
            set(0, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(1, 4);
            set(2, 4);
            break;
        case 'H':
            set(0, 0);
            set(2, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(1, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(2, 4);
            break;
        case 'I':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(1, 1);
            set(1, 2);
            set(1, 3);
            set(0, 4);
            set(1, 4);
            set(2, 4);
            break;
        case 'J':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(2, 1);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(1, 4);
            break;
        case 'K':
            set(0, 0);
            set(2, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(1, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(2, 4);
            break;
        case 'L':
            set(0, 0);
            set(0, 1);
            set(0, 2);
            set(0, 3);
            set(0, 4);
            set(1, 4);
            set(2, 4);
            break;
        case 'M':
            set(0, 0);
            set(2, 0);
            set(0, 1);
            set(1, 1);
            set(2, 1);
            set(0, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(2, 4);
            break;
        case 'N':
            set(0, 0);
            set(2, 0);
            set(0, 1);
            set(1, 1);
            set(2, 1);
            set(0, 2);
            set(1, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(2, 4);
            break;
        case 'O':
            set(1, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(1, 4);
            break;
        case 'P':
            set(0, 0);
            set(1, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(1, 2);
            set(0, 3);
            set(0, 4);
            break;
        case 'Q':
            set(1, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(2, 2);
            set(1, 3);
            set(2, 4);
            break;
        case 'R':
            set(0, 0);
            set(1, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(1, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(2, 4);
            break;
        case 'S':
            set(1, 0);
            set(2, 0);
            set(0, 1);
            set(1, 2);
            set(2, 3);
            set(0, 4);
            set(1, 4);
            break;
        case 'T':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(1, 1);
            set(1, 2);
            set(1, 3);
            set(1, 4);
            break;
        case 'U':
            set(0, 0);
            set(2, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(1, 4);
            break;
        case 'V':
            set(0, 0);
            set(2, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(2, 2);
            set(1, 3);
            set(1, 4);
            break;
        case 'W':
            set(0, 0);
            set(2, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(2, 2);
            set(0, 3);
            set(1, 3);
            set(2, 3);
            set(0, 4);
            set(2, 4);
            break;
        case 'X':
            set(0, 0);
            set(2, 0);
            set(1, 1);
            set(1, 2);
            set(1, 3);
            set(0, 4);
            set(2, 4);
            break;
        case 'Y':
            set(0, 0);
            set(2, 0);
            set(0, 1);
            set(2, 1);
            set(1, 2);
            set(1, 3);
            set(1, 4);
            break;
        case 'Z':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(2, 1);
            set(1, 2);
            set(0, 3);
            set(0, 4);
            set(1, 4);
            set(2, 4);
            break;
        case '0':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(1, 4);
            set(2, 4);
            break;
        case '1':
            set(1, 0);
            set(0, 1);
            set(1, 1);
            set(1, 2);
            set(1, 3);
            set(0, 4);
            set(1, 4);
            set(2, 4);
            break;
        case '2':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(2, 1);
            set(2, 2);
            set(1, 2);
            set(0, 3);
            set(0, 4);
            set(1, 4);
            set(2, 4);
            break;
        case '3':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(2, 1);
            set(2, 2);
            set(1, 2);
            set(2, 3);
            set(0, 4);
            set(1, 4);
            set(2, 4);
            break;
        case '4':
            set(0, 0);
            set(2, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(1, 2);
            set(2, 2);
            set(2, 3);
            set(2, 4);
            break;
        case '5':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(0, 1);
            set(0, 2);
            set(1, 2);
            set(2, 2);
            set(2, 3);
            set(0, 4);
            set(1, 4);
            break;
        case '6':
            set(1, 0);
            set(2, 0);
            set(0, 1);
            set(0, 2);
            set(1, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(1, 4);
            set(2, 4);
            break;
        case '7':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(2, 1);
            set(1, 2);
            set(1, 3);
            set(1, 4);
            break;
        case '8':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(0, 1);
            set(2, 1);
            set(0, 2);
            set(1, 2);
            set(2, 2);
            set(0, 3);
            set(2, 3);
            set(0, 4);
            set(1, 4);
            set(2, 4);
            break;
        case '9':
            set(0, 0);
            set(1, 0);
            set(2, 0);
            set(0, 1);
            set(2, 1);
            set(1, 2);
            set(2, 2);
            set(2, 3);
            set(0, 4);
            set(1, 4);
            break;
        case '.': m = matrix(1, 5);
            m.set_pixel(0, 4, fg);
            break;
        case '-': m = matrix(2, 5);
            m.set_pixel(0, 2, fg);
            m.set_pixel(1, 2, fg);
            break;
        case '+': m = matrix(3, 5);
            m.set_pixel(0, 2, fg);
            m.set_pixel(1, 2, fg);
            m.set_pixel(1, 1, fg);
            m.set_pixel(1, 3, fg);
            m.set_pixel(2, 2, fg);
            break;
        case ' ': m = matrix(1, 5);
            break;
        default:
            // #ifdef prod
            std::cout << "Heap is empty" << std::endl;
            abort();
            // #else
            // throw std::runtime_error("unrecognized letter: " + std::string(1, c));
            // #endif
    }

    return m;
}
