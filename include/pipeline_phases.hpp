#ifndef F3482B3C_894C_459F_A2E6_C32AB776125D
#define F3482B3C_894C_459F_A2E6_C32AB776125D

#include "defines.hpp"
#include "mips_types.hpp"

struct WB
{
    mips_word_t RegWrite;
    mips_word_t MemToReg;
};

struct MEM
{
    mips_word_t MemRead;
    mips_word_t MemWrite;
    mips_word_t Branch;
    mips_word_t Jump;
    mips_word_t JumpReg;
};

struct EX
{
    mips_word_t RegDest;
    mips_word_t ALUSrc;
    mips_word_t ALUOp;
};

struct Control
{
    mips_word_t RegDest;
    mips_word_t Jump;
    mips_word_t JumpReg;
    mips_word_t Branch;
    mips_word_t MemRead;
    mips_word_t MemToReg;
    mips_word_t ALUOp;
    mips_word_t MemWrite;
    mips_word_t ALUSrc;
    mips_word_t RegWrite;
};

struct IF_ID
{
    mips_word_t pc_plus_four;
    mips_word_t instruction;
};

struct ID_EX
{
    WB          wb;
    MEM         m;
    EX          ex;
    mips_int_t  address;
    mips_word_t pc_plus_four;
    mips_word_t rt;
    mips_word_t rd;
    mips_word_t read_data_1;
    mips_word_t read_data_2;
};

struct EX_MEM
{
    MEM         m;
    WB          wb;
    mips_word_t jump_reg_address;
    mips_word_t jump_address;
    mips_word_t branch_address;
    mips_word_t alu_result;
    mips_word_t pc_plus_four;
    mips_word_t write_data;
    mips_word_t dest_register;
    bool        is_zero;
};

struct MEM_WB
{
    WB          wb;
    mips_word_t read_data;
    mips_word_t alu_result;
    mips_word_t dest_register;
    mips_word_t pc_plus_four;
};

#endif /* F3482B3C_894C_459F_A2E6_C32AB776125D */
