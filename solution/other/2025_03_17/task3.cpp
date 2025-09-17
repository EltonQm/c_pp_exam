#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <optional>

class Bmi
{
public:
    // Compute BMI given weight kg and height cm
    static double compute(double kg, double height_cm)
    {
        double m = height_cm / 100.0;
        return (m > 0.0) ? kg / (m * m) : 0.0;
    }

    // Parse one line "weight<TAB>height" (returns std::nullopt on failure)
    static std::optional<std::pair<double, double>>
    parse_line(const std::string &line)
    {
        if (line.empty())
            return std::nullopt;
        std::string::size_type tab = line.find('\t');
        if (tab == std::string::npos)
            return std::nullopt;
        std::string w = line.substr(0, tab);
        std::string h = line.substr(tab + 1);

        auto trim = [](std::string &s)
        {
            size_t a = s.find_first_not_of(" \t\r\n");
            size_t b = s.find_last_not_of(" \t\r\n");
            if (a == std::string::npos)
            {
                s.clear();
                return;
            }
            s = s.substr(a, b - a + 1);
        };
        trim(w);
        trim(h);

        if (!is_numeric(w) || !is_numeric(h))
            return std::nullopt;
        return std::make_pair(std::stod(w), std::stod(h));
    }

    // Process a file: returns vector of tuples (weight, height, bmi)
    static std::vector<std::tuple<double, double, double>>
    process_file(const std::string &filename)
    {
        std::ifstream in(filename);
        if (!in)
            throw std::runtime_error("Cannot open file: " + filename);
        std::vector<std::tuple<double, double, double>> rows;
        std::string line;
        while (std::getline(in, line))
        {
            auto parsed = parse_line(line);
            if (!parsed)
                continue; // skip malformed lines silently (could warn)
            double w = parsed->first;
            double h = parsed->second;
            double bmi = compute(w, h);
            rows.emplace_back(w, h, bmi);
        }
        return rows;
    }

    // Numeric check (allows one dot)
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

private:
    static bool is_numeric(char) = delete; // prevent accidental overload
};

// Helper wrappers (avoid qualifying everywhere)
static inline bool is_numeric(const std::string &s) { return Bmi::is_numeric(s); }

void print_usage(const std::string &app)
{
    std::cout << app << " [--help,-h] [INPUTFILE [OUTFILE] | kg:number cm:number]\n";
}

void print_help(const std::string &app)
{
    print_usage(app);
    std::cout
        << "\nModes:\n"
        << "  " << app << " weight_kg height_cm\n"
        << "      Compute single BMI. Height must be greater than weight to detect reversal.\n"
        << "  " << app << " input.tab [output.tab]\n"
        << "      Each line: weight<TAB>height  (floats). Outputs weight height bmi.\n"
        << "      If OUTFILE provided, also writes that table (tab separated).\n"
        << "\nExamples:\n"
        << "  " << app << " 76 180\n"
        << "  " << app << " students.tab\n"
        << "  " << app << " students.tab results.tab\n";
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

    // Two-number mode
    if (argc == 3 && is_numeric(argv[1]) && is_numeric(argv[2]))
    {
        double w = std::stod(argv[1]);
        double h = std::stod(argv[2]);
        if (w >= h)
        {
            std::cerr << "Error: You weight is larger than your size??\n"
                      << "Please switch arguments or give height in cm!\n";
            return 1;
        }
        double bmi = Bmi::compute(w, h);
        std::cout << std::fixed << std::setprecision(1)
                  << "Weight: " << w << "kg, Height: " << h << "cm, BMI: " << bmi << "\n";
        return 0;
    }

    // File mode: input + optional output
    if (argc == 2 || argc == 3)
    {
        std::string infile = argv[1];

        // If second argument is numeric we already handled it above (so here it is OUTFILE)
        std::string outfile;
        if (argc == 3)
        {
            outfile = argv[2];
        }

        try
        {
            auto rows = Bmi::process_file(infile);
            std::ios old_state(nullptr);
            old_state.copyfmt(std::cout);
            std::cout << std::fixed << std::setprecision(1);
            for (auto &t : rows)
            {
                double w, h, b;
                std::tie(w, h, b) = t;
                std::cout << w << "\t" << h << "\t" << b << "\n";
            }
            std::cout.copyfmt(old_state);

            if (!outfile.empty())
            {
                std::ofstream out(outfile);
                if (!out)
                {
                    std::cerr << "Warning: Could not open output file: " << outfile << "\n";
                }
                else
                {
                    out << std::fixed << std::setprecision(1);
                    for (auto &t : rows)
                    {
                        double w, h, b;
                        std::tie(w, h, b) = t;
                        out << w << "\t" << h << "\t" << b << "\n";
                    }
                }
            }
        }
        catch (const std::exception &ex)
        {
            std::cerr << "Error: " << ex.what() << "\n";
            return 1;
        }
        return 0;
    }

    std::cerr << "Error: Invalid argument combination.\n";
    print_usage(app);
    return 1;
}
