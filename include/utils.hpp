#ifndef D7E43EE3_989B_4725_8949_FF2382DF549E
#define D7E43EE3_989B_4725_8949_FF2382DF549E

#include "defines.hpp"

#define ARRAYSIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

inline void clear_istream(std::istream& in)
{
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#define press_enter clear_istream

#endif /* D7E43EE3_989B_4725_8949_FF2382DF549E */
