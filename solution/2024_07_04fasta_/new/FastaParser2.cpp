#include <argparse/argparse.hpp> // Command-line argument parsing
#include <filesystem>            // File system operations
#include <fstream>               // File stream operations
#include <iostream>              // Console input/output
#include <regex>                 // Regular expression matching
#include <string>                // String operations
#include <tuple>                 // Tuple data structure
#include <unordered_set>         // Hash set for deduplication
#include <vector>                // Dynamic arrays
#include <stdexcept>             // Exception handling
#include <cctype>                // Character classification
#include <algorithm>             // String transformations
#include <sstream>               // String stream operations

namespace fs = std::filesystem; // Alias for filesystem namespace

// Tuple to store sequence ID, pattern, and match result
using SearchTuple = std::tuple<std::string, std::string, bool>;

// Display program usage with examples
static void usage(const std::string &prog)
{ // prog: program name
    std::cout << "Usage:\n"
              << "  " << prog << " --search <FILES and PATTERNS in any order>\n"
              << "  " << prog << " --summary <FILES...>\n\n"
              << "Examples:\n"
              << "  " << prog << " --search sars-cov1.fasta \"MAT\"\n"
              << "  " << prog << " --search \"sp|P59637|VEMP_CVHSA\" sars-cov2.fasta sars-cov1.fasta\n"
              << "  " << prog << " --search sars-cov1.fasta sars-cov2.fasta \"A.T\" \"MAT\"\n"
              << "  " << prog << " --summary sars-cov1.fasta sars-cov2.fasta\n\n"
              << "Notes:\n"
              << "  - Files must have .fasta or .fasta.gz extension (case-insensitive).\n"
              << "  - Patterns are regular expressions; quote complex patterns.\n"
              << "  - Search concatenates sequence lines, ignoring line breaks.\n"
              << "Run '" << prog << " --help' for detailed options.\n";
}

// Convert string to lowercase
static std::string to_lower(std::string s)
{                                                 // s: input string
    std::transform(s.begin(), s.end(), s.begin(), // Transform to lowercase
                   [](unsigned char c)
                   { return static_cast<char>(std::tolower(c)); });
    return s; // Return lowercase string
}

// Check if string ends with suffix (case-insensitive)
static bool ends_with_ci(const std::string &s, // s: input string
                         const std::string &suf)
{ // suf: suffix to check
    if (s.size() < suf.size())
        return false;                                             // Check size
    std::string tail = to_lower(s.substr(s.size() - suf.size())); // Get tail
    std::string needle = to_lower(suf);                           // Lowercase suffix
    return tail == needle;                                        // Compare strings
}

// Check if file has .fasta or .fasta.gz extension
static bool has_fasta_ext(const std::string &path)
{                                           // path: file path
    return ends_with_ci(path, ".fasta") ||  // Check .fasta extension
           ends_with_ci(path, ".fasta.gz"); // Check .fasta.gz extension
}

// Check if string resembles a file path
static bool looks_like_path(const std::string &s)
{                                               // s: input string
    return s.find('/') != std::string::npos ||  // Check for slashes
           s.find('\\') != std::string::npos || // Check for backslashes
           s.find('.') != std::string::npos;    // Check for dots
}

// Extract extension after last dot
static std::string ext_after_last_dot(const std::string &s)
{                                                                          // s: input string
    auto pos = s.rfind('.');                                               // Find last dot
    return (pos == std::string::npos) ? std::string{} : s.substr(pos + 1); // Return extension
}

// Read FASTA file into vector of (ID, sequence) pairs
static std::vector<std::pair<std::string, std::string>>
read_fasta(const std::string &filename)
{                               // filename: input file path
    std::ifstream in(filename); // Open input file
    if (!in)
    {                                                              // Check file opening
        throw std::runtime_error("Cannot open file: " + filename); // Throw error
    }

    std::vector<std::pair<std::string, std::string>> records; // Store records
    std::string line, id, seq;                                // Current line, ID, sequence

    auto flush = [&]() { // Flush current record
        if (!id.empty())
        {                                  // Check if ID exists
            records.emplace_back(id, seq); // Add record to vector
        }
        id.clear();  // Clear ID
        seq.clear(); // Clear sequence
    };

    while (std::getline(in, line))
    { // Read lines
        if (line.empty())
            continue; // Skip empty lines
        if (line[0] == '>')
        {                                                               // Check header line
            flush();                                                    // Flush previous record
            std::string rest = line.substr(1);                          // Extract header content
            size_t sp = rest.find_first_of(" \t");                      // Find first space/tab
            id = (sp == std::string::npos) ? rest : rest.substr(0, sp); // Extract ID
        }
        else
        { // Sequence line
            for (char c : line)
            { // Process characters
                if (!std::isspace(static_cast<unsigned char>(c)))
                {                     // Skip whitespace
                    seq.push_back(c); // Append to sequence
                }
            }
        }
    }
    flush();        // Flush final record
    return records; // Return records
}

