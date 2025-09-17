// ---------------------------------------------------------------
// GOfastaParser Utilities - task_utils.cpp
// ---------------------------------------------------------------
// Author: Elton Ugbogu, University of Potsdam
// Description: Utility functions for validating files and FASTA ID patterns
// Modes: Used by task1.cpp for input validation, error handling, and CLI UX
// ---------------------------------------------------------------

#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "task_utils.hpp"

namespace fs = std::filesystem;

// ---------------------------------------------------------------
// Print CLI usage information
// ---------------------------------------------------------------
void usage()
{
    std::cout << R"(
Elton's FastaParser1 - Search & Summary on FASTA files
Author: Elton Ugbogu, University of Potsdam, 2024

Usage:
  FastaParser1 --search|--summary|--help ?PATTERN? file.fasta [file2.fasta ...]

- Files must exist and end in .fasta or .fasta.gz
)" << std::endl;
}

// ---------------------------------------------------------------
// Print error and show usage information
// ---------------------------------------------------------------
void print_command_error(const argparse::ArgumentParser &program,
                         const std::vector<std::string> &args,
                         const std::string &error_message)
{
    std::cerr << "Error: " << error_message << "\n";
    std::cerr << "You entered: ";
    for (const auto &arg : args)
        std::cerr << arg << " ";
    std::cerr << "\n\n";
    usage();
}

void print_command_usage(const std::vector<std::string> &args,
                         const std::string &error_message)
{
    std::cerr << "Error: " << error_message << "\n";
    std::cerr << "You entered: ";
    for (const auto &arg : args)
        std::cerr << arg << " ";
    std::cerr << "\n\n";
    usage();
}

// ---------------------------------------------------------------
// Check if file has valid .fasta or .fasta.gz extension
// ---------------------------------------------------------------
bool has_fasta_ext(const std::string &path)
{
    static const std::regex pattern(R"(.*\.fasta(\.gz)?$)", std::regex_constants::icase);
    return std::regex_match(path, pattern);
}

// ---------------------------------------------------------------
// Return only valid FASTA files that exist and have correct extension
// ---------------------------------------------------------------
std::vector<std::string> validate_files(const std::vector<std::string> &files)
{
    std::vector<std::string> valid;
    for (const auto &filepath : files)
    {
        if (has_fasta_ext(filepath) && fs::exists(filepath))
        {
            valid.push_back(filepath);
        }
    }
    return valid;
}

// ---------------------------------------------------------------
// Validate user-provided patterns as UniProt FASTA IDs (e.g., sp|P59637|VEMP_CVHSA)
// ---------------------------------------------------------------
std::vector<std::string> validate_patterns(const std::vector<std::string> &patterns)
{
    static const std::regex uniprot_pattern(R"(^[a-z]{2}\|[A-Z0-9]+\|[A-Z0-9_]+$)", std::regex::icase);

    std::vector<std::string> valid;
    for (const auto &p : patterns)
    {
        if (std::regex_match(p, uniprot_pattern))
        {
            valid.push_back(p);
        }
    }
    return valid;
}

// ---------------------------------------------------------------
// Print warning if filename appears invalid
// ---------------------------------------------------------------
void print_invalid_filenames(const std::vector<std::string> &args,
                             const std::vector<std::string> &invalid_items)
{
    static const std::regex dot_checker(R"(\.)");

    for (const auto &item : invalid_items)
    {
        if (std::regex_search(item, dot_checker))
        {
            if (!has_fasta_ext(item))
            {

                auto pos = item.rfind('.');
                std::string ext = (pos != std::string::npos) ? item.substr(pos + 1) : "(none)";

                std::ostringstream oss;
                oss << "Invalid file: " << item
                    << " — extension '" << ext << "' is not supported. "
                    << "Expected '.fasta' or '.fasta.gz'.";

                print_command_usage(args, oss.str());
            }
            if (!fs::exists(item))
            {
                std::cout << item << " does exist" << std::endl;
            }
        }
    }
}

// ---------------------------------------------------------------
// Print warning if a pattern is not a valid UniProt FASTA ID
// ---------------------------------------------------------------
void print_invalid_pattern(const std::vector<std::string> &args,
                           const std::vector<std::string> &invalid_items)
{
    static const std::regex dot_checker(R"(\.)");

    for (const auto &item : invalid_items)
    {
        if (!std::regex_search(item, dot_checker) && validate_patterns({item}).empty())
        {
            std::ostringstream oss;
            oss << "Invalid pattern: " << item
                << " — expected UniProt ID like sp|P59637|VEMP_CVHSA";
            print_command_usage(args, oss.str());
        }
    }
}

// ---------------------------------------------------------------
// Dummy processor for --search
// ---------------------------------------------------------------
void process_search(const std::vector<std::string> &files,
                    const std::vector<std::string> &patterns)
{
    std::cout << "[DEBUG] Files: " << files.size() << ", Patterns: " << patterns.size() << "\n";
}

// ---------------------------------------------------------------
// Dummy processor for --summary
// ---------------------------------------------------------------
void process_summary(const std::vector<std::string> &files,
                     const std::vector<std::string> &patterns)
{
    std::cout << "[DEBUG] Files: " << files.size() << ", Patterns: " << patterns.size() << "\n";
}
