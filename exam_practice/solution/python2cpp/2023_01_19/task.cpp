// task.cpp - Main entry point for GOfastaParser

#include <argparse/argparse.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "task_utils.hpp" // Contains declarations for file/namespace validation and processing

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    argparse::ArgumentParser program("GOfastaParser");

    program.add_description("FASTA-Parser by Elton Ugbogu, University of Potsdam, 2025\nExtract information from FASTA sequence files");

    // Define mutually exclusive modes
    program.add_argument("--get-seq")
        .help("Show the sequence for a given sequence ID")
        .nargs(argparse::nargs_pattern::at_least_two);

    program.add_argument("--get-n")
        .help("Show sequence lengths for all sequence IDs")
        .nargs(argparse::nargs_pattern::at_least_one);

    program.add_argument("--grep-seq")
        .help("Search the sequences for a given amino acid (regex) pattern")
        .nargs(argparse::nargs_pattern::at_least_two);

    // Parse args and handle parsing errors
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        print_command_error(program, args, err.what());
        return 1;
    }

    // Check for mutually exclusive use
    int mode_count = program.is_used("--get-seq") + program.is_used("--get-n") + program.is_used("--grep-seq");
    if (mode_count > 1)
    {
        print_command_usage(args, "Choose only one of --get-seq, --get-n or --grep-seq");
        return 1;
    }

    // No mode specified
    if (mode_count == 0)
    {
        usage();
        return 0;
    }

    std::vector<std::string> raw_inputs;
    if (program.is_used("--get-seq"))
    {
        raw_inputs = program.get<std::vector<std::string>>("--get-seq");
    }
    else if (program.is_used("--get-n"))
    {
        raw_inputs = program.get<std::vector<std::string>>("--get-n");
    }
    else if (program.is_used("--grep-seq"))
    {
        raw_inputs = program.get<std::vector<std::string>>("--grep-seq");
    }

    // Separate files and namespaces from user input
    auto valid_files = validate_files(program, raw_inputs);
    auto valid_namespaces = validate_namespaces(program, raw_inputs);

    // Print diagnostics for invalid input
    print_invalid_filenames(args, raw_inputs);
    print_invalid_namespace(program, args, raw_inputs);

    for (const auto &f : valid_files)
        std::cout << "Valid file - " << f << "\n";
    for (const auto &ns : valid_namespaces)
        std::cout << "Valid namespace - " << ns << "\n";

    // Check if at least one valid file is present
    if (valid_files.empty())
    {
        print_command_usage(args, "No input files provided.");
        return 1;
    }

    // Print selected mode
    if (program.is_used("--get-seq"))
    {
        std::cout << "Mode: get-seq\n";
        process_consider_table(valid_files, valid_namespaces);
    }
    else if (program.is_used("--get-n"))
    {
        std::cout << "Mode: get-n\n";
        process_obsolete_stats(valid_files, valid_namespaces);
    }
    else if (program.is_used("--grep-seq"))
    {
        std::cout << "Mode: grep-seq\n";
        process_consider_table(valid_files, valid_namespaces);
    }

    return 0;
}