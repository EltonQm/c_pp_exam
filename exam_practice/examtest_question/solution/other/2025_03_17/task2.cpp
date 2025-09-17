#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

void print_usage(const std::string &app)
{
    std::cout << app << " [--help,-h] [INPUTFILE | kg:number cm:number]\n";
}

void print_help(const std::string &app)
{
    print_usage(app);
    std::cout
        << "\nTwo-number mode:\n"
        << "  " << app << " weight_kg height_cm\n"
        << "  Computes BMI = kg / (m^2). Height must be in cm.\n"
        << "File mode (Task 3):\n"
        << "  " << app << " file.tab [output.tab]\n";
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
            return false;
    }
    return true;
}

double compute_bmi(double kg, double height_cm)
{
    double m = height_cm / 100.0;
    if (m <= 0.0)
        return 0.0;
    return kg / (m * m);
}

int main(int argc, char *argv[])
{
    std::string app = (argc > 0) ? argv[0] : "app";

    if (argc == 1)
    {
        print_usage(app);
        return 0;
    }
    if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")
    {
        print_help(app);
        return 0;
    }

    if (argc == 3)
    {
        std::string wstr = argv[1];
        std::string hstr = argv[2];
        if (!is_numeric(wstr) || !is_numeric(hstr))
        {
            std::cerr << "Error: arguments must be numeric.\n";
            print_usage(app);
            return 1;
        }
        double w = std::stod(wstr);
        double h = std::stod(hstr);
        if (w >= h)
        {
            std::cerr << "Error: You weight is larger than your size??\n"
                      << "Please switch arguments or give height in cm!\n";
            return 1;
        }
        double bmi = compute_bmi(w, h);
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "Weight: " << w << "kg, Height: " << h << "cm, BMI: " << bmi << "\n";
        return 0;
    }

    // If not the two-number mode, just accept (Task 3 adds real file processing)
    if (argc == 2)
    {
        // Could be a filename; Task 3 will implement reading
        return 0;
    }

    std::cerr << "Error: Too many arguments.\n";
    print_usage(app);
    return 1;
}
