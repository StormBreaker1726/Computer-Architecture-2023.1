// DCC057 - 2023.1-A - Arquitetura de Computadores
// Trabalho de Implementação: Método de Tomasulo
//
// Integrantes:
// - Carlos Alexandre
// - Igor Correa Rodrigues
// - João Victor Costa de Oliveira

#include "Tomasulo.hpp"
#include "defines.hpp"
#include "menu.hpp"
#include <fstream>

enum ExitError {
    INVALID_ARGC = 1,
    DUMP_FILE = 2
};

int main(int argc, const char *const argv[])
{
    const char *const program_name = argv[0];
    std::ostream *out_stream_ptr = &std::cout;
    std::ofstream outfile;

    if (argc == 2) {
        outfile.open(argv[1]);

        if (!outfile.is_open()) {
            std::cerr << program_name << ": failed opening file: `" << argv[1] << "`\n";
            return ExitError::DUMP_FILE;
        }
        out_stream_ptr = &outfile;
    } else if (argc > 2) {
        std::cerr << "Usage: " << program_name << " [DUMP_FILE]\n";
        return ExitError::INVALID_ARGC;
    }

    Tomasulo t(*out_stream_ptr);
    menu(t);
    return 0;
}
