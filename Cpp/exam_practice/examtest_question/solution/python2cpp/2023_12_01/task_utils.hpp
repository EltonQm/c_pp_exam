#ifndef TASK_UTILS_HPP
#define TASK_UTILS_HPP

#include <vector>
#include <string>
#include <unordered_set>
#include "argparse/argparse.hpp" // Include argparse header for CLI parsing

// ----------------------------------------------------
// FUNCTION DECLARATIONS FOR GO dat FILE PROCESSING UTILS
// ----------------------------------------------------

//
// Reads lines from an dat or dat.GZ file and returns them as a vector of strings
// If the file ends in `.gz`, this should decompress before reading (implementation dependent).
//
std::vector<std::string> read_dat_lines(const std::string &filename);

//
// Prints usage information for the command-line tool.
//
void usage();

void print_command_usage(const std::vector<std::string> &args,
                         const std::string &error_message);

//
// Validates whether provided strings are known GO uniprot_ids.
// Allowed: "molecular_function", "cellular_component", "biological_process"
//
std::vector<std::string> validate_uniprot_ids(
    const std::vector<std::string> &uniprot_ids);

//
// Validates that provided file paths:
//   - Have `.dat` or `.dat.gz` extensions
//   - Exist on disk
//
std::vector<std::string> validate_files(
    const std::vector<std::string> &files);

//
// Runs the processing logic to generate a table of obsolete GO terms
// with "consider" alternatives.
//
void process_seq_start(const std::vector<std::string> &files,
                       const std::vector<std::string> &uniprot_ids);

//
// Runs the processing logic to compute stats on obsolete GO terms:
// - How many have "consider" replacements
// - How many are orphaned
//
void process_get_entry(const std::vector<std::string> &files,
                       const std::vector<std::string> &uniprot_ids);

//
// Prints a custom error message followed by usage/help text from argparse.
//
void print_command_error(const argparse::ArgumentParser &program,
                         const std::vector<std::string> &args,
                         const std::string &error_message);

//
// Checks whether a path has a valid .dat or .dat.gz extension (case-insensitive).
//
bool has_dat_ext(const std::string &path);

//
// Prints warnings for files that are invalid due to:
//   - Missing
//   - Invalid file extension (not .dat or .dat.gz)
//
void print_invalid_filenames(
    const std::vector<std::string> &args,
    const std::vector<std::string> &invalid_items);

//
// Prints warnings for invalid uniprot_id strings
// (i.e., anything that doesn't match the allowed uniprot_id list)
//
void print_invalid_uniprot_id(
    const std::vector<std::string> &args,
    const std::vector<std::string> &invalid_items);

#endif // TASK_UTILS_HPP
