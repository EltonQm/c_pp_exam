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

  ./GOdatparser ----get-entry file1.dat [file2.dat ...] [uniprot_id]
  ./GOdatparser --seq-start file1.dat [file2.dat ...] [uniprot_id]
  

Notes:
- -help|--| [FILE] ?[FILE ...]?  ?[UPID]?
)" << std::endl;
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

// Function to check if a given file path has a valid dat or dat.GZ extension (case-insensitive)
bool has_dat_ext(const std::string &path)
{
    // Regex matches:
    //   - any filename ending in ".dat"
    //   - or ".dat.gz" (compressed version)
    //   - case-insensitive match due to std::regex_constants::icase
    static const std::regex pattern(R"(.*\.dat(\.gz)?$)", std::regex_constants::icase);

    return std::regex_match(path, pattern);
}

// Validates that input strings represent existing files with valid .dat/.dat.gz extensions
// Returns a vector of only the valid files from the input list
std::vector<std::string> validate_files(
    const std::vector<std::string> &files)
{
    // This vector will store only the valid files
    std::vector<std::string> valid_files;

    for (const auto &filepath : files)
    {
        // Check if the file has a valid extension
        bool valid_ext = has_dat_ext(filepath);

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

std::vector<std::string> validate_uniprot_ids(const std::vector<std::string> &ids)
{
    std::vector<std::string> valid_ids;

    // Regex pattern for UniProt-like IDs (e.g., AP3A_SARS2, P12345, Q9XYZ1)
    std::regex pattern(R"([A-Z0-9]+_[A-Z0-9]+)");

    for (const auto &id : ids)
    {
        if (std::regex_match(id, pattern))
        {
            valid_ids.push_back(id);
        }
        else
        {
            continue;
        }
    }

    return valid_ids;
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
        // Looks like a file (has dot) but is not a valid .dat or .dat.gz
        if (std::regex_search(item, dot_checker_regex) && !has_dat_ext(item) && !fs::exists(item))
        {
            // Extract extension for message
            auto pos = item.rfind('.');
            std::string ext = item.substr(pos + 1);

            // Build error message
            std::ostringstream oss;
            oss << "Invalid file: " << item
                << " — extension '" << ext << "' is not supported. "
                << "Expected '.dat' or '.dat.gz'." << std::endl;

            // Print error message with usage hint
            print_command_usage(args, oss.str());
        }
    }
}

// Function: print_invalid_uniprot_id
// ---------------------------------
// Checks inputs that are not files (i.e., don't contain a dot) and verifies
// if they are valid UniProt-style IDs. Prints warnings for any invalid entries.
void print_invalid_uniprot_id(
    const std::vector<std::string> &args,
    const std::vector<std::string> &invalid_items)
{
    static const std::regex dot_checker_regex(R"(\.)");

    for (const auto &item : invalid_items)
    {
        // If the item does NOT contain a dot (i.e., likely not a filename)
        // and is NOT a valid UniProt ID
        if (!std::regex_search(item, dot_checker_regex) &&
            validate_uniprot_ids({item}).empty())
        {
            std::ostringstream oss;
            oss << "Invalid UniProt ID: " << item
                << " — expected format like AP3A_SARS2 (UPPERCASE alphanumerics with underscore)" << std::endl;

            print_command_usage(args, oss.str());
        }
    }
}

// Dummy declarations (implement somthing)
void process_seq_start(const std::vector<std::string> &files,
                       const std::vector<std::string> &uniprot_ids)
{
    std::cout
        << files.size() << " files and "
        << uniprot_ids.size() << " uniprot_ids\n"
        << std::endl;
}

void process_get_entry(const std::vector<std::string> &files,
                       const std::vector<std::string> &uniprot_ids)
{
    std::cout
        << files.size() << " files and "
        << uniprot_ids.size() << " uniprot_ids\n"
        << std::endl;
}