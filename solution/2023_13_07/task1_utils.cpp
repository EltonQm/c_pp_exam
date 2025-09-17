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

  ./GOoboparser --consider-table file1.obo [file2.obo ...] [namespace]
  ./GOoboparser --obsolete-stats file1.obo [file2.obo ...] [namespace]

Notes:
- Namespace is optional but must be one of: molecular_function, cellular_component, biological_process
- Files must have .obo or .obo.gz extension and exist
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

// Function to validate whether user-provided namespace strings are part of an allowed list
// Returns a vector of only the valid namespaces found in the input
std::vector<std::string> validate_namespaces(
    const std::vector<std::string> &namespaces)
{
    // Define the set of allowed Gene Ontology namespaces as a static constant.
    // These are the only valid options a user can pass.
    static const std::vector<std::string> allowed = {
        "molecular_function",  // Functions of gene products (e.g., enzyme activity)
        "cellular_component",  // Where gene products are active (e.g., nucleus)
        "biological_process"}; // Larger processes (e.g., mitosis, immune response)

    // This vector will store only the namespaces from `namespaces` input that match the allowed list.
    std::vector<std::string> valid_namespace;

    // Loop through each user-provided namespace
    for (const auto &ns : namespaces)
    {
        // Check if the current namespace `ns` exists in the allowed list using std::find
        if (std::find(allowed.begin(), allowed.end(), ns) != allowed.end())
        {
            // If valid, add it to the output vector
            valid_namespace.push_back(ns);
        }
        else
        {
            // If not in the allowed list, skip it
            continue;
        }
    }

    // Return the list of valid namespaces (could be empty if none matched)
    return valid_namespace;
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

// Function: print_invalid_namespace
// ---------------------------------
// This function checks a list of input strings (e.g., command-line arguments) to identify
// those that are intended to be GO namespaces, but are invalid.
//
// Parameters:
// - program: the argparse object (not used directly here, but passed for consistency / potential use)
// - args: the full command-line arguments (not used in this function)
// - invalid_items: a list of arguments that were not already identified as valid files or namespaces
//
// Behavior:
// - It treats items *without a dot* (`.`) as candidate namespaces.
// - It prints a warning if such an item does *not* match a valid namespace.
void print_invalid_namespace(
    const std::vector<std::string> &args,
    const std::vector<std::string> &invalid_items)
{
    // Regex to detect if the argument contains a dot (likely a filename)
    static const std::regex dot_checker_regex(R"(\.)");

    for (const auto &item : invalid_items)
    {
        // If the item does NOT contain a dot AND is not a valid namespace
        if (!std::regex_search(item, dot_checker_regex) &&
            validate_namespaces(program, {item}).empty())
        {
            // Build a full error message using ostringstream
            std::ostringstream oss;
            oss << "Invalid namespace: " << item
                << " — allowed values are: molecular_function, cellular_component, biological_process" << std::endl;

            // Print the full error message with context
            print_command_usage(args, oss.str());
        }
    }
}

// Dummy declarations (implement somthing)
void process_consider_table(const std::vector<std::string> &files,
                            const std::vector<std::string> &namespaces)
{
    std::cout
        << files.size() << " files and "
        << namespaces.size() << " namespaces\n"
        << std::endl;
}

void process_obsolete_stats(const std::vector<std::string> &files,
                            const std::vector<std::string> &namespaces)
{
    std::cout
        << files.size() << " files and "
        << namespaces.size() << " namespaces\n"
        << std::endl;
}
