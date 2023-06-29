#include "menu.hpp"
#include "utils.hpp"

static void not_yet_implemented(Tomasulo& t)
{
    std::cout << "Not yet implemented\n";
}

static inline std::ostream& print_error(const char *s, std::ostream& out = std::cout)
{
    return out << "******* " << s << " *******\n";
}

void menu(Tomasulo& tomasulo)
{
#define EXIT_OPTION         0UL

#define OPTION_IS_VALID(op) ((op) <= ARRAYSIZE(options))

    struct
    {
        const char *title;
        void (*exec)(Tomasulo&);
    } options[] = {
        { "Start execution", not_yet_implemented },
        { "Read instructions from file", not_yet_implemented },
        { "Type instructions", not_yet_implemented },
        { "Reset (clean memory and registers)", not_yet_implemented },
    };

    while (true)
    {
        std::cout << "*********************************************\n"
                  << "**************** MIPS Processor *************\n"
                  << "*********************************************\n\n";

        size_t i = 1;
        for (const auto& option : options)
            std::cout << (i++) << " - " << option.title << '\n';
        std::cout << EXIT_OPTION << " - Exit\n\n";
        std::cout << "R: ";

        if (std::cin >> i)
        {
            if (!OPTION_IS_VALID(i))
                print_error("Invalid option!");
            else if (i == EXIT_OPTION)
                break;
            else
                options[i - 1].exec(tomasulo);
        }
        else
        {
            if (std::cin.eof())
            {
                std::cout << "\nEnd of file\n";
                break;
            }
            else if (std::cin.bad())
            {
                std::cout << "\nInput error\n";
                break;
            }

            print_error("Invalid input!");
            std::cin.clear();
            clear_istream(std::cin);
        }
    }
}
