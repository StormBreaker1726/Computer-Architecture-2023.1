#pragma once

#include "utils.hpp"

#include <cstdint>

using mips_word_t = uint32_t;
using mips_half_t = uint16_t;
using mips_byte_t = uint8_t;
using mips_int_t  = int32_t;
using mips_float_t = float;

static_assert(SIZE_IN_BITS(mips_float_t) == 32, "Invalid mips_float_t size");
