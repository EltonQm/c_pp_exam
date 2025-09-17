#ifndef TASK_UTILS_HPP // Prevent multiple inclusions
#define TASK_UTILS_HPP // Define header guard

#include <vector> // Dynamic arrays
#include <string> // String operations

// Forward declaration
namespace argparse
{
    class ArgumentParser;
}

// Print program usage instructions
void usage(const std::string &progname); // progname: program name

// Print error with args and usage
void print_command_usage(                 // Print error and usage
    const std::vector<std::string> &args, // args: cmd arguments
    const std::string &error_message);    // error_message: text

// Validate namespaces against allowed list
std::vector<std::string> validate_namespaces(    // Return valid namespaces
    const std::vector<std::string> &namespaces); // namespaces: input

// Validate files for existence and extension
std::vector<std::string> validate_files(    // Return valid files
    const std::vector<std::string> &files); // files: input paths

// CLI action for --consider-table (prints summary)
void run_consider_table(                         // Process consider-table
    const std::vector<std::string> &files,       // files: valid files
    const std::vector<std::string> &namespaces); // namespaces: valid

// CLI action for --obsolete-stats (prints summary)
void run_obsolete_stats(                         // Process obsolete-stats
    const std::vector<std::string> &files,       // files: valid files
    const std::vector<std::string> &namespaces); // namespaces: valid

// Print error with args
void print_command_error(                    // Print error message
    const argparse::ArgumentParser &program, // program: parser
    const std::vector<std::string> &args,    // args: cmd arguments
    const std::string &error_message);       // error_message: text

// Check if file has .obo or .obo.gz extension
bool has_obo_ext(const std::string &path); // path: file path

// Warn about invalid file extensions
void print_invalid_filenames(                       // Print file warnings
    const std::vector<std::string> &args,           // args: cmd arguments
    const std::vector<std::string> &invalid_items); // invalid_items: to check

// Warn about invalid namespace arguments
void print_invalid_namespace(                       // Print namespace warnings
    const std::vector<std::string> &args,           // args: cmd arguments
    const std::vector<std::string> &invalid_items); // invalid_items: to check

#endif // TASK_UTILS_HPP                                 // End header guard