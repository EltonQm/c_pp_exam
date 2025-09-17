#include <argparse/argparse.hpp> // Parse command-line args
#include <filesystem>            // Handle file operations
#include <iostream>              // Console input/output
#include <string>                // String operations
#include <vector>                // Dynamic arrays
#include <regex>                 // Pattern matching
#include "task_utils.hpp"        // Task utility declarations

namespace fs = std::filesystem; // Alias for filesystem

// Print program usage instructions
void usage(const std::string &progname)
{                                   // progname: program name
    std::cout << "Example usage:\n" // Output usage header
              << progname << " --consider-table file1.obo [file2.obo ...] [ns]\n"
              << progname << " --obsolete-stats file1.obo [file2.obo ...] [ns]\n"
              << "Notes:\n" // Output notes header
              << "- Namespace is optional: molecular_function, "
              << "cellular_component, biological_process\n" // Valid namespaces
              << "- Files must have .obo or .obo.gz extension and exist\n"
              << std::endl; // End with newline
}

// Print error with args (unused here)
void print_command_error(                    // Print error message
    const argparse::ArgumentParser &program, // program: parser
    const std::vector<std::string> &args,    // args: cmd arguments
    const std::string &error_message)
{                                                    // error_message: text
    std::cerr << "Error: " << error_message << "\n"; // Print error
    std::cerr << "You entered: ";                    // Output args header
    for (const auto &arg : args)
    {                            // Loop through args
        std::cerr << arg << " "; // Print each arg
    }
    std::cerr << "\n\n"; // Add newlines
    usage(args[0]);      // Call usage with progname
}

// Print error with args and usage
void print_command_usage(                 // Print error and usage
    const std::vector<std::string> &args, // args: cmd arguments
    const std::string &error_message)
{                                                    // error_message: text
    std::cerr << "Error: " << error_message << "\n"; // Print error
    std::cerr << "You entered: ";                    // Output args header
    for (const auto &arg : args)
    {                            // Loop through args
        std::cerr << arg << " "; // Print each arg
    }
    std::cerr << "\n\n"; // Add newlines
    usage(args[0]);      // Call usage with progname
}

// Check if file has .obo or .obo.gz extension
bool has_obo_ext(const std::string &path)
{                                    // path: file path
    static const std::regex pattern( // Regex for .obo/.obo.gz
        R"(.*\.obo(\.gz)?$)", std::regex_constants::icase);
    return std::regex_match(path, pattern); // True if matches
}

// Validate files for existence and extension
std::vector<std::string> validate_files( // Return valid files
    const std::vector<std::string> &files)
{                                         // files: input paths
    std::vector<std::string> valid_files; // Store valid files
    for (const auto &filepath : files)
    {                                           // Loop through files
        bool valid_ext = has_obo_ext(filepath); // Check extension
        bool exists = fs::exists(filepath);     // Check file exists
        if (valid_ext && exists)
        {                                    // If valid and exists
            valid_files.push_back(filepath); // Add to valid files
        }
        else
        {             // Skip invalid files
            continue; // Continue loop
        }
    }
    return valid_files; // Return valid files
}

// Validate namespaces against allowed list
std::vector<std::string> validate_namespaces( // Return valid namespaces
    const std::vector<std::string> &namespaces)
{                                                    // namespaces: input
    static const std::vector<std::string> allowed = {// Allowed namespaces
                                                     "molecular_function", "cellular_component", "biological_process"};
    std::vector<std::string> valid_namespace; // Store valid namespaces
    for (const auto &ns : namespaces)
    { // Loop through namespaces
        if (std::find(allowed.begin(), allowed.end(), ns) != allowed.end())
        {
            valid_namespace.push_back(ns); // Add valid namespace
        }
        else
        {             // Skip invalid namespaces
            continue; // Continue loop
        }
    }
    return valid_namespace; // Return valid namespaces
}

// Warn about invalid file extensions
void print_invalid_filenames(             // Print file warnings
    const std::vector<std::string> &args, // args: cmd arguments
    const std::vector<std::string> &invalid_items)
{                                                 // invalid_items: to check
    static const std::regex dot_checker(R"(\.)"); // Regex to detect dot
    for (const auto &item : invalid_items)
    { // Loop through items
        if (std::regex_search(item, dot_checker))
        { // Check for dot
            if (!has_obo_ext(item))
            {                                                                                 // Invalid extension
                auto pos = item.rfind('.');                                                   // Find last dot
                std::string ext = pos != std::string::npos ? item.substr(pos + 1) : "(none)"; // Get ext or none
                std::ostringstream oss;                                                       // Build error message
                oss << "Invalid file: " << item << " — extension '" << ext
                    << "' not supported. Expected .obo or .obo.gz.";
                print_command_usage(args, oss.str()); // Print error
            }
            if (!fs::exists(item))
            {                                             // File does not exist
                std::cout << item << " does not exist\n"; // Print warning
            }
        }
    }
}

// Warn about invalid namespace arguments
void print_invalid_namespace(             // Print namespace warnings
    const std::vector<std::string> &args, // args: cmd arguments
    const std::vector<std::string> &invalid_items)
{                                                       // invalid_items: to check
    static const std::regex dot_checker_regex(R"(\.)"); // Regex to detect dot
    for (const auto &item : invalid_items)
    {                                                      // Loop through items
        if (!std::regex_search(item, dot_checker_regex) && // No dot, not file
            validate_namespaces({item}).empty())
        {                                        // Invalid namespace
            std::ostringstream oss;              // Build error message
            oss << "Invalid namespace: " << item // Error message
                << " — allowed: molecular_function, cellular_component, "
                << "biological_process\n";
            print_command_usage(args, oss.str()); // Print error
        }
    }
}

// Process --consider-table command
void process_consider_table(               // Process consider-table
    const std::vector<std::string> &files, // files: valid files
    const std::vector<std::string> &namespaces)
{                                                     // namespaces: valid
    std::cout << files.size() << " files and "        // Output file count
              << namespaces.size() << " namespaces\n" // Output namespace count
              << std::endl;                           // End with newline
}

// Process --obsolete-stats command
void process_obsolete_stats(               // Process obsolete-stats
    const std::vector<std::string> &files, // files: valid files
    const std::vector<std::string> &namespaces)
{                                                     // namespaces: valid
    std::cout << files.size() << " files and "        // Output file count
              << namespaces.size() << " namespaces\n" // End with newline
              << std::endl;                           // End with newline
}
