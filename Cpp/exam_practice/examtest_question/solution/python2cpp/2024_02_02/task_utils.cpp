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
Usage: GOoboparser --help|--go|--doi [FILE] ?[FILE] ...?

Uniprot-Parser by Elton Ugbogu, 2025
Extract information from GO .obo files.
-------------------------------------------
Optional arguments:
    --help  - display this help page
    --go    - generate GO ID tables
    --doi   - show obsolete term stats

Mandatory arguments:
    FILE - one or more .obo or .obo.gz files
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
std::vector<std::string> validate_files(const argparse::ArgumentParser & /*program*/,
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
void print_invalid_filenames(const argparse::ArgumentParser & /*program*/, const std::vector<std::string> &args,
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

// Dummy declarations (implement somthing)
void get_go_ids(const std::vector<std::string> &files)
{
    std::cout << "get_go_ids called" << std::endl;
    std::cout
        << files.size() << " files and " << std::endl;
}

void get_kegg_ids(const std::vector<std::string> &files)
{
    std::cout << "get_keggs_id calles" << std::endl;
    std::cout
        << files.size() << " files and " << std::endl;
}