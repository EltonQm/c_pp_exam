// task2.cpp
#include "task2_utils.hpp"
#include <argparse/argparse.hpp>
#include <iostream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <zlib.h>
#include <cstring>

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("task2");

    program.add_argument("option")
        .help("Choose --consider-table or --replaced-by");
    program.add_argument("filename")
        .help("Input GO .obo[.gz] file");
    program.add_argument("namespace")
        .default_value(std::string(""))
        .help("Optional namespace filter (e.g., molecular_function)");

    try
    {
        program.parse_args(argc, argv);
        std::string opt = program.get("option");
        std::string file = program.get("filename");
        std::string ns = program.get("namespace");

        auto lines = read_obo_lines(file);

        if (opt == "--consider-table")
        {
            auto result = considerTable(lines, ns);
            for (const auto &row : result)
                std::cout << row[0] << "\t" << row[1] << "\t" << row[2] << "\n";
        }
        else if (opt == "--replaced-by")
        {
            auto result = replacedByTable(lines, ns);
            for (const auto &row : result)
                std::cout << row[0] << "\t" << row[1] << "\t" << row[2] << "\n";
        }
        else
        {
            std::cerr << "Invalid option: " << opt << "\n";
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
