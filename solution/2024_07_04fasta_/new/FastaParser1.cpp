// file2.cpp — FastaParser with custom usage (distinct from argparse --help)

#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <unordered_set>
#include <sstream>
#include <iterator>

namespace fs = std::filesystem;

// ---------------------------------------------------------------
// Custom quick-usage (distinct from argparse's --help)
// ---------------------------------------------------------------
static void usage(const std::string &progname)
{
    std::cout
        << "Usage (quick):\n"
        << "  " << progname << " --search <PATTERN... FILE...>\n"
        << "  " << progname << " --summary <FILE...>\n\n"
        << "Examples:\n"
        << "  " << progname << " --search sp|P59637|VEMP_CVHSA sequences.fasta\n"
        << "  " << progname << " --search sp|Q9H0H5|ABC_HUMAN seq1.fasta seq2.fasta\n"
        << "  " << progname << " --summary seq1.fasta seq2.fasta\n\n"
        << "Notes:\n"
        << "  • FILE must end with .fasta or .fasta.gz\n"
        << "  • PATTERN should be a UniProt-style ID like sp|P59637|VEMP_CVHSA\n"
        << "For detailed options, run: " << progname << " --help\n";
}

// ---------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------

static bool has_fasta_ext(const std::string &path)
{
    auto to_lower = [](std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c)
                       { return static_cast<char>(std::tolower(c)); });
        return s;
    };
    std::string lower = to_lower(path);

    return ((lower.size() >= 6 && lower.rfind(".fasta") == lower.size() - 6) ||
            (lower.size() >= 9 && lower.rfind(".fasta.gz") == lower.size() - 9));
}

// Heuristic: treat strings with a dot as file-like
static inline bool looks_like_file(const std::string &s)
{
    return s.find('.') != std::string::npos;
}

// UniProt FASTA header ID (e.g., sp|P59637|VEMP_CVHSA)
static inline bool is_valid_uniprot_pattern(const std::string &p)
{
    static const std::regex rx(R"(^[a-z]{2}\|[A-Z0-9]+\|[A-Z0-9_]+$)",
                               std::regex::icase);
    return std::regex_match(p, rx);
}

// ---------------------------------------------------------------
// Data structs
// ---------------------------------------------------------------
struct ParsedArgs
{
    std::vector<std::string> files;
    std::vector<std::string> patterns;
    std::vector<std::string> invalid_items;
};

// Split mixed arguments into files / patterns / invalids.
// - File: looks_like_file && has_fasta_ext
// - Pattern: not file-like && valid UniProt pattern
// - Otherwise: invalid
static ParsedArgs separate_files_and_patterns(const std::vector<std::string> &args)
{
    ParsedArgs out;
    std::unordered_set<std::string> seen;

    for (const auto &arg : args)
    {
        if (!seen.insert(arg).second)
            continue; // skip duplicates

        if (looks_like_file(arg))
        {
            if (has_fasta_ext(arg))
                out.files.push_back(arg);
            else
                out.invalid_items.push_back(arg);
        }
        else
        {
            if (is_valid_uniprot_pattern(arg))
                out.patterns.push_back(arg);
            else
                out.invalid_items.push_back(arg);
        }
    }
    return out;
}

// Validate existence of files; return those that exist, else print quick-usage
static std::vector<std::string> validate_file_existence(
    const std::vector<std::string> &files,
    const std::string &progname)
{
    std::vector<std::string> ok, missing;

    for (const auto &f : files)
    {
        try
        {
            if (fs::exists(f) && fs::is_regular_file(f))
                ok.push_back(f);
            else
                missing.push_back(f);
        }
        catch (const fs::filesystem_error &e)
        {
            std::cerr << "Warning: cannot access '" << f << "': " << e.what() << "\n";
            missing.push_back(f);
        }
    }

    if (!missing.empty())
    {
        std::ostringstream oss;
        oss << "File(s) not found: ";
        for (size_t i = 0; i < missing.size(); ++i)
        {
            if (i)
                oss << ", ";
            oss << "'" << missing[i] << "'";
        }
        std::cerr << "Error: " << oss.str() << "\n\n";
        usage(progname);
        return {};
    }

    return ok;
}

