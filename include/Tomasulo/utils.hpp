#pragma once

#include "mips_types.hpp"
#include <string>

inline mips_word_t _read_word(mips_byte_t mem[], mips_word_t pos)
{
    return *((mips_word_t *) (mem + pos));
}

/* Reservation station line */
struct ReservationStation {
    bool busy;
    mips_word_t operation;
    mips_word_t vj;
    mips_word_t vk;
    mips_word_t qj;
    mips_word_t qk;
    mips_word_t a;
};
