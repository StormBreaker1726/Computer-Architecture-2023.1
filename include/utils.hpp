#ifndef D7E43EE3_989B_4725_8949_FF2382DF549E
#define D7E43EE3_989B_4725_8949_FF2382DF549E

#include "defines.hpp"

inline void clear_input(std::istream& in)
{
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#endif /* D7E43EE3_989B_4725_8949_FF2382DF549E */