// task1.cpp
#include "task2_utils.hpp"
#include <argparse/argparse.hpp>
#include <iostream>
#include <filesystem>

bool is_valid_namespace(const std::string &ns)
{
    return ns == "molecular_function" || ns == "cellular_component" || ns == "biological_process";
}

bool is_valid_option(const std::string &opt)
{
    return opt == "--consider-table" || opt == "--obsolete-stats" || opt == "--replaced-by" || opt == "--hierarchical_relationship";
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("goobo_parser");
    program.add_argument("option")
        .help("Allowed: --consider-table, --obsolete-stats, --replaced-by, --hierarchical_relationship");
    program.add_argument("filename")
        .help("GO OBO file (.obo or .obo.gz)");
    program.add_argument("namespace")
        .default_value(std::string(""))
        .help("Optional namespace: molecular_function, cellular_component, biological_process");

    try
    {
        program.parse_args(argc, argv);
        std::string opt = program.get("option");
        std::string file = program.get("filename");
        std::string ns = program.get("namespace");

        if (!is_valid_option(opt))
        {
            std::cerr << "Invalid option: " << opt << "\n";
            std::cerr << program;
            return 1;
        }

        if (!std::filesystem::exists(file))
        {
            std::cerr << "File not found: " << file << "\n";
            return 1;
        }

        if (!ns.empty() && !is_valid_namespace(ns))
        {
            std::cerr << "Invalid namespace: " << ns << "\n";
            return 1;
        }

        std::cout << "Arguments valid. Proceeding...\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n"
                  << program;
        return 1;
    }

    return 0;
}
