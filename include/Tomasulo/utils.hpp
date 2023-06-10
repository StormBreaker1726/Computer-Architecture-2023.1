#pragma once

#include "mips_types.hpp"
#include <string>

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
