#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "task2_utils.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    argparse::ArgumentParser program("GOoboparser");
    program.add_description("Elton's GOoboParser - GOobo file processor\nAuthor: Elton Ugbogu, University of Potsdam, 2025");

    program.add_argument("--consider-table")
        .help("Search for obsolete GO ids")
        .nargs(argparse::nargs_pattern::at_least_one);

    program.add_argument("--obsolete-stats")
        .help("Alias for --consider-table")
        .nargs(argparse::nargs_pattern::at_least_one);

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        print_command_error(program, args, err.what());
        return 1;
    }

    std::vector<std::string> files_and_ns;
    bool mode_consider = false;

    if (program.is_used("--consider-table"))
    {
        files_and_ns = program.get<std::vector<std::string>>("--consider-table");
        mode_consider = true;
    }
    else if (program.is_used("--obsolete-stats"))
    {
        files_and_ns = program.get<std::vector<std::string>>("--obsolete-stats");
        mode_consider = false;
    }
    else
    {
        usage(program);
        return 0;
    }

    if (files_and_ns.empty())
    {
        print_command_error(program, args, "No input files provided.");
        return 1;
    }

    auto valid_files = validate_files(program, files_and_ns);
    auto valid_namespaces = validate_namespaces(program, files_and_ns);

    if (mode_consider)
    {
        auto result = process_consider_table(valid_files, valid_namespaces);
        for (const auto &row : result)
        {
            std::cout << row[0] << "\t" << row[1] << "\n";
        }
    }
    else
    {
        auto result = process_obsolete_stats(valid_files, valid_namespaces);
        for (const auto &row : result)
        {
            std::cout << row[0] << "\t" << row[1] << "\n";
        }
    }

    return 0;
}
