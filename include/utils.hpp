#pragma once

#include "defines.hpp"

#define ARRAYSIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))
#define SIZE_IN_BITS(expr) (sizeof(expr) << 3)

inline void clear_istream(std::istream& in)
{
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#define press_enter() clear_istream(std::cin)
