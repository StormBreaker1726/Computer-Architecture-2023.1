//
// Created by joao-oliveira on 21/04/23.
//

#include "Tomasulo.hpp"
#include "defines.hpp"
#include "menu.hpp"
#include <fstream>

int main()
{
    Tomasulo t;

    std::ifstream input_file("./inputs/compiled/1");

    if (!input_file.is_open()) {
        std::cerr << "Falha ao arquivo de entrada\n";
        return 1;
    }

    t.load_instructions(input_file);
    t.start();
    return 0;
}
