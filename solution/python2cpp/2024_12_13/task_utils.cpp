#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "task_utils.hpp" // Function declarations

namespace fs = std::filesystem;

void usage()
{
    std::cout << R"(
Example usage:

  (Usage: GOoboparser --help | --go-search GO-ID FILE | --go-count GO-ID FILE

Notes:
Mandatory argument:
  FILE               One compressed or uncompressed Uniprot file (.dat or .dat.gz)

Examples:
  ./GOoboparser --go-search GO:0005576 uniprot1.dat
  ./GOoboparser --go-count GO:0022002 uniprot1.dat.gz


)";
}

// Print error with help
void print_command_error(const argparse::ArgumentParser &program,
                         const std::vector<std::string> &args,
                         const std::string &error_message)
{
    std::cerr << "Error: " << error_message << "\n";

    std::cerr << "You entered: ";
    for (const auto &arg : args)
        std::cerr << arg << " ";
    std::cerr << "\n\n";

    usage(); // correctly call without << operator
}

// Print error with usage
void print_command_usage(
    const std::vector<std::string> &args,
    const std::string &error_message)
{
    std::cerr << "Error: " << error_message << "\n";

    std::cerr << "You entered: ";
    for (const auto &arg : args)
        std::cerr << arg << " ";
    std::cerr << "\n\n";

    usage(); // correctly call without << operator
}

// Function to check if a given file path has a valid OBO or OBO.GZ extension (case-insensitive)
bool has_obo_ext(const std::string &path)
{
    // Regex matches:
    //   - any filename ending in ".obo"
    //   - or ".obo.gz" (compressed version)
    //   - case-insensitive match due to std::regex_constants::icase
    static const std::regex pattern(R"(.*\.obo(\.gz)?$)", std::regex_constants::icase);

    return std::regex_match(path, pattern);
}

// Validates that input strings represent existing files with valid .obo/.obo.gz extensions
// Returns a vector of only the valid files from the input list
std::vector<std::string> validate_files(
    const std::vector<std::string> &files)
{
    // This vector will store only the valid files
    std::vector<std::string> valid_files;

    for (const auto &filepath : files)
    {
        // Check if the file has a valid extension
        bool valid_ext = has_obo_ext(filepath);

        // Check if the file actually exists on disk
        bool exists = fs::exists(filepath);

        if (valid_ext && exists)
        {
            // If both extension and file existence are valid, keep it
            valid_files.push_back(filepath);
        }
        else
        {
            continue;
        }
    }

    return valid_files;
}

// Function to extract valid GO IDs from a list of command-line arguments.
// Only IDs matching the GO: followed by exactly 7 digits (e.g., GO:0008150) are accepted.
std::vector<std::string> validate_go_id(const std::vector<std::string> &args)
{
    // Define a regular expression for valid GO IDs
    static const std::regex go_id_pattern(R"(GO:\d{7})");

    // Output vector to collect valid GO IDs
    std::vector<std::string> valid_go_ids;

    // Loop through each argument and test against regex
    for (const auto &arg : args)
    {
        if (std::regex_match(arg, go_id_pattern))
        {
            valid_go_ids.push_back(arg); // Valid GO ID → keep
        }
        else
        {
            continue; // Not a GO ID → ignore
        }
    }

    return valid_go_ids;
}

// Function: print_invalid_filenames
// ----------------------------------
// This function takes a list of potentially invalid file names (`invalid_items`) and
// checks if their extensions are valid and whether the files exist.
// It prints warnings for those with invalid extensions.
//
// Parameters:
// - program: the argparse program object, passed in case extended error printing is desired
// - args: the original command-line arguments (not used in this function)
// - invalid_items: list of user-provided arguments to check for validity
// Print warnings for inputs that look like files but have invalid extensions
void print_invalid_filenames(const std::vector<std::string> &args,
                             const std::vector<std::string> &invalid_items)
{
    // A regex to detect if a string contains a dot (indicating a filename)
    static const std::regex dot_checker_regex(R"(\.)");

    for (const auto &item : invalid_items)
    {
        // Looks like a file (has dot) but is not a valid .obo or .obo.gz
        if (std::regex_search(item, dot_checker_regex) && !has_obo_ext(item) && !fs::exists(item))
        {
            // Extract extension for message
            auto pos = item.rfind('.');
            std::string ext = item.substr(pos + 1);

            // Build error message
            std::ostringstream oss;
            oss << "Invalid file: " << item
                << " — extension '" << ext << "' is not supported. "
                << "Expected '.obo' or '.obo.gz'.";

            // Print error message with usage hint
            print_command_usage(args, oss.str());
        }
    }
}

// Function: print_invalid_go_id
// ---------------------------------
// This function checks a list of input strings (e.g., command-line arguments) to identify
// those that are intended to be GO go_id, but are invalid.
//
// Parameters:
// - program: the argparse object (not used directly here, but passed for consistency / potential use)
// - args: the full command-line arguments (not used in this function)
// - invalid_items: a list of arguments that were not already identified as valid files or go_id
//
// Behavior:
// - It treats items *without a dot* (`.`) as candidate go_id.
// - It prints a warning if such an item does *not* match a valid go_id.
void print_invalid_go_id(
    const std::vector<std::string> &args,
    const std::vector<std::string> &invalid_items)
{
    // Regex to detect if the argument contains a dot (likely a filename)
    static const std::regex dot_checker_regex(R"(\.)");

    for (const auto &item : invalid_items)
    {
        // If the item does NOT contain a dot AND is not a valid go_id
        if (!std::regex_search(item, dot_checker_regex) &&
            validate_go_id({item}).empty())
        {
            // Build a full error message using ostringstream
            std::ostringstream oss;
            oss << "Invalid go_id: " << item
                << " — allowed values are: molecular_function, cellular_component, biological_process";

            // Print the full error message with context
            print_command_usage(args, oss.str());
        }
    }
}

// Dummy declarations (implement somthing)
void process_count(const std::vector<std::string> &files,
                   const std::vector<std::string> &go_id)
{
    std::cout
        << files.size() << " files and "
        << go_id.size() << " go_id\n";
}

void process_search(const std::vector<std::string> &files,
                    const std::vector<std::string> &go_id)
{
    std::cout
        << files.size() << " files and "
        << go_id.size() << " go_id\n";
}