#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

// Trim simple
static inline std::string trim(const std::string &s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos)
        return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

void print_usage(const std::string &app)
{
    std::cout << app << " [--help,-h] [INPUTFILE | kg:number cm:number]\n";
}

void print_help(const std::string &app)
{
    print_usage(app);
    std::cout
        << "\nModes:\n"
        << "  1) " << app << " weight_kg height_cm\n"
        << "       Compute BMI for given numbers.\n"
        << "  2) " << app << " file.tab\n"
        << "       (Task 3) Process a TAB file with lines: weight<TAB>height\n"
        << "\nNotes:\n"
        << "  - Weight and height must be numeric (floats allowed).\n"
        << "  - Height (cm) must be greater than weight (kg) to catch swapped arguments.\n";
}

static bool is_numeric(const std::string &s)
{
    if (s.empty())
        return false;
    bool dot = false;
    for (char c : s)
    {
        if (c == '.')
        {
            if (dot)
                return false;
            dot = true;
        }
        else if (!std::isdigit(static_cast<unsigned char>(c)))
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    std::string app = (argc > 0) ? argv[0] : "app";

    if (argc == 1)
    {
        // No arguments -> show usage line only
        print_usage(app);
        return 0;
    }

    // Help option
    if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")
    {
        print_help(app);
        return 0;
    }

    // Task 1: only validate arguments here (no file reading or BMI calc yet)
    if (argc == 2)
    {
        // Could be a filename OR first number only (invalid).
        // We cannot decide validity of file path (spec doesn’t force existence check for task 1 yet)
        // Just accept & exit for now.
        // Real processing arrives in tasks 2/3.
        return 0;
    }

    if (argc == 3)
    {
        std::string w = argv[1];
        std::string h = argv[2];
        if (!is_numeric(w) || !is_numeric(h))
        {
            std::cerr << "Error: Both arguments must be numeric (floats allowed).\n";
            print_usage(app);
            return 1;
        }
        // Task 2 will implement BMI; here just pass.
        return 0;
    }

    std::cerr << "Error: Too many arguments.\n";
    print_usage(app);
    return 1;
}
