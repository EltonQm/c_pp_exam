// task1.cpp
#include "task_utils.hpp"
#include <argparse/argparse.hpp>
#include <iostream>
#include <filesystem>
#include <unordered_set>

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("task1");

    program.add_description("GO Term Extractor: Prints GO ID and Name from .obo file");

    program.add_argument("--go-id-name")
        .default_value(false)
        .implicit_value(true)
        .help("Extract and print GO ID and name from the .obo file");

    program.add_argument("file")
        .help("Path to .obo or .obo.gz file");

    program.add_argument("namespace")
        .default_value(std::string(""))
        .nargs(0, 1)
        .help("Optional namespace filter: molecular_function | biological_process | cellular_component");

    try
    {
        program.parse_args(argc, argv);

        if (!program.get<bool>("--go-id-name"))
        {
            std::cerr << "Error: Missing required option --go-id-name\n\n";
            std::cerr << program;
            return 1;
        }

        std::string filepath = program.get<std::string>("file");

        // Check file validity
        if (!std::filesystem::exists(filepath))
        {
            std::cerr << "Error: File does not exist: " << filepath << "\n";
            return 1;
        }

        std::string ns = program.get<std::string>("namespace");
        if (!ns.empty())
        {
            static const std::unordered_set<std::string> valid_ns = {
                "molecular_function", "biological_process", "cellular_component"};
            if (!valid_ns.count(ns))
            {
                std::cerr << "Invalid namespace: " << ns << "\n";
                return 1;
            }
        }

        // Process file
        const auto lines = read_obo_lines(filepath);
        print_go_id_and_name(lines); // (optionally extend this to use ns if required)
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
