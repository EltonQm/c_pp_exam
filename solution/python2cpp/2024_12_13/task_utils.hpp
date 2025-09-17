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
// Validates whether provided strings are known GO go_id.
// Allowed: "molecular_function", "cellular_component", "biological_process"
//
std::vector<std::string> validate_go_id(
    const std::vector<std::string> &go_id);

//
// Validates that provided file paths:
//   - Have `.obo` or `.obo.gz` extensions
//   - Exist on disk
//
std::vector<std::string> validate_files(
    const std::vector<std::string> &files);

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
// Prints warnings for invalid go_id strings
// (i.e., anything that doesn't match the allowed go_id list)
//
void print_invalid_go_id(
    const std::vector<std::string> &args,
    const std::vector<std::string> &invalid_items);

void process_count(const std::vector<std::string> &files,
                   const std::vector<std::string> &go_id);

void process_search(const std::vector<std::string> &files,
                    const std::vector<std::string> &go_id);

#endif // TASK_UTILS_HPP
