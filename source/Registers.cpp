#include "Registers.hpp"

void print_wb(std::ostream& out, const std::string& prefix, const WB& wb)
{
    out << prefix << "RegWrite: " << wb.RegWrite << '\n';
    out << prefix << "MemToReg: " << wb.MemToReg << '\n';
}

void print_mem(std::ostream& out, const std::string& prefix, const MEM& mem)
{
    out << prefix << "MemRead:  " << mem.MemRead << '\n';
    out << prefix << "MemWrite: " << mem.MemWrite << '\n';
    out << prefix << "Branch:   " << mem.Branch << '\n';
    out << prefix << "Jump:     " << mem.Jump << '\n';
    out << prefix << "JumpReg:  " << mem.JumpReg << '\n';
}

void print_ex(std::ostream& out, const std::string& prefix, const EX& ex)
{
    out << prefix << "RegDst: " << ex.RegDest << '\n';
    out << prefix << "ALUSrc: " << ex.ALUSrc << '\n';
    out << prefix << "ALUOp:  " << ex.ALUOp << '\n';
}
