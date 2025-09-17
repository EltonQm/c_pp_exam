#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "task_utils.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    // Set up CLI parser
    argparse::ArgumentParser program("GOoboparser");
    program.add_description(
        "Elton's GOoboParser - GOobo file processor\n"
        "Author: Elton Ugbogu, University of Potsdam, 2025");

    // Declare CLI options
    program.add_argument("--go-search")
        .help("Search Uniprot IDs for a given GO annotation (GO:XXXXXXX)")
        .nargs(argparse::nargs_pattern::at_least_one);

    program.add_argument("--go-count")
        .help("Count how often a GO ID appears in the file (GO:XXXXXXX)")
        .nargs(argparse::nargs_pattern::at_least_one);

    // Parse CLI arguments
    try
    {
        program.parse_args(argc, argv); // May throw exception if args are malformed
    }
    catch (const std::exception &err)
    {
        // If parsing fails, print error message + usage help and exit
        print_command_error(program, args, err.what());
        return 1;
    }

    // Ensure only one mode is used
    if (program.is_used("--go-search") && program.is_used("--go-count"))
    {
        print_command_usage(args, "Choose either --go-search or --go-count (not both)");
        return 1;
    }

    // Determine mode and extract input
    std::vector<std::string> files_and_ids;
    bool mode_search = false, mode_count = false;

    if (program.is_used("--go-search"))
    {
        files_and_ids = program.get<std::vector<std::string>>("--go-search");
        mode_search = true;
    }
    else if (program.is_used("--go-count"))
    {
        files_and_ids = program.get<std::vector<std::string>>("--go-count");
        mode_count = true;
    }

    if (files_and_ids.empty())
    {
        print_command_usage(args, "No input files or GO IDs provided.");
        return 1;
    }

    // Validate inputs
    auto valid_files = validate_files(files_and_ids);
    auto valid_go_ids = validate_go_id(files_and_ids);

    print_invalid_filenames(args, files_and_ids);
    print_invalid_go_id(args, files_and_ids);

    for (const auto &f : valid_files)
        std::cout << "Valid file - " << f << "\n";
    for (const auto &id : valid_go_ids)
        std::cout << "Valid GO ID - " << id << "\n";

    std::cout << "Mode: " << (mode_search ? "go-search" : "go-count") << "\n";

    if (valid_files.empty())
    {
        print_command_usage(args, "No valid input files found.");
        return 1;
    }

    if (mode_search)
        process_search(valid_files, valid_go_ids);
    else if (mode_count)
        process_count(valid_files, valid_go_ids);

    return 0;
}