// Search sequences in FASTA file for pattern
static std::vector<SearchTuple>
searchSequence(const std::string &filename, // filename: input file path
               const std::string &pattern)
{                                     // pattern: regex pattern
    std::regex rgx(pattern);          // Create regex object
    auto recs = read_fasta(filename); // Read FASTA records
    std::vector<SearchTuple> out;     // Store search results
    out.reserve(recs.size());         // Reserve space

    for (const auto &rec : recs)
    {                                                   // Iterate records
        const std::string &seq_id = rec.first;          // Get sequence ID
        const std::string &seq_str = rec.second;        // Get sequence
        bool matched = std::regex_search(seq_str, rgx); // Perform regex search
        out.emplace_back(seq_id, pattern, matched);     // Store result
    }
    return out; // Return results
}

// Structure to hold split arguments
struct SplitArgs
{
    std::vector<std::string> files_valid;             // Valid FASTA files
    std::vector<std::string> files_invalid_wrong_ext; // Files with wrong extension
    std::vector<std::string> files_invalid_missing;   // Missing FASTA files
    std::vector<std::string> patterns;                // Search patterns
};

// Split tokens into files and patterns
static SplitArgs split_tokens(const std::vector<std::string> &tokens)
{                                                              // tokens: input arguments
    SplitArgs out;                                             // Result structure
    std::unordered_set<std::string> seen_files, seen_patterns; // Track unique files, patterns

    for (const auto &t : tokens)
    { // Iterate tokens
        if (looks_like_path(t))
        { // Check if path-like
            if (!has_fasta_ext(t))
            {                                             // Check FASTA extension
                out.files_invalid_wrong_ext.push_back(t); // Add invalid extension
                continue;                                 // Skip to next token
            }
            if (fs::exists(t) && fs::is_regular_file(t))
            {                                     // Check file existence
                if (seen_files.insert(t).second)  // Check if unique
                    out.files_valid.push_back(t); // Add valid file
            }
            else
            {                                           // File missing
                out.files_invalid_missing.push_back(t); // Add missing file
            }
        }
        else
        {                                       // Treat as pattern
            if (seen_patterns.insert(t).second) // Check if unique
                out.patterns.push_back(t);      // Add pattern
        }
    }
    return out; // Return results
}

