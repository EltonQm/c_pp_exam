#ifndef TASK_UTILS_HPP
#define TASK_UTILS_HPP

#include <vector>
#include <string>
#include <unordered_set>
#include "argparse/argparse.hpp" // Include argparse header for CLI parsing

// ----------------------------------------------------
// FUNCTION DECLARATIONS FOR GO OBO FILE PROCESSING UTILS
// ----------------------------------------------------

//
// Reads lines from an OBO or OBO.GZ file and returns them as a vector of strings
// If the file ends in `.gz`, this should decompress before reading (implementation dependent).
//
std::vector<std::string> read_obo_lines(const std::string &filename);

//
// Prints usage information for the command-line tool.
//
void usage();

void print_command_usage(const std::vector<std::string> &args,
                         const std::string &error_message);

//
// Validates whether provided strings are known GO namespaces.
// Allowed: "molecular_function", "cellular_component", "biological_process"
//
std::vector<std::string> validate_namespaces(const argparse::ArgumentParser &program,
                                             const std::vector<std::string> &namespaces);

//
// Validates that provided file paths:
//   - Have `.obo` or `.obo.gz` extensions
//   - Exist on disk
//
std::vector<std::string> validate_files(const argparse::ArgumentParser &program,
                                        const std::vector<std::string> &files);

//
// Runs the processing logic to generate a table of obsolete GO terms
// with "consider" alternatives.
//
void process_consider_table(const std::vector<std::string> &files,
                            const std::vector<std::string> &namespaces);

//
// Runs the processing logic to compute stats on obsolete GO terms:
// - How many have "consider" replacements
// - How many are orphaned
//
void process_obsolete_stats(const std::vector<std::string> &files,
                            const std::vector<std::string> &namespaces);

//
// Prints a custom error message followed by usage/help text from argparse.
//
void print_command_error(const argparse::ArgumentParser &program,
                         const std::vector<std::string> &args,
                         const std::string &error_message);

//
// Checks whether a path has a valid .obo or .obo.gz extension (case-insensitive).
//
bool has_obo_ext(const std::string &path);

//
// Prints warnings for files that are invalid due to:
//   - Missing
//   - Invalid file extension (not .obo or .obo.gz)
//
void print_invalid_filenames(
    const std::vector<std::string> &args,
    const std::vector<std::string> &invalid_items);

//
// Prints warnings for invalid namespace strings
// (i.e., anything that doesn't match the allowed namespace list)
//
void print_invalid_namespace(
    const std::vector<std::string> &args,
    const std::vector<std::string> &invalid_items);

#endif // TASK_UTILS_HPP
