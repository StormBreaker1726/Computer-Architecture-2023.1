//
// Created by joao-oliveira on 21/04/23.
//

#include "Tomasulo.hpp"
#include "utils.hpp"
#include <string>

#define RISCV_EOI 0

#define R_OP_CODE 0x53
#define S_OP_CODE 0x27
#define I_OP_CODE 0x07

static inline mips_word_t _read_word(mips_byte_t mem[], mips_word_t pos)
{
    return *((mips_word_t *) (mem + pos));
}

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

static inline mips_word_t rd(mips_word_t instruction)
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
    this->load_buffer.reserve(5);
    this->store_buffer.reserve(5);
    this->rs_add.reserve(3);
    this->rs_mult.reserve(2);
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
#define MEMSET(arr, value) memset((void *) (arr), value, sizeof(arr))
    MEMSET(this->instruction_memory, 0);
    MEMSET(this->registers, 0);
}

void Tomasulo::load_instruction(std::istream& input_file)
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

void Tomasulo::issue()
{
    mips_word_t instruction = this->instructions_queue.front();
    mips_word_t i_type = instruction_type(instruction);

    std::cerr << "Issue begin\n";
    this->instructions_queue.pop_front();
    switch (i_type) {
        case ADD:
        case SUB:
        {
            std::cout << __LINE__ << '\n';
            std::string vj = this->register_status[rs1(instruction)];
            std::cout << __LINE__ << '\n';
            std::string vk = this->register_status[rs2(instruction)];
            std::string qj = "";
            std::string qk = "";

            if (vj != "")
            {
                std::swap(vj, qj);
            }
            if (vk != "")
            {
                std::swap(vk, qk);
            }

            std::cout << __LINE__ << '\n';
            mips_word_t rd_value = rd(instruction);
            std::cout << "rd: " << rd_value << '\n';
            this->register_status[rd(instruction)] = "Add" + std::to_string(this->rs_add.size() + 1);
            std::cout << __LINE__ << '\n';
            this->rs_add.push_back({ true, i_type, vj, vk, qj, qk });
            break;
        }
        case DIV:
        case MULT:
        {
            std::string vj = this->register_status[rs1(instruction)];
            std::string vk = this->register_status[rs2(instruction)];
            std::string qj = "";
            std::string qk = "";

            if (vj != "")
            {
                std::swap(vj, qj);
            }
            if (vk != "")
            {
                std::swap(vk, qk);
            }

            this->register_status[rd(instruction)] = "Mult" + std::to_string(this->rs_mult.size() + 1);
            this->rs_mult.push_back({ true, i_type, vj, vk, qj, qk });
            break;
        }
        case SW:
        {
            std::string vj = this->register_status[rs1(instruction)];
            std::string vk = this->register_status[rs2(instruction)];
            std::string qj = "";
            std::string qk = "";

            if (vj != "")
            {
                std::swap(vj, qj);
            }
            if (vk != "")
            {
                std::swap(vk, qk);
            }

            this->store_buffer.push_back({ true, i_type, vj, vk, qj, qk });
            break;
        }
        case LW:
        {
            std::string vj = this->register_status[rs1(instruction)];
            std::string vk = this->register_status[rs2(instruction)];
            std::string qj = "";
            std::string qk = "";

            if (vj != "")
            {
                std::swap(vj, qj);
            }
            if (vk != "")
            {
                std::swap(vk, qk);
            }

            this->load_buffer.push_back({ true, i_type, vj, vk, qj, qk });
            break;
        }
        default:
            throw std::invalid_argument(__FUNCTION__);
    }
    std::cerr << "Issue end\n";
}

void Tomasulo::instruction_fetch(mips_word_t instruction)
{
    this->_if_id.instruction  = instruction;
    this->_if_id.pc_plus_four = this->pc;
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
