// GooboParser1.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "argparse/argparse.hpp"

namespace fs = std::filesystem;

void print_help()
{
    std::cout << "Usage:\n"
              << "  ./GooboParser --get-metacyc ID file.obo[.gz]\n"
              << "  ./GooboParser --tab-metacyc file.obo[.gz]\n";
}

bool valid_metacyc_id(const std::string &id)
{
    return id.find("RXN") != std::string::npos || id.find("PWY") != std::string::npos;
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("GooboParser");

    program.add_argument("--get-metacyc")
        .help("Get GO term(s) matching MetaCyc ID (e.g. RXN or PWY)")
        .nargs(1);

    program.add_argument("--tab-metacyc")
        .help("Output table mapping GO, EC and RXN")
        .nargs(0);

    program.add_argument("file")
        .help("Input .obo or .obo.gz file");

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        std::cerr << err.what() << "\n";
        print_help();
        return 1;
    }

    std::string obo_file = program.get<std::string>("file");

    if (!fs::exists(obo_file))
    {
        std::cerr << "Error: File does not exist: " << obo_file << "\n";
        return 1;
    }

    if (program.is_used("--get-metacyc"))
    {
        auto id = program.get<std::vector<std::string>>("--get-metacyc")[0];
        if (!valid_metacyc_id(id))
        {
            std::cerr << "Error: MetaCyc ID must contain RXN or PWY.\n";
            return 1;
        }
        std::cout << "Searching for MetaCyc ID: " << id << "\n";
        // Call processing logic (Task 2)
    }

    if (program.is_used("--tab-metacyc"))
    {
        std::cout << "Generating MetaCyc EC-GO table...\n";
        // Call processing logic (Task 3)
    }

    return 0;
}