// Report invalid tokens using quick-usage style
static bool report_invalid_items(
    const std::vector<std::string> &invalid_items,
    const std::string &progname)
{
    if (invalid_items.empty())
        return false;

    std::ostringstream oss;
    oss << "Invalid argument(s): ";
    for (size_t i = 0; i < invalid_items.size(); ++i)
    {
        if (i)
            oss << ", ";
        oss << "'" << invalid_items[i] << "'";
        if (looks_like_file(invalid_items[i]) && !has_fasta_ext(invalid_items[i]))
        {
            oss << " (expected '.fasta' or '.fasta.gz')";
        }
        else if (!looks_like_file(invalid_items[i]))
        {
            oss << " (invalid UniProt pattern; e.g., sp|P59637|VEMP_CVHSA)";
        }
    }
    std::cerr << "Error: " << oss.str() << "\n\n";
    usage(progname);
    return true;
}

// ---------------------------------------------------------------
// Processing stubs
// ---------------------------------------------------------------
static void process_search(const std::vector<std::string> &patterns,
                           const std::vector<std::string> &files)
{
    std::cout << "Search Mode\n";
    std::cout << "Patterns (" << patterns.size() << "):\n";
    for (const auto &p : patterns)
        std::cout << "  - " << p << "\n";
    std::cout << "Files (" << files.size() << "):\n";
    for (const auto &f : files)
        std::cout << "  - " << f << "\n";
    // TODO: implement actual search
}

static void process_summary(const std::vector<std::string> &files)
{
    std::cout << "Summary Mode\n";
    std::cout << "Files (" << files.size() << "):\n";
    for (const auto &f : files)
        std::cout << "  - " << f << "\n";
    // TODO: implement actual summary
}

// ---------------------------------------------------------------
// Main
// ---------------------------------------------------------------
int main(int argc, char *argv[])
{
    const std::string progname = (argc > 0 ? std::string(argv[0]) : "FastaParser");

    argparse::ArgumentParser program("FastaParser", "1.0");
    program.add_description(
        "Elton's FastaParser - FASTA file processor\n"
        "Author: Elton Ugbogu, University of Potsdam, 2025");
    // NOTE: We rely on argparse's built-in -h/--help; we do NOT add our own --help

    // Mutually exclusive: --search OR --summary (required one)
    auto &mode = program.add_mutually_exclusive_group(true);
    mode.add_argument("--search")
        .help("Search for UniProt patterns in FASTA files")
        .nargs(argparse::nargs_pattern::at_least_one);
    mode.add_argument("--summary")
        .help("Summarize FASTA files")
        .nargs(argparse::nargs_pattern::at_least_one);

    // Parse args
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        // Let argparse show its error; then show our quick usage
        std::cerr << "Error: " << err.what() << "\n\n";
        usage(progname);
        return 1;
    }

    // If neither mode was given (shouldn’t happen with required group, but handle anyway)
    const bool mode_search = program.is_used("--search");
    const bool mode_summary = program.is_used("--summary");
    if (!mode_search && !mode_summary)
    {
        usage(progname);
        return 0;
    }
    if (mode_search && mode_summary)
    {
        std::cerr << "Error: choose either --search or --summary.\n\n";
        usage(progname);
        return 1;
    }

    // Gather raw args
    std::vector<std::string> raw = mode_search
                                       ? program.get<std::vector<std::string>>("--search")
                                       : program.get<std::vector<std::string>>("--summary");

    if (raw.empty())
    {
        std::cerr << "Error: missing arguments for selected mode.\n\n";
        usage(progname);
        return 1;
    }

    // Separate and validate
    auto parsed = separate_files_and_patterns(raw);

    if (report_invalid_items(parsed.invalid_items, progname))
    {
        return 1;
    }

    if (mode_search)
    {
        if (parsed.patterns.empty())
        {
            std::cerr << "Error: --search requires at least one UniProt pattern.\n\n";
            usage(progname);
            return 1;
        }
        if (parsed.files.empty())
        {
            std::cerr << "Error: --search requires at least one FASTA file.\n\n";
            usage(progname);
            return 1;
        }
    }
    else
    { // summary
        if (!parsed.patterns.empty())
        {
            std::cerr << "Error: --summary does not accept patterns.\n\n";
            usage(progname);
            return 1;
        }
        if (parsed.files.empty())
        {
            std::cerr << "Error: --summary requires at least one FASTA file.\n\n";
            usage(progname);
            return 1;
        }
    }

    auto valid_files = validate_file_existence(parsed.files, progname);
    if (valid_files.empty())
        return 1;

    // Execute
    if (mode_search)
        process_search(parsed.patterns, valid_files);
    else
        process_summary(valid_files);

    return 0;
}
