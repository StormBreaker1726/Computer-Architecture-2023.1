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

static void write_value_to_mem(Tomasulo& t)
{
    mips_word_t  address;
    mips_float_t value;

    std::cout << "Type memory address (0-508) and value: ";
    std::cin >> address >> value;

    if (t.memory_address_is_valid(address))
    {
        t.write_fp_to_memory(value, address);
    }
    else
    {
        print_error("Invalid memory address");
    }
}

static void write_value_to_f_reg(Tomasulo& t)
{
    mips_word_t  f;
    mips_float_t value;

    std::cout << "Type the number of the fp register and the value: ";
    std::cin >> f >> value;

    if (f < 32)
    {
        t.write_to_f_register(f, value);
    }
    else
    {
        print_error("Invalid register number");
    }
}

static void write_value_to_x_reg(Tomasulo& t)
{
    mips_word_t x;
    mips_word_t value;

    std::cout << "Type the number of the x register and the value: ";
    std::cin >> x >> value;

    if (x < 32)
    {
        t.write_to_x_register(x, value);
    }
    else
    {
        print_error("Invalid register number");
    }
}

static void print_registers_content(Tomasulo& t)
{
    t.register_dump();
}

static void print_memory_content(Tomasulo& t)
{
    t.memory_dump();
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
        { "Start execution"                         , start_execution             },
        { "Read instructions from file"             , read_instructions_from_file },
        { "Type instructions"                       , type_instructions           },
        { "Print register content"                  , print_registers_content     },
        { "Print memory content"                    , print_memory_content        },
        { "Write content to floating point register", write_value_to_f_reg        },
        { "Write content to integer register"       , write_value_to_x_reg        },
        { "Write content to memory"                 , write_value_to_mem          },
        { "Reset (clean memory and registers)"      , reset                       },
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
