#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include "task_utils.hpp" // Function declarations

namespace fs = std::filesystem;

// ------------------------
// Help Usage Function
// ------------------------
void usage()
{
    std::cout << "Usage:\n"
              << "  GOfastaParser [--help | --get-metacyc MetaCyc-ID | --tab-metacyc] OBOFILE\n\n";
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
    usage();
}

// Print error with usage
void print_command_usage(const std::vector<std::string> &args,
                         const std::string &error_message)
{
    std::cerr << "Error: " << error_message << "\n";
    std::cerr << "You entered: ";
    for (const auto &arg : args)
        std::cerr << arg << " ";
    std::cerr << "\n\n";
    usage();
}

// Function to check valid .obo or .obo.gz extension
bool has_obo_ext(const std::string &path)
{
    static const std::regex pattern(R"(.*\.obo(\.gz)?$)", std::regex_constants::icase);
    return std::regex_match(path, pattern);
}

// Validate files
std::vector<std::string> validate_files(const std::vector<std::string> &files)
{
    std::vector<std::string> valid_files;
    for (const auto &filepath : files)
    {
        if (has_obo_ext(filepath) && fs::exists(filepath))
        {
            valid_files.push_back(filepath);
        }
    }
    return valid_files;
}

// Validate MetaCyc identifiers (e.g., RXN-12345, PWY-6789)
std::vector<std::string> validate_metaidentifiers(const std::vector<std::string> &metaidentifiers)
{
    static const std::regex metacyc_regex(R"(^\s*(RXN|PWY)-\d+\s*$)", std::regex::icase);
    std::vector<std::string> valid_metaidentifier;
    for (const auto &id : metaidentifiers)
    {
        if (std::regex_match(id, metacyc_regex))
        {
            valid_metaidentifier.push_back(id);
        }
    }
    return valid_metaidentifier;
}

// Print invalid file names
void print_invalid_filenames(const std::vector<std::string> &args,
                             const std::vector<std::string> &invalid_items)
{
    static const std::regex dot_checker_regex(R"(\.)");

    for (const auto &item : invalid_items)
    {
        if (std::regex_search(item, dot_checker_regex) && !has_obo_ext(item) && !fs::exists(item))
        {
            auto pos = item.rfind('.');
            std::string ext = (pos != std::string::npos) ? item.substr(pos + 1) : "(none)";

            std::ostringstream oss;
            oss << "Invalid file: " << item
                << " — extension '" << ext << "' is not supported. "
                << "Expected '.obo' or '.obo.gz'.";

            print_command_usage(args, oss.str());
        }
    }
}

// Print invalid MetaCyc IDs
void print_invalid_metaidentifier(const std::vector<std::string> &args,
                                  const std::vector<std::string> &invalid_items)
{
    static const std::regex dot_checker_regex(R"(\.)");

    for (const auto &item : invalid_items)
    {
        if (!std::regex_search(item, dot_checker_regex) &&
            validate_metaidentifiers({item}).empty())
        {

            std::ostringstream oss;
            oss << "Invalid MetaCyc ID: " << item
                << " — expected format like RXN-1234 or PWY-5678.";

            print_command_usage(args, oss.str());
        }
    }
}

// Dummy task processors
void process_get_metacyc(const std::vector<std::string> &files,
                         const std::vector<std::string> &metaidentifiers)
{
    std::cout << "Processing get-metacyc with "
              << files.size() << " file(s) and "
              << metaidentifiers.size() << " MetaCyc ID(s).\n";
}

void process_tab_metacyc(const std::vector<std::string> &files,
                         const std::vector<std::string> &metaidentifiers)
{
    std::cout << "Processing tab-metacyc with "
              << files.size() << " file(s).\n";
}
