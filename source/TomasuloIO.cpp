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

    this->out << "============== REGISTERS ==============\n";
    this->out << "Name\tQi\tValue\n";
    for (unsigned i = 0; i < Tomasulo::FP_REG_OFFSET; i++)
        this->out << registers_name[i] << "\t" << this->register_status[i] << '\t' << this->registers[i] << '\n';
    for (unsigned i = Tomasulo::FP_REG_OFFSET; i < ARRAYSIZE(this->registers); ++i)
        this->out << registers_name[i] << '\t' << this->register_status[i] << '\t' << interpret_word_as_float(this->registers[i]) << '\n';
    this->out << "=======================================\n";
}

void Tomasulo::memory_dump()
{
    this->out << "=============== MEMORY ===============\n";
    for (mips_word_t i = 0; i < ARRAYSIZE(this->data_memory); i += 4)
        this->out << i << ": " << (mips_int_t) _read_word(this->data_memory, i) << '\n';
    this->out << "=======================================\n";
}

void Tomasulo::print_instruction_queue()
{
    for (auto& instruction : this->instructions_queue) {
        std::bitset<SIZE_IN_BITS(mips_word_t)> bin(instruction);

        this->out << bin << '\n';
    }
}

void Tomasulo::reservation_station_dump()
{
    extern const char *const reservation_station_names[];

    this->out << "====================== RESERVATION STATION ======================\n";
    this->out << "Name\tBusy\tOp\tVj\tVk\tQj\tQk\tAddress\tCycles\n";
    this->out << "-----------------------------------------------------------------\n";
    for (size_t i = 0; i < ARRAYSIZE(this->_reservation_station); ++i) {
        const ReservationStation& rs = this->_reservation_station[i];

        this->out << reservation_station_names[i] << '\t' << rs.busy << '\t' << get_instruction_name(rs.operation) << '\t' << rs.vj << '\t' << rs.vk
                  << '\t' << rs.qj << '\t' << rs.qk << '\t' << rs.a << '\t' << rs.cycles_executing << '\n';
    }
    this->out << "=================================================================\n";
}
