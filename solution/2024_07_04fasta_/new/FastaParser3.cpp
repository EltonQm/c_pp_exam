#include <argparse/argparse.hpp> // Command-line argument parsing
#include <filesystem>            // File system operations
#include <fstream>               // File stream operations
#include <iostream>              // Console input/output
#include <regex>                 // Regular expression matching
#include <stdexcept>             // Exception handling
#include <string>                // String operations
#include <tuple>                 // Tuple data structure
#include <vector>                // Dynamic arrays
#include <cctype>                // Character classification
#include <unordered_set>         // Hash set for deduplication
#include <algorithm>             // String transformations
#include <sstream>               // String stream operations

namespace fs = std::filesystem; // Alias for filesystem namespace

// Structure to store search result information
struct SearchHit
{                        // Search result data
    std::string id;      // Sequence ID
    std::string pattern; // Search pattern
    bool match;          // Match result
};

// Structure to store sequence length information
struct LengthInfo
{                       // Sequence length data
    std::string id;     // Sequence ID
    std::size_t length; // Sequence length
};

// Class for parsing and processing FASTA files
class FastaParser
{ // FASTA file processor
public:
    // Parse FASTA file into ID-sequence pairs
    static std::vector<std::pair<std::string, std::string>>
    parseFile(const std::string &filename)
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
                records.emplace_back(id, seq); // Add record
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
                size_t sp = rest.find_first_of(" \t");                      // Find space/tab
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

    // Search sequences for a pattern
    static std::vector<SearchHit>
    search(const std::string &filename, // filename: input file path
           const std::string &pattern)
    {                                    // pattern: regex pattern
        auto recs = parseFile(filename); // Parse FASTA file
        std::vector<SearchHit> hits;     // Store search results
        hits.reserve(recs.size());       // Reserve space
        std::regex rgx(pattern);         // Create regex object
        for (auto &p : recs)
        {                                              // Iterate records
            bool m = std::regex_search(p.second, rgx); // Perform regex search
            hits.push_back({p.first, pattern, m});     // Store result
        }
        return hits; // Return results
    }

    // Summarize sequence lengths
    static std::vector<LengthInfo>
    summary(const std::string &filename)
    {                                    // filename: input file path
        auto recs = parseFile(filename); // Parse FASTA file
        std::vector<LengthInfo> lens;    // Store length info
        lens.reserve(recs.size());       // Reserve space
        for (auto &p : recs)
        {                                               // Iterate records
            lens.push_back({p.first, p.second.size()}); // Store ID and length
        }
        return lens; // Return results
    }
};

// Display program banner
static void print_banner(const std::string &prog)
{ // prog: program name
    std::cout << "Elton Ugbogu FastaParser3 search FASTA files\n"
              << "Author: Elton Ugbogu, University of Potsdam, 2025\n";
}

// Display usage instructions
static void usage_quick(const std::string &prog)
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
              << "  - Patterns are regular expressions; quote patterns with '|'.\n"
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

// Structure to hold classified input tokens
struct Split
{                                                     // Token classification
    std::vector<std::string> files_valid;             // Valid FASTA files
    std::vector<std::string> files_invalid_wrong_ext; // Files with wrong extension
    std::vector<std::string> files_invalid_missing;   // Missing FASTA files
    std::vector<std::string> patterns;                // Search patterns
};

