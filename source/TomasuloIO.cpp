#include "utils.hpp"

#include "Tomasulo.hpp"
#include "Tomasulo/instructions.hpp"

#include <stdexcept>

static const char *const registers_name[] = {
    "x0",  "x1",  "x2",  "x3",  "x4",  "x5",  "x6",  "x7",  "x8",  "x9",  "x10", "x11", "x12", "x13", "x14", "x15",
    "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31",
    "f0",  "f1",  "f2",  "f3",  "f4",  "f5",  "f6",  "f7",  "f8",  "f9",  "f10", "f11", "f12", "f13", "f14", "f15",
    "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31",
};

static const char *operation_name(mips_word_t op_num)
{
    switch (op_num) {
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case MULT:
            return "MULT";
        case DIV:
            return "DIV";
        case SW:
            return "SW";
        case LW:
            return "LW";
        default:
            throw std::invalid_argument(__FUNCTION__);
    }
}

static void print_rs_line(const RSLine& line)
{
    std::cout << "\t" << line.busy << "\t" << operation_name(line.operation) << "\t" << line.vj << "\t" << line.vk << "\t" << line.qj << "\t"
              << line.qk << "\n";
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

void Tomasulo::print_instruction_queue()
{
    for (auto& instruction : this->instructions_queue) {
        std::bitset<SIZE_IN_BITS(mips_word_t)> bin(instruction);

        std::cout << bin << '\n';
    }
}

void Tomasulo::print_reservation_stations()
{
    size_t rs_num = 1;
    std::cout << "Name\tBusy\tOp\tVj\tVk\tQj\tQk\n";
    for (const auto& line : this->rs_add) {
        std::cout << "Add" << rs_num++;
        print_rs_line(line);
    }

    rs_num = 1;
    for (const auto& line: this->rs_mult) {
        std::cout << "Mult" << rs_num++;
        print_rs_line(line);
    }

    rs_num = 1;
    for (const auto& line: this->load_buffer) {
        std::cout << "Mult" << rs_num++;
        print_rs_line(line);
    }

    rs_num = 1;
    for (const auto& line: this->store_buffer) {
        std::cout << "Store" << rs_num++;
        print_rs_line(line);
    }
}
