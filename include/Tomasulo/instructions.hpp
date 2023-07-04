#pragma once

#include <stdexcept>

#include "mips_types.hpp"

enum class InstructionType { ADD = 1, SUB, MULT, DIV, SW, LW };
enum class OpCode { R = 0x53, S = 0x27, I = 0x07 };

inline mips_word_t instruction_opcode(mips_word_t instruction)
{
    return instruction & 0x7f;
}

inline mips_word_t instruction_funct7(mips_word_t instruction)
{
    return (instruction >> 25) & 0x7F;
}

inline mips_word_t instruction_rs1(mips_word_t instruction)
{
    /* rs1: [19-15] */
    return (instruction >> 15) & 0x1F;
}

inline mips_word_t instruction_rs2(mips_word_t instruction)
{
    /* rs2: [24-20] */
    return (instruction >> 20) & 0x1F;
}

inline mips_word_t instruction_rd(mips_word_t instruction)
{
    /* rd: [11-7] */
    return (instruction >> 7) & 0x1F;
}

inline mips_word_t instruction_imm_i(mips_word_t instruction)
{
    /* Pro tipo I: IMM: [31-20] */
    return (instruction >> 20) & 0xFFF;
}

inline mips_word_t instruction_imm_s(mips_word_t instruction)
{
    /* Pro tipo S IMM: [31-25]*/
    return (instruction >> 25) & 0x3F;
}

inline InstructionType type_from_funct(mips_word_t instruction)
{
    switch (instruction_funct7(instruction)) {
        case 0x1: return InstructionType::ADD;
        case 0x5: return InstructionType::SUB;
        case 0x9: return InstructionType::MULT;
        case 0xD: return InstructionType::DIV;
    }
    throw std::invalid_argument(__FUNCTION__);
}

inline InstructionType instruction_type(mips_word_t instruction)
{
    OpCode opcode = static_cast<OpCode>(instruction_opcode(instruction));

    switch (opcode) {
        case OpCode::S: return InstructionType::SW;
        case OpCode::I: return InstructionType::LW;
        case OpCode::R: return type_from_funct(instruction);
    }
    throw std::invalid_argument(__FUNCTION__);
}
