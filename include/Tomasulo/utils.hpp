#pragma once

#include "mips_types.hpp"
#include <string>

inline mips_word_t _read_word(mips_byte_t mem[], mips_word_t pos)
{
    return *((mips_word_t *) (mem + pos));
}

struct Line {
    bool busy;
    std::string address;
};

/* Reservation station line */
struct RSLine {
    bool busy;
    mips_word_t operation;
    std::string vj;
    std::string vk;
    std::string qj;
    std::string qk;
};
