//
// Created by joao-oliveira on 21/04/23.
//

#ifndef COMPUTER_ARCHITECTURE_2023_1_TOMASULO_H
#define COMPUTER_ARCHITECTURE_2023_1_TOMASULO_H

#include "mips_types.hpp"

class Tomasulo
{
    public:
        Tomasulo(){};

        void start(bool step_by_step = false);
        void clear();
        void load_instruction(std::istream&);

    private:
        void register_dump();
        void memory_dump();

        void instruction_fetch(mips_word_t);
        void instruction_decode(mips_word_t);
        void execution(mips_word_t);
        void mem(mips_word_t);
        void write_back(mips_word_t);
};


#endif //COMPUTER_ARCHITECTURE_2023_1_TOMASULO_H
