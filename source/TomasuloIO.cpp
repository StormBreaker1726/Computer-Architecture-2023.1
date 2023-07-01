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

static const char *const reservation_station_names[7] = { "Load1", "Load2", "Add1", "Add2", "Add3", "Mult1", "Mult2" };

static const char *get_instruction_name(InstructionType type)
{
    switch (type) {
        case InstructionType::ADD:
            return "ADD";
        case InstructionType::SUB:
            return "SUB";
        case InstructionType::MULT:
            return "MULT";
        case InstructionType::DIV:
            return "DIV";
        case InstructionType::SW:
            return "SW";
        case InstructionType::LW:
            return "LW";
    }
    return "";
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
    extern const char *const reservation_station_names[];

    std::cout << "====================== RESERVATION STATION ======================\n";
    std::cout << "Name\tBusy\tOp\tVj\tVk\tQj\tQk\tAddress\n";
    std::cout << "-----------------------------------------------------------------\n";
    for (size_t i = 0; i < ARRAYSIZE(this->_reservation_station); ++i) {
        const ReservationStation& rs = this->_reservation_station[i];

        std::cout << reservation_station_names[i] << '\t' << rs.busy << '\t' << get_instruction_name(rs.operation) << '\t' << rs.vj << '\t' << rs.vk
                  << '\t' << rs.qj << '\t' << rs.qk << '\t' << rs.a << '\n';
    }
    std::cout << "=================================================================\n";
}