// Classify tokens into files and patterns
static Split split_tokens(const std::vector<std::string> &tokens)
{                                                              // tokens: input arguments
    Split out;                                                 // Result structure
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
{                                                                                // argc, argv: command-line
    const std::string prog = (argc > 0 ? std::string(argv[0]) : "FastaParser3"); // Program name

    argparse::ArgumentParser program("FastaParser3");                                              // Initialize parser
    program.add_description("Multi-file FASTA search & summary (position-independent arguments)"); // Set description

    auto &modes = program.add_mutually_exclusive_group(false);                // Define mode group
    modes.add_argument("--search")                                            // Add search mode
        .help("Search files for patterns; accepts mixed files and patterns.") // Help text
        .nargs(argparse::nargs_pattern::at_least_one);                        // Require ≥1 argument
    modes.add_argument("--summary")                                           // Add summary mode
        .help("Summarize sequence lengths for files.")                        // Help text
        .nargs(argparse::nargs_pattern::at_least_one);                        // Require ≥1 argument

    try
    {                                   // Parse arguments
        program.parse_args(argc, argv); // Process command-line
    }
    catch (const std::exception &e)
    {                                                 // Handle parse errors
        std::cerr << "Error: " << e.what() << "\n\n"; // Output error
        print_banner(prog);                           // Show banner
        usage_quick(prog);                            // Show usage
        std::cout << program;                         // Show help
        return 1;                                     // Exit with error
    }

    const bool mode_search = program.is_used("--search");   // Check search mode
    const bool mode_summary = program.is_used("--summary"); // Check summary mode
    if (!mode_search && !mode_summary)
    {                       // No mode specified
        print_banner(prog); // Show banner
        usage_quick(prog);  // Show usage
        return 0;           // Exit successfully
    }
    if (mode_search && mode_summary)
    {                                                                              // Both modes used
        std::cerr << "Error: --search and --summary cannot be used together.\n\n"; // Output error
        usage_quick(prog);                                                         // Show usage
        return 1;                                                                  // Exit with error
    }

    try
    { // Process modes
        if (mode_search)
        {                                                                       // Handle search mode
            const auto raw = program.get<std::vector<std::string>>("--search"); // Get arguments
            const auto split = split_tokens(raw);                               // Split into files/patterns

            if (!split.files_invalid_wrong_ext.empty())
            { // Check wrong extensions
                for (const auto &f : split.files_invalid_wrong_ext)
                { // Iterate invalid files
                    std::cerr << "Error: invalid FASTA file " << f
                              << " (.fasta or .fasta.gz required).\n"; // Output error
                    const std::string ext = ext_after_last_dot(f);     // Get extension
                    std::cerr << "Extension: " << (ext.empty() ? "(none)" : "." + ext) << "\n";
                }
                std::cerr << "\n"; // Newline
                usage_quick(prog); // Show usage
                std::cerr << "\n"; // Newline
            }
            if (!split.files_invalid_missing.empty())
            {                                              // Check missing files
                std::cerr << "Warning: missing file(s): "; // Output warning
                for (size_t i = 0; i < split.files_invalid_missing.size(); ++i)
                { // Iterate files
                    if (i)
                        std::cerr << ", ";                                     // Add comma
                    std::cerr << "'" << split.files_invalid_missing[i] << "'"; // Output file
                }
                std::cerr << "\n\n"; // Newline
            }

            if (split.files_valid.empty())
            {             // No valid files
                return 1; // Exit with error
            }
            if (split.patterns.empty())
            {                                                                      // No patterns
                std::cerr << "Error: --search requires at least one pattern.\n\n"; // Output error
                usage_quick(prog);                                                 // Show usage
                return 1;                                                          // Exit with error
            }

            for (const auto &vf : split.files_valid)
            {                                              // Report valid files
                std::cout << "Valid file: " << vf << "\n"; // Output valid file
            }

            for (const auto &file : split.files_valid)
            {                                             // Iterate files
                auto recs = FastaParser::parseFile(file); // Parse file
                for (const auto &pattern : split.patterns)
                {                            // Iterate patterns
                    std::regex rgx(pattern); // Create regex
                    for (const auto &rec : recs)
                    {                                                      // Iterate records
                        const std::string &sid = rec.first;                // Get sequence ID
                        const std::string &sseq = rec.second;              // Get sequence
                        const bool matched = std::regex_search(sseq, rgx); // Search
                        std::cout << sid << " " << pattern << " "
                                  << (matched ? "true" : "false") << "\n"; // Output result
                    }
                }
            }
        }
        else
        {                                                                        // Handle summary mode
            const auto raw = program.get<std::vector<std::string>>("--summary"); // Get arguments
            const auto split = split_tokens(raw);                                // Split into files/patterns

            if (!split.patterns.empty())
            {                                                                  // Check for patterns
                std::cerr << "Error: --summary does not accept patterns.\n\n"; // Output error
                usage_quick(prog);                                             // Show usage
                return 1;                                                      // Exit with error
            }
            if (!split.files_invalid_wrong_ext.empty())
            { // Check wrong extensions
                for (const auto &f : split.files_invalid_wrong_ext)
                { // Iterate invalid files
                    std::cerr << "Error: invalid FASTA file " << f
                              << " (.fasta or .fasta.gz required).\n"; // Output error
                    const std::string ext = ext_after_last_dot(f);     // Get extension
                    std::cerr << "Extension: " << (ext.empty() ? "(none)" : "." + ext) << "\n";
                }
                std::cerr << "\n"; // Newline
                usage_quick(prog); // Show usage
                std::cerr << "\n"; // Newline
            }
            if (!split.files_invalid_missing.empty())
            {                                              // Check missing files
                std::cerr << "Warning: missing file(s): "; // Output warning
                for (size_t i = 0; i < split.files_invalid_missing.size(); ++i)
                { // Iterate files
                    if (i)
                        std::cerr << ", ";                                     // Add comma
                    std::cerr << "'" << split.files_invalid_missing[i] << "'"; // Output file
                }
                std::cerr << "\n\n"; // Newline
            }
            if (split.files_valid.empty())
            {             // No valid files
                return 1; // Exit with error
            }

            for (const auto &f : split.files_valid)
            {                                        // Iterate files
                auto lens = FastaParser::summary(f); // Get lengths
                for (auto &li : lens)
                {                                                   // Iterate results
                    std::cout << li.id << " " << li.length << "\n"; // Output result
                }
            }
        }
    }
    catch (const std::exception &ex)
    {                                                // Handle runtime errors
        std::cerr << "Error: " << ex.what() << "\n"; // Output error
        return 1;                                    // Exit with error
    }

    return 0; // Exit successfully
}