//
// Created by joao-oliveira on 21/04/23.
//

#include "Tomasulo.hpp"
#include "utils.hpp"

#define RISCV_EOI 0

#define R_OP_CODE 0x53
#define S_OP_CODE 0x27
#define I_OP_CODE 0x07

#define ADD       1
#define SUB       2
#define MULT      3
#define SW        4
#define LW        5

/*
 * NOTE(igolt): Documentação me deixou Confuso com o nome certo dos registradores
 *              Vai ser necessário para fazer o dump dos registradores.
*/
static const char *const registers_name[] = {
    "x0",  "x1",  "x2",  "x3",  "x4",  "x5",  "x6",  "x7",  "x8",  "x9",  "x10", "x11", "x12", "x13", "x14", "x15",
    "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31",
    "f0",  "f1",  "f2",  "f3",  "f4",  "f5",  "f6",  "f7",  "f8",  "f9",  "f10", "f11", "f12", "f13", "f14", "f15",
    "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31",
};

static inline mips_word_t _read_word(mips_byte_t mem[], mips_word_t pos)
{
    return *((mips_word_t *) (mem + pos));
}

static inline mips_word_t opcode(mips_word_t instruction)
{
    return instruction & 0x7F;
}

static inline mips_word_t type_from_funct(mips_word_t instruction)
{
    mips_word_t funct = (instruction >> 25) & 0x7F;

    switch (funct) {
        case 0x1: return ADD;
        case 0x5: return SUB;
        case 0x9:
        case 0xD: return MULT;
    }
    throw 0;
}

static int instruction_type(mips_word_t instruction)
{
    switch (opcode(instruction)) {
        case R_OP_CODE:
            return type_from_funct(instruction);
        case S_OP_CODE:
            return SW;
        case I_OP_CODE:
            return LW;
    }
    throw 0;
}

Tomasulo::Tomasulo()
{
    this->load_buffer.reserve(5);
    this->store_buffer.reserve(5);
    this->rs_add.reserve(3);
    this->rs_mult.reserve(2);
    this->register_status.reserve(64);
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
    // TODO(igolt): usar cast do C++ ao invés de C-style cast

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

void Tomasulo::register_dump()
{
    extern const char *const registers_name[];

    std::cout << "============== REGISTERS ==============\n";
    for (mips_word_t i = 0; i < ARRAYSIZE(this->registers); i++)
        std::cout << "* " << registers_name[i] << ": " << (mips_int_t) this->registers[i] << '\n';
    std::cout << "=======================================\n";
}

void Tomasulo::memory_dump()
{
    std::cout << "=============== MEMORY ===============\n";
    for (mips_word_t i = 0; i < ARRAYSIZE(this->data_memory); i += 4)
        std::cerr << i << ": " << (mips_int_t) _read_word(this->data_memory, i) << '\n';
    std::cout << "=======================================\n";
}

void Tomasulo::issue()
{
    mips_word_t instruction = this->instructions_queue.front();

    this->instructions_queue.pop_front();
    /* TODO: Adicionar as instruções de acordo com o tipo de instrução */
    switch (instruction_type(instruction)) {
        case ADD:
            break;
        case MULT:
            break;
        case SW:
            break;
        case LW:
            break;
    }
}

void Tomasulo::print_instruction_queue()
{
    for (auto& instruction : this->instructions_queue) {
        std::bitset<sizeof(mips_word_t) << 3> bin(instruction);

        std::cout << bin << '\n';
    }
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
