#pragma once

#include "mips_types.hpp"

#include "Tomasulo/instructions.hpp"

#include <string>

struct ReservationStation {
    bool busy;
    InstructionType operation;
    mips_word_t vj;
    mips_word_t vk;
    mips_word_t qj;
    mips_word_t qk;
    mips_word_t a;

    // Execution info
    unsigned cycles_executing;
};

inline mips_word_t _read_word(mips_byte_t mem[], mips_word_t pos)
{
    return *reinterpret_cast<mips_word_t *>(mem + pos);
}

inline mips_float_t interpret_word_as_float(mips_word_t w)
{
    return *reinterpret_cast<mips_float_t *>(&w);
}

inline mips_word_t str_to_instruction(const std::string& s)
{
    return std::stoul(s, nullptr, 2);
}
