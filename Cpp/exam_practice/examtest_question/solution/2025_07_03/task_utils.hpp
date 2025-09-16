#ifndef TASK_UTILS_HPP
#define TASK_UTILS_HPP

#include <vector>
#include <string>
#include "argparse/argparse.hpp"

// ---------------------------------------------
// task_utils.hpp - Header file for GO OBO tools
// ---------------------------------------------
// This file declares utility functions for processing GO `.obo` files,
// including argument validation, help printing, and task handling.
//
// Author: Elton Ugbogu
// Project: GOfastaParser
// Year: 2025
// ---------------------------------------------

// ----------------------------------------------
// FUNCTION: usage
// ----------------------------------------------
// Print general usage information to stdout.
void usage();

// ----------------------------------------------
// FUNCTION: print_command_usage
// ----------------------------------------------
// Print a custom error message followed by the usage block.
// - args: full command-line arguments
// - error_message: the specific error to print
void print_command_usage(const std::vector<std::string> &args,
                         const std::string &error_message);

// ----------------------------------------------
// FUNCTION: print_command_error
// ----------------------------------------------
// Print a parser-related error with argparse and fallback to usage.
// - program: argparse parser object (in case extended logic is needed)
// - args: command-line arguments
// - error_message: the error to explain
void print_command_error(const argparse::ArgumentParser &program,
                         const std::vector<std::string> &args,
                         const std::string &error_message);

// ----------------------------------------------
// FUNCTION: has_obo_ext
// ----------------------------------------------
// Returns true if a file has valid extension `.obo` or `.obo.gz`
// Extension check is case-insensitive.
bool has_obo_ext(const std::string &path);

// ----------------------------------------------
// FUNCTION: validate_files
// ----------------------------------------------
// Filters and returns valid file paths that:
// - Exist on disk
// - Have valid extensions: `.obo`, `.obo.gz`
std::vector<std::string> validate_files(const std::vector<std::string> &files);

// ----------------------------------------------
// FUNCTION: validate_metaidentifiers
// ----------------------------------------------
// Returns only valid MetaCyc identifiers from input list.
// Valid formats include:
// - RXN-12345
// - PWY-6789
// Match is case-insensitive and validated with regex.
std::vector<std::string> validate_metaidentifiers(const std::vector<std::string> &metaidentifiers);

// ----------------------------------------------
// FUNCTION: print_invalid_filenames
// ----------------------------------------------
// Prints warnings for filenames that:
// - Have invalid extensions
// - Do not exist on disk
void print_invalid_filenames(
    const std::vector<std::string> &args,
    const std::vector<std::string> &invalid_items);

// ----------------------------------------------
// FUNCTION: print_invalid_metaidentifier
// ----------------------------------------------
// Prints warnings for invalid MetaCyc identifiers.
// Assumes input items without a dot (`.`) are candidate IDs.
void print_invalid_metaidentifier(
    const std::vector<std::string> &args,
    const std::vector<std::string> &invalid_items);

// ----------------------------------------------
// FUNCTION: process_get_metacyc
// ----------------------------------------------
// Logic for --get-metacyc mode.
// - Extract GO terms linked to specified MetaCyc IDs
void process_get_metacyc(const std::vector<std::string> &files,
                         const std::vector<std::string> &metaidentifiers);

// ----------------------------------------------
// FUNCTION: process_tab_metacyc
// ----------------------------------------------
// Logic for --tab-metacyc mode.
// - Generate a summary table of all MetaCyc → GO links
void process_tab_metacyc(const std::vector<std::string> &files,
                         const std::vector<std::string> &metaidentifiers);

#endif // TASK_UTILS_HPP