// Main program entry point
int main(int argc, char *argv[])
{                                                                                // argc, argv: command-line arguments
    const std::string prog = (argc > 0 ? std::string(argv[0]) : "FastaParser2"); // Program name

    argparse::ArgumentParser program("FastaParser2"); // Initialize argument parser
    program.add_description(                          // Set description
        "Regex search in FASTA files.\n"
        "--search accepts mixed files and patterns in any order.");
    auto &modes = program.add_mutually_exclusive_group(false); // Define mode group
    modes.add_argument("--search")                             // Add search mode
        .help("Search files for patterns; accepts mixed files and patterns.")
        .nargs(argparse::nargs_pattern::at_least_one); // Require ≥1 argument
    modes.add_argument("--summary")                    // Add summary mode
        .help("Summarize FASTA files (placeholder for Task 3).")
        .nargs(argparse::nargs_pattern::at_least_one); // Require ≥1 argument

    try
    {                                   // Parse arguments
        program.parse_args(argc, argv); // Process command-line
    }
    catch (const std::exception &e)
    {                                                 // Handle parse errors
        std::cerr << "Error: " << e.what() << "\n\n"; // Print error
        usage(prog);                                  // Show usage
        std::cout << program;                         // Show detailed help
        return 1;                                     // Exit with error
    }

    const bool mode_search = program.is_used("--search");   // Check search mode
    const bool mode_summary = program.is_used("--summary"); // Check summary mode

    if (!mode_search && !mode_summary)
    {                // No mode specified
        usage(prog); // Show usage
        return 0;    // Exit successfully
    }
    if (mode_search && mode_summary)
    {                                                                             // Both modes used
        std::cerr << "Error: --search and --summary are mutually exclusive.\n\n"; // Print error
        usage(prog);                                                              // Show usage
        return 1;                                                                 // Exit with error
    }

    try
    { // Process modes
        if (mode_search)
        {                                                                       // Handle search mode
            const auto raw = program.get<std::vector<std::string>>("--search"); // Get arguments
            const auto split = split_tokens(raw);                               // Split into files/patterns

            // Report invalid files
            if (!split.files_invalid_wrong_ext.empty())
            { // Check wrong extensions
                for (const auto &f : split.files_invalid_wrong_ext)
                { // Iterate invalid files
                    std::cerr << "Error: invalid FASTA file " << f
                              << " (.fasta or .fasta.gz required).\n"; // Print error
                    std::string ext = ext_after_last_dot(f);           // Get extension
                    std::cerr << "Extension: " << (ext.empty() ? "(none)" : "." + ext) << "\n";
                }
                std::cerr << "\n"; // Newline
                usage(prog);       // Show usage
                std::cerr << "\n"; // Newline
            }
            if (!split.files_invalid_missing.empty())
            {                                              // Check missing files
                std::cerr << "Warning: missing file(s): "; // Print warning
                for (size_t i = 0; i < split.files_invalid_missing.size(); ++i)
                { // Iterate files
                    if (i)
                        std::cerr << ", ";                                     // Add comma
                    std::cerr << "'" << split.files_invalid_missing[i] << "'"; // Print file
                }
                std::cerr << "\n\n"; // Newline
            }

            if (split.files_valid.empty())
            {             // No valid files
                return 1; // Exit with error
            }

            if (split.patterns.empty())
            {                                                                      // No patterns
                std::cerr << "Error: --search requires at least one pattern.\n\n"; // Print error
                usage(prog);                                                       // Show usage
                return 1;                                                          // Exit with error
            }

            for (const auto &vf : split.files_valid)
            {                                              // Report valid files
                std::cout << "Valid file: " << vf << "\n"; // Print valid file
            }

            // Search each file for each pattern using searchSequence
            for (const auto &file : split.files_valid)
            { // Iterate files
                for (const auto &pattern : split.patterns)
                {                                                 // Iterate patterns
                    auto results = searchSequence(file, pattern); // Perform search
                    for (const auto &result : results)
                    {                                                                  // Iterate results
                        std::cout << std::get<0>(result) << " "                        // Print sequence ID
                                  << std::get<1>(result) << " "                        // Print pattern
                                  << (std::get<2>(result) ? "true" : "false") << "\n"; // Print match
                    }
                }
            }
            return 0; // Exit successfully
        }

        if (mode_summary)
        {                                                                        // Handle summary mode
            const auto raw = program.get<std::vector<std::string>>("--summary"); // Get arguments
            const auto split = split_tokens(raw);                                // Split into files/patterns

            if (!split.patterns.empty())
            {                                                                  // Check for patterns
                std::cerr << "Error: --summary does not accept patterns.\n\n"; // Print error
                usage(prog);                                                   // Show usage
                return 1;                                                      // Exit with error
            }

            if (!split.files_invalid_wrong_ext.empty())
            { // Check wrong extensions
                for (const auto &f : split.files_invalid_wrong_ext)
                { // Iterate invalid files
                    std::cerr << "Error: invalid FASTA file " << f
                              << " (.fasta or .fasta.gz required).\n"; // Print error
                    std::string ext = ext_after_last_dot(f);           // Get extension
                    std::cerr << "Extension: " << (ext.empty() ? "(none)" : "." + ext) << "\n";
                }
                std::cerr << "\n"; // Newline
                usage(prog);       // Show usage
                std::cerr << "\n"; // Newline
            }
            if (!split.files_invalid_missing.empty())
            {                                              // Check missing files
                std::cerr << "Warning: missing file(s): "; // Print warning
                for (size_t i = 0; i < split.files_invalid_missing.size(); ++i)
                { // Iterate files
                    if (i)
                        std::cerr << ", ";                                     // Add comma
                    std::cerr << "'" << split.files_invalid_missing[i] << "'"; // Print file
                }
                std::cerr << "\n\n"; // Newline
            }

            if (split.files_valid.empty())
            {             // No valid files
                return 1; // Exit with error
            }

            for (const auto &f : split.files_valid)
            {                                                   // Summarize files
                std::cout << "Summary for file: " << f << "\n"; // Print placeholder
            }
            return 0; // Exit successfully
        }
    }
    catch (const std::exception &ex)
    {                                                // Handle runtime errors
        std::cerr << "Error: " << ex.what() << "\n"; // Print error
        return 1;                                    // Exit with error
    }

    return 0; // Exit successfully
}