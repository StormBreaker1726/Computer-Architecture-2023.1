#include "menu.hpp"
#include "utils.hpp"

static inline std::ostream& print_error(const std::string& s, std::ostream& out = std::cout)
{
    return out << "******* " << s << " *******\n";
}

static void start_execution(Tomasulo& t)
{
    std::string input;
    std::cout << "Step by step (y/N)? ";
    std::getline(std::cin, input);

    t.start(input == "y" || input == "Y");
}

static void read_instructions_from_file(Tomasulo& t)
{
    std::string   filepath;
    std::ifstream inputfile;

    std::cout << "Enter intructions file path: ";
    std::getline(std::cin, filepath);

    inputfile.open(filepath);

    if (!inputfile.is_open())
    {
        print_error("Could not open file: `" + filepath + "`");
        return;
    }
    t.load_instructions(inputfile);
}

static void type_instructions(Tomasulo& t)
{
    std::cout << "Press CTRL-D (or any other key combination that represents EOF) when you are done.\n";
    t.load_instructions(std::cin);
    std::cin.clear();
    std::cin.unget();
}

static void reset(Tomasulo& t)
{
    std::cout << "Memory and registers clear\n";
    t.clear();
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
        { "Start execution"                   , start_execution             },
        { "Read instructions from file"       , read_instructions_from_file },
        { "Type instructions"                 , type_instructions           },
        { "Reset (clean memory and registers)", reset                       },
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
            {
                clear_istream(std::cin);
                options[i - 1].exec(tomasulo);
            }
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
