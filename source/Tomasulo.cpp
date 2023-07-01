//
// Created by joao-oliveira on 21/04/23.
//

#include "defines.hpp"

#include "Tomasulo.hpp"
#include "Tomasulo/instructions.hpp"

#include <string>

#define RISCV_EOI 0

Tomasulo::Tomasulo()
{
    this->clear();
}

void Tomasulo::clear()
{
#define CLEAR(arr) memset(reinterpret_cast<void *>(arr), 0, sizeof(arr))
    CLEAR(this->instruction_memory);
    CLEAR(this->registers);
    CLEAR(this->register_status);
    CLEAR(this->_reservation_station);
}

void Tomasulo::load_instructions(std::istream& input_file)
{
    std::string  line;
    mips_word_t *instruction_memory_ptr = reinterpret_cast<mips_word_t *>(this->instruction_memory);

    this->clear();
    while (std::getline(input_file, line))
    {
        if (!line.empty() && line.front() != '#')
        {
            *instruction_memory_ptr++ = str_to_instruction(line);
        }
    }
    *instruction_memory_ptr = RISCV_EOI;
}

void Tomasulo::start(bool step_by_step)
{
    bool execution_ended;

    this->clock_cycle = 1;
    execution_ended   = false;
    this->initialize_instruction_queue();
    do {
        // execute instructions in instruction queue
        if (!this->instructions_queue.empty()) {
            this->issue();
        } else {
            execution_ended = true;
        }
        // write back
    } while (!execution_ended);
}

void Tomasulo::initialize_instruction_queue()
{
    mips_word_t instruction;

    this->pc    = 0;
    while ((instruction = _read_word(this->instruction_memory, this->pc)) != RISCV_EOI)
    {
        this->instructions_queue.push_back(instruction);
        this->pc += 4;
    }
}

void Tomasulo::issue()
{
    mips_word_t     instruction = this->instructions_queue.front();
    InstructionType i_type      = instruction_type(instruction);
    unsigned        r           = this->get_rs_num_for_instruction_type(i_type);

    if (r)
    {
        mips_word_t rs, rt, rd;

        rs = instruction_rs1(instruction);
        rt = instruction_rs2(instruction);
        rd = instruction_rd(instruction);
        switch (i_type)
        {
            case InstructionType::ADD:
            case InstructionType::SUB:
            case InstructionType::MULT:
            case InstructionType::DIV:

                if (this->register_status[rs])
                {
                    this->reservation_station(r).qj = this->register_status[rs];
                }
                else
                {
                    this->reservation_station(r).vj = this->registers[rs];
                    this->reservation_station(r).qj = 0;
                }

                if (this->register_status[rt])
                {
                    this->reservation_station(r).qk = this->register_status[rt];
                }
                else
                {
                    this->reservation_station(r).vk = this->registers[rt];
                    this->reservation_station(r).qk = 0;
                }
                this->register_status[rd] = r;
                break;
            case InstructionType::LW:
            case InstructionType::SW:
                if (this->register_status[rs])
                {
                    this->reservation_station(r).qj = this->register_status[rs];
                }
                else
                {
                    this->reservation_station(r).vj = this->registers[rs];
                    this->reservation_station(r).qj = 0;
                }
                // TODO(igolt): puta que pariu immediate é de fuder
                // this->reservation_station(r).a = immediate

                if (i_type == InstructionType::LW)
                {
                    this->register_status[rt] = r;
                }
                else
                {
                    if (this->register_status[rt])
                    {
                        this->reservation_station(r).qk = this->register_status[rt];
                    }
                    else
                    {
                        this->reservation_station(r).vk = this->registers[rt];
                        this->reservation_station(r).qk = 0;
                    }
                }
                break;
        }
        this->reservation_station(r).busy = true;
        this->reservation_station(r).operation = i_type;
        this->instructions_queue.pop_front();
    }
}

unsigned Tomasulo::get_rs_num_for_instruction_type(InstructionType i_type)
{
    unsigned lower, upper;

    switch (i_type)
    {
        case InstructionType::LW:
        case InstructionType::SW:
            lower = 0, upper = 2;
            break;
        case InstructionType::ADD:
        case InstructionType::SUB:
            lower = 2, upper = 5;
            break;
        case InstructionType::DIV:
        case InstructionType::MULT:
            lower = 5, upper = 7;
            break;
        default:
            throw std::invalid_argument(__FUNCTION__);
    }

    for (unsigned i = lower; i < upper; ++i)
    {
        if (!this->_reservation_station[i].busy)
            return i + 1;
    }
    return 0;
}

ReservationStation& Tomasulo::reservation_station(unsigned r)
{
    return this->_reservation_station[r - 1];
}
