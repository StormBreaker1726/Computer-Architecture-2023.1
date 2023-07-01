//
// Created by joao-oliveira on 21/04/23.
//

#include "utils.hpp"

#include "Tomasulo.hpp"
#include "Tomasulo/instructions.hpp"

#include <string>

#define RISCV_EOI 0

#define R_OP_CODE 0x53
#define S_OP_CODE 0x27
#define I_OP_CODE 0x07

static inline mips_word_t opcode(mips_word_t instruction)
{
    return instruction & 0x7F;
}

static inline mips_word_t funct7(mips_word_t instruction)
{
    return (instruction >> 25) & 0x7F;
}

static inline mips_word_t rs1(mips_word_t instruction)
{
    /* rs1: [19-15] */
    return (instruction >> 15) & 0x1F;
}

static inline mips_word_t rs2(mips_word_t instruction)
{
    /* rs2: [24-20] */
    return (instruction >> 20) & 0x1F;
}

static inline mips_word_t instruction_rd(mips_word_t instruction)
{
    /* rd: [11-7] */
    return (instruction >> 7) & 0x1F;
}

static inline mips_word_t imm_i(mips_word_t instruction)
{
    /* Pro tipo I: IMM: [31-20] */
    return (instruction >> 20) & 0xFFF;
}

static inline mips_word_t type_from_funct(mips_word_t instruction)
{
    switch (funct7(instruction)) {
        case 0x1: return ADD;
        case 0x5: return SUB;
        case 0x9: return MULT;
        case 0xD: return DIV;
    }
    throw std::invalid_argument(__FUNCTION__);
}

static inline mips_word_t imm_s(mips_word_t instruction)
{
    /* Pro tipo S IMM: [31-25]*/
    return (instruction >> 25) & 0x3F;
}

static int instruction_type(mips_word_t instruction)
{
    switch (opcode(instruction)) {
        case S_OP_CODE: return SW;
        case I_OP_CODE: return LW;
        case R_OP_CODE: return type_from_funct(instruction);
    }
    throw std::invalid_argument(__FUNCTION__);
}

Tomasulo::Tomasulo()
{
}

void Tomasulo::start(bool step_by_step)
{
    mips_word_t instruction;

    this->clock = 1;
    this->pc = 0;

    while ((instruction = _read_word(this->instruction_memory, this->pc)) != RISCV_EOI) {
        this->instructions_queue.push_back(instruction);
        this->pc += 4;
    }
    this->print_instruction_queue();

    while (!instructions_queue.empty()) {
        this->issue();
    }
    this->print_reservation_stations();
}

void Tomasulo::clear()
{
#define CLEAR(arr) memset((void *) (arr), 0, sizeof(arr))
    CLEAR(this->instruction_memory);
    CLEAR(this->registers);
    CLEAR(this->register_status);
}

void Tomasulo::load_instructions(std::istream& input_file)
{
    std::string line;
    mips_word_t *write_ptr = (mips_word_t *) (this->instruction_memory);

    this->clear();
    while (std::getline(input_file, line))
    {
        if (!line.empty() && line.front() != '#')
        {
            *write_ptr++ = std::stoul(line, nullptr, 2);
        }
    }
    *write_ptr = RISCV_EOI;
}

unsigned Tomasulo::get_rs_for_instruction_type(mips_word_t i_type)
{
    unsigned lower, upper;

    switch (i_type)
    {
        case LW:
        case SW:
            lower = 0, upper = 2;
            break;
        case ADD:
        case SUB:
            lower = 2, upper = 5;
            break;
        case DIV:
        case MULT:
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

void Tomasulo::issue()
{
    mips_word_t instruction = this->instructions_queue.front();
    mips_word_t i_type      = instruction_type(instruction);
    unsigned    r           = this->get_rs_for_instruction_type(i_type);

    if (r) {
        mips_word_t rs, rt, rd;

        switch (i_type) {
            case ADD:
            case SUB:
            case MULT:
            case DIV:
                rs = rs1(instruction);
                rt = rs2(instruction);
                rd = instruction_rd(instruction);

                if (this->register_status[rs]) {
                    this->reservation_station(r).qj = this->register_status[rs];
                } else {
                    this->reservation_station(r).vj = this->registers[rs];
                    this->reservation_station(r).qj = 0;
                }

                if (this->register_status[rt]) {
                    this->reservation_station(r).qk = this->register_status[rt];
                } else {
                    this->reservation_station(r).vk = this->registers[rt];
                    this->reservation_station(r).qk = 0;
                }
                this->register_status[rd] = r;
                break;
            case LW:
            case SW:
                if (this->register_status[rs]) {
                    this->reservation_station(r).qj = this->register_status[rs];
                } else {
                    this->reservation_station(r).vj = this->registers[rs];
                    this->reservation_station(r).qj = 0;
                }
                // this->reservation_station(r).a = immediate

                if (i_type == LW) {
                    this->register_status[rt] = r;
                } else {
                    if (this->register_status[rt]) {
                        this->reservation_station(r).qk = this->register_status[rt];
                    } else {
                        this->reservation_station(r).vk = this->registers[rt];
                        this->reservation_station(r).qk = 0;
                    }
                }
                break;
        }
        this->reservation_station(r).busy = true;
        this->instructions_queue.pop_front();
    }
}

void Tomasulo::instruction_fetch(mips_word_t instruction)
{
}

void Tomasulo::instruction_decode(mips_word_t instruction)
{
}

void Tomasulo::execution(mips_word_t instruction)
{
}

void Tomasulo::mem(mips_word_t instruction)
{
}

void Tomasulo::write_back(mips_word_t instruction)
{
}
