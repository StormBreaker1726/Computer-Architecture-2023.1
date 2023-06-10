//
// Created by joao-oliveira on 21/04/23.
//

#include "Tomasulo.hpp"
#include "defines.hpp"
#include "menu.hpp"
#include <fstream>

int main(int argc, const char *argv[])
{
    Tomasulo t;

    std::ifstream input_file("./inputs/compiled/1");

    t.load_instruction(input_file);
    t.start();

    return 0;
}
