#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <regex>

namespace fs = std::filesystem;

// usage
void usage()
{
    std::cout << "Usage examples:\n"
              << " --search PATTERN FILE1 [FILE2...]\n"
              << " --search PATTERN1 PATTERN2 FILE1 [FILE2...]\n"
              << " --summary FILE1 [FILE2...]\n"
              << " --help   (shows help message)\n\n"
              << "Note: FILE arguments must end with .fasta\n";
}

// Checks if a file path has a .fasta extension (case insensitive)
static bool has_fasta_ext(const std::string &path)
{
    // Find the last occurrence of '.' in the path
    auto pos = path.rfind('.');

    // If no '.' found, it has no extension
    if (pos == std::string::npos)
        return false;

    // Extract substring after the last '.'
    std::string ext = path.substr(pos + 1);

    // Convert extension to lowercase
    for (char &c : ext)
        c = tolower(c);

    // Check if extension equals "fasta"
    return ext == "fasta";
}

// print error message
void print_command_error(argparse::ArgumentParser &program,
                         const std::vector<std::string> & /*args*/,
                         const std::string &error_message)
{
    std::cerr << "Error: " << error_message << "\n\n"
              << program; // prints usage/help text
}

// Validate FASTA files and exit on error
std::vector<std::string> validate_files(argparse::ArgumentParser &program,
                                        const std::vector<std::string> &args,
                                        const std::vector<std::string> &files)
{
    if (files.empty())
    {
        print_command_error(program, args, "No input files provided");
    }

    std::vector<std::string> valid_files;
    static const std::regex dot_checker_regex(R"(\.)");

    for (const auto &filepath : files)
        if (std::regex_search(filepath, dot_checker_regex))

        {
            if (!has_fasta_ext(filepath))
            {
                print_command_error(program, args,
                                    "Invalid extension: '" + filepath + "'. Expected '.fasta'.");
            }
            if (!fs::exists(filepath))
            {
                print_command_error(program, args,
                                    "File not found: '" + filepath + "'");
            }
            if (has_fasta_ext(filepath) && fs::exists(filepath))
            {
                valid_files.push_back(filepath);
            }
        }

        else
        {
            continue;
        }
    return valid_files;
}

// validate pattern
std::vector<std::string> validate_pattern(argparse::ArgumentParser &program,
                                          const std::vector<std::string> &args,
                                          const std::vector<std::string> &patterns)
{
    if (patterns.empty())
    {
        print_command_error(program, args, "No input Arguments");
    }

    std::vector<std::string> valid_patterns;

    static const std::regex dot_checker_regex(R"(\.)");

    for (const auto &valid_pattern : patterns)

    {
        if (std::regex_search(valid_pattern, dot_checker_regex))
        {
            continue;
        }
        else
        {
            if (!has_fasta_ext(valid_pattern))
            {
                valid_patterns.push_back(valid_pattern);
            }

            if (valid_patterns.empty())
            {
                print_command_error(program, args, "No input Pattern");
            }
        }
    }
    return valid_patterns;
}

int main(int argc, char *argv[])
{
    // Store original command line arguments
    std::vector<std::string> args(argv, argv + argc);

    argparse::ArgumentParser program("FastaParser", "1.0");
    program.add_description(R"(
Elton's FastaParser - FASTA file processor
Author: Elton Ugbogu, University of Potsdam, 2025
)");

    // Argument setup
    program.add_argument("--search")
        .help("Search for PATTERN in FASTA files")
        .nargs(argparse::nargs_pattern::at_least_one);

    program.add_argument("--summary")
        .help("Generate summary of FASTA files")
        .nargs(argparse::nargs_pattern::at_least_one);

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        print_command_error(program, args, err.what());
    }

    auto search_args = program.get<std::vector<std::string>>("--search");
    auto summary_args = program.get<std::vector<std::string>>("--summary");

    bool mode_search = !search_args.empty();
    bool mode_summary = !summary_args.empty();

    if (mode_search && mode_summary)
    {
        print_command_error(program, args,
                            "--search and --summary are mutually exclusive");
        return 1;
    }

    if (!mode_search && !mode_summary)
    {

        usage();
        return 1;
    }

    if (mode_search)

    {
        if (search_args.size() < 1)
        {
            print_command_error(program, args,
                                "--search requires at least a PATTERN and one FASTA file");
            return 1;
        }
        else
        {
            // for (const auto &arguments : search_args)

            auto valid_files = validate_files(program, args, search_args);
            for (const auto &file : valid_files)
            {
                std::cout << "  Processing: " << file << "\n";
                // Search implementation would go here
            }
            auto valid_patterns = validate_pattern(program, args, search_args);

            if (valid_patterns.empty())
            {
                print_command_error(program, args, "No input Pattern");
                return 1;
            }

            for (const auto &val_pattern : valid_patterns)
            {
                std::cout << "Searching for pattern: " << val_pattern << "\n";
                // Search implementation would go here
            }
        }
        return 0;
    }

    if (mode_summary)
    {
        // if (search_args.empty() && !summary_args.empty())
        //{ // summary mode
        if (summary_args.size() < 1)
        {
            print_command_error(program, args,
                                "Incomplete argument; At least one file needed");
            return 1;
        }
        else
        {
            // for (const auto &arguments : summary_args)
            //{
            auto valid_patterns = validate_pattern(program, args, summary_args);
            if (!valid_patterns.empty())
            {
                print_command_error(program, args,
                                    "No Pattern required");
                return 1;
            }
            auto valid_files = validate_files(program, args, summary_args);
            for (const auto &file : valid_files)
            {
                std::cout << "  Processing: " << file << "\n";
                // Search implementation would go here
            }
            std::cout << "Implementing Summary" << "\n ";
            //}
            return 0;
        }
    }
    return 1;
}
