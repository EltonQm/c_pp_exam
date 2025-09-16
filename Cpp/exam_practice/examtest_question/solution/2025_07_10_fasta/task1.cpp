// ---------------------------------------------------------------
// GOfastaParser CLI Main Program - task1.cpp
// ---------------------------------------------------------------
// Author: Elton Ugbogu, University of Potsdam
// Description: CLI entry point to run --search or --summary modes
// ---------------------------------------------------------------

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
    // Convert raw argv into vector<string> for convenience
    std::vector<std::string> args(argv, argv + argc);

    // Setup the command-line parser
    argparse::ArgumentParser program("GOfastaParser");

    program.add_description(
        "GOfastaParser: Search and summarize FASTA files by UniProt patterns\n"
        "Author: Elton Ugbogu, University of Potsdam, 2025");

    // --search mode: Accepts ≥1 argument (file(s) and optionally UniProt pattern)
    program.add_argument("--search")
        .help("Search sequences in one or more FASTA files, optionally by UniProt ID pattern (e.g., sp|P59637|VEMP_CVHSA)")
        .nargs(argparse::nargs_pattern::at_least_one);

    // --summary mode: Accepts ≥1 argument (at least one FASTA file)
    program.add_argument("--summary")
        .help("Show summary statistics from one or more FASTA files")
        .nargs(argparse::nargs_pattern::at_least_one);

    // Parse CLI input
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        print_command_error(program, args, err.what());
        return 1;
    }

    // ---------------------------------------------------
    // Ensure mutual exclusivity of --search and --summary
    // ---------------------------------------------------
    bool mode_search = program.is_used("--search");
    bool mode_summary = program.is_used("--summary");

    if (mode_search && mode_summary)
    {
        print_command_usage(args, "Error: Please use only one of --search or --summary at a time.");
        return 1;
    }

    if (!mode_search && !mode_summary)
    {
        usage();
        return 1;
    }

    // Collect raw input arguments passed to the selected mode
    std::vector<std::string> files_and_patterns;
    if (mode_search)
    {
        files_and_patterns = program.get<std::vector<std::string>>("--search");
    }
    else if (mode_summary)
    {
        files_and_patterns = program.get<std::vector<std::string>>("--summary");
    }

    if (files_and_patterns.empty())
    {
        print_command_usage(args, "No input files or patterns provided.");
        return 1;
    }

    // Validate input: separate valid FASTA files and valid patterns
    std::vector<std::string> valid_files = validate_files(files_and_patterns);
    std::vector<std::string> valid_patterns = validate_patterns(files_and_patterns);

    // Print feedback for invalid inputs
    print_invalid_filenames(args, files_and_patterns);
    print_invalid_pattern(args, files_and_patterns);

    // Show recognized valid inputs
    for (const auto &f : valid_files)
        std::cout << "Valid file: " << f << "\n";

    for (const auto &p : valid_patterns)
        std::cout << "Valid pattern: " << p << "\n";

    // Sanity check: must have at least one valid file
    if (valid_files.empty())
    {
        print_command_usage(args, "No valid input FASTA files found.");
        return 1;
    }

    // Run appropriate processing mode
    if (mode_search)
    {
        process_search(valid_files, valid_patterns);
    }
    else if (mode_summary)
    {
        process_summary(valid_files, valid_patterns); // valid_patterns can be empty
    }

    return 0;
}
