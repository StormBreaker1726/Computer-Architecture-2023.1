//
// Created by joao-oliveira on 21/04/23.
//

#ifndef COMPUTER_ARCHITECTURE_2023_1_TOMASULO_H
#define COMPUTER_ARCHITECTURE_2023_1_TOMASULO_H

#include "mips_types.hpp"
#include "pipeline_phases.hpp"
// fld, fsd, fadd.d, fsub.d, fmul.d, fdiv.d
class Tomasulo
{
private:
    mips_byte_t instruction_memory[512];

public:
    Tomasulo();

    void start(bool step_by_step = false);
    void clear();
    void load_instruction(std::istream& input_file);

private:
    void register_dump();
    void memory_dump();

    void instruction_fetch(mips_word_t instruction);
    void instruction_decode(mips_word_t instruction);
    void execution(mips_word_t instruction);
    void mem(mips_word_t instruction);
    void write_back(mips_word_t instruction);
};

#endif  //COMPUTER_ARCHITECTURE_2023_1_TOMASULO_H
