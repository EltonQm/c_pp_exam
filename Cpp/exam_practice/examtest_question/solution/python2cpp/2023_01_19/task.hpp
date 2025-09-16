#ifndef TASK_UTILS_HPP
#define TASK_UTILS_HPP

#include <vector>
#include <string>
#include <unordered_set>
#include "argparse/argparse.hpp" // For command-line parsing

// ─────────────────────────────────────────────────────────────
// FUNCTION DECLARATIONS FOR FASTA FILE PARSING UTILITIES
// ─────────────────────────────────────────────────────────────

//
// Reads lines from a FASTA or FASTA.GZ file and returns them as a vector of strings.
// If the file ends in `.gz`, it should be decompressed during reading.
//
std::vector<std::string> read_fasta_lines(const std::string &filename);

//
// Prints usage instructions to stdout (when no args or --help)
//
void usage();

//
// Prints a detailed command-line usage message for errors.
//
void print_command_usage(const std::vector<std::string> &args,
                         const std::string &error_message);

//
// Validates user-supplied file paths:
//   - Must end in `.fasta` or `.fasta.gz` (case-insensitive)
//   - Must exist on disk
//
std::vector<std::string> validate_files(const argparse::ArgumentParser &program,
                                        const std::vector<std::string> &files);

//
// Warns about arguments that are not valid FASTA files.
//
void print_invalid_filenames(const argparse::ArgumentParser &program,
                             const std::vector<std::string> &args,
                             const std::vector<std::string> &invalid_items);

//
// Handles unknown arguments (not valid files or recognized options).
//
void print_invalid_arguments(const argparse::ArgumentParser &program,
                             const std::vector<std::string> &args,
                             const std::vector<std::string> &invalid_items);

//
// Checks whether a given path ends with `.fasta` or `.fasta.gz` (case-insensitive).
//
bool has_fasta_ext(const std::string &path);

//
// Extracts sequence lengths from a FASTA file and prints them
// (used for --get-n mode).
//
void process_length_summary(const std::vector<std::string> &lines);

//
// Prints the sequence for a given ID from a FASTA file
// (used for --get-seq mode).
//
void process_get_sequence(const std::vector<std::string> &lines,
                          const std::string &query_id);

//
// Searches sequences using a regex pattern (amino acids, etc.)
// (used for --grep-seq mode).
//
void process_grep_sequence(const std::vector<std::string> &lines,
                           const std::string &pattern);

//
// Prints an error message from the parser followed by usage info.
//
void print_command_error(const argparse::ArgumentParser &program,
                         const std::vector<std::string> &args,
                         const std::string &error_message);

#endif // TASK_UTILS_HPP
