#ifndef TASK_UTILS_HPP
#define TASK_UTILS_HPP

#include <string>
#include <vector>
#include "argparse/argparse.hpp" // For command-line parsing

// ---------------------------------------------------------------
// Header Declarations for GOfastaParser Utility Functions
// ---------------------------------------------------------------
// Author: Elton Ugbogu, University of Potsdam
// Used by: task1.cpp (main) and task_utils.cpp
// ---------------------------------------------------------------

//
// usage()
// -------
// Prints usage/help instructions for the CLI tool.
//
void usage();

//
// print_command_error()
// ---------------------
// Prints a CLI error message and usage info using argparse program and raw args.
//
void print_command_error(const argparse::ArgumentParser &program,
                         const std::vector<std::string> &args,
                         const std::string &error_message);

//
// print_command_usage()
// ---------------------
// Prints a CLI error message with usage instructions (fallback if argparse is not passed).
//
void print_command_usage(const std::vector<std::string> &args,
                         const std::string &error_message);

//
// has_fasta_ext()
// ---------------
// Checks if a given file path ends in `.fasta` or `.fasta.gz` (case-insensitive).
//
bool has_fasta_ext(const std::string &path);

//
// validate_files()
// ----------------
// Filters and returns only the files that:
//   - Exist on disk
//   - Have a valid `.fasta` or `.fasta.gz` extension
//
std::vector<std::string> validate_files(const std::vector<std::string> &files);

//
// validate_patterns()
// -------------------
// Filters and returns only the patterns that match UniProt FASTA ID format:
// Format: `sp|P12345|SOME_PROTEIN`
//
std::vector<std::string> validate_patterns(const std::vector<std::string> &patterns);

//
// print_invalid_filenames()
// -------------------------
// Prints warnings for items that look like filenames (contain dot `.`) but:
//   - Do not exist, or
//   - Have an invalid extension
//
void print_invalid_filenames(const std::vector<std::string> &args,
                             const std::vector<std::string> &invalid_items);

//
// print_invalid_pattern()
// ------------------------
// Prints warnings for items that do NOT contain a dot (assumed to be UniProt patterns)
// but don't match the required format.
//
void print_invalid_pattern(const std::vector<std::string> &args,
                           const std::vector<std::string> &invalid_items);

//
// process_search()
// ----------------
// Placeholder logic for the --search mode:
// Processes the valid FASTA files and optional UniProt patterns.
//
void process_search(const std::vector<std::string> &files,
                    const std::vector<std::string> &patterns);

//
// process_summary()
// -----------------
// Placeholder logic for the --summary mode:
// Computes statistics across valid FASTA files.
//
void process_summary(const std::vector<std::string> &files,
                     const std::vector<std::string> &patterns);

#endif // TASK_UTILS_HPP
