#ifndef COLORS_H
#define COLORS_H

#include <iostream>

enum Colors {
    Red = 31,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan
};


inline void set_color(int color) {
    std::cout << "\033[" << color << "m";
}


inline void reset_color() {
    std::cout << "\033[0m";
}

#endif //COLORS_H
