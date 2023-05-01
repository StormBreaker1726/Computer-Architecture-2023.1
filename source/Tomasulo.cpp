//
// Created by joao-oliveira on 21/04/23.
//

#include "Tomasulo.hpp"
#include "utils.hpp"

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

Tomasulo::Tomasulo()
{
}

void Tomasulo::start(bool step_by_step)
{
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
    *write_ptr = 0;
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
