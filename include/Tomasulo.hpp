//
// Created by joao-oliveira on 21/04/23.
//

#ifndef COMPUTER_ARCHITECTURE_2023_1_TOMASULO_H
#define COMPUTER_ARCHITECTURE_2023_1_TOMASULO_H

#include "Registers.hpp"
// fld, fsd, fadd.d, fsub.d, fmul.d, fdiv.d
class Tomasulo
{
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

    mips_byte_t instruction_memory[1024];
    mips_byte_t data_memory[512];
    mips_word_t registers[64];
    mips_word_t pc;

    Control _control;
    IF_ID   _if_id;
    ID_EX   _id_ex;
    EX_MEM  _ex_mem;
    MEM_WB  _mem_wb;
};

#endif  //COMPUTER_ARCHITECTURE_2023_1_TOMASULO_H
