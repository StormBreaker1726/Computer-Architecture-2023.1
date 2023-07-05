//
// Created by joao-oliveira on 21/04/23.
//

#pragma once

#include "Tomasulo/utils.hpp"
#include "Tomasulo/instructions.hpp"

#include <list>

class Tomasulo
{
private:
    mips_byte_t instruction_memory[1024];
    mips_byte_t data_memory[512];
    mips_word_t registers[64];
    mips_word_t register_status[64];
    mips_word_t pc;
    unsigned clock_cycle;

    std::list<mips_word_t> instructions_queue;

    ReservationStation _reservation_station[7];

    std::ostream& out;

public:
    Tomasulo(std::ostream& out);

    void start(bool step_by_step = false);
    void clear();
    void load_instructions(std::istream& input_file);

    void write_fp_to_memory(mips_float_t f, mips_word_t address);
    mips_float_t read_fp_from_memory(mips_word_t address);

private:
    void register_dump();
    void memory_dump();
    void print_instruction_queue();
    void reservation_station_dump();

    void issue();
    void execute();
    void write_result();

    void initialize_instruction_queue();

    mips_word_t get_reservation_station_result(const ReservationStation& rs);

    unsigned get_rs_num_for_instruction_type(InstructionType i_type);
    ReservationStation& reservation_station(unsigned r);

    bool reservation_station_is_empty();
};
