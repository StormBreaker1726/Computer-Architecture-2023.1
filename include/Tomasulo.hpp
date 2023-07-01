//
// Created by joao-oliveira on 21/04/23.
//

#pragma once

#include "Tomasulo/utils.hpp"
#include "Registers.hpp"

class Tomasulo
{
private:
    mips_byte_t instruction_memory[1024];
    mips_byte_t data_memory[512];
    mips_word_t registers[64];
    mips_word_t register_status[64];
    mips_word_t pc;
    size_t clock;

    std::list<mips_word_t> instructions_queue;

    ReservationStation _reservation_station[7];

public:
    Tomasulo();

    void start(bool step_by_step = false);
    void clear();
    void load_instructions(std::istream& input_file);

private:
    void register_dump();
    void memory_dump();
    void print_instruction_queue();
    void print_reservation_stations();

    void issue();
    void execute();
    void write_result();

    void instruction_fetch(mips_word_t instruction);
    void instruction_decode(mips_word_t instruction);
    void execution(mips_word_t instruction);
    void mem(mips_word_t instruction);
    void write_back(mips_word_t instruction);

    unsigned get_rs_for_instruction_type(mips_word_t i_type);
    ReservationStation& reservation_station(unsigned r);
};
