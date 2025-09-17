// FastaParser2.cpp
#include <argparse/argparse.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

namespace fs = std::filesystem;

// Result tuple: (sequence_id, pattern, matched?)
using SearchTuple = std::tuple<std::string, std::string, bool>;

static void print_banner(const std::string &prog)
{
    std::cout
        << "Detlef's FastaParser2 search FASTA files\n"
        << "Author: Detlef Groth, University of Potsdam, 2020\n"
        << "Usage: " << prog << " --search|--summary|--help ?PATTERN? file.fasta\n\n"
        << "Modes:\n"
        << "  --search PATTERN file.fasta  Search (regex) for PATTERN in sequences.\n"
        << "  --summary file.fasta         (Task 3) Print sequence lengths.\n"
        << "  --help                       Show this help.\n";
}

static std::vector<std::pair<std::string, std::string>>
read_fasta(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in)
        throw std::runtime_error("Cannot open file: " + filename);

    std::vector<std::pair<std::string, std::string>> records;
    std::string line, id, seq;

    auto flush = [&]()
    {
        if (!id.empty())
        {
            records.emplace_back(id, seq);
        }
        id.clear();
        seq.clear();
    };

    while (std::getline(in, line))
    {
        if (line.empty())
            continue;
        if (line[0] == '>')
        {
            flush();
            std::string rest = line.substr(1);
            size_t sp = rest.find_first_of(" \t");
            id = (sp == std::string::npos) ? rest : rest.substr(0, sp);
        }
        else
        {
            for (char c : line)
            {
                if (!std::isspace(static_cast<unsigned char>(c)))
                    seq.push_back(c);
            }
        }
    }
    flush();
    return records;
}

static std::vector<SearchTuple>
searchSequence(const std::string &filename, const std::string &pattern)
{
    auto recs = read_fasta(filename);
    std::vector<SearchTuple> out;
    out.reserve(recs.size());
    std::regex rgx(pattern);
    for (auto &p : recs)
    {
        bool m = std::regex_search(p.second, rgx);
        out.emplace_back(p.first, pattern, m);
    }
    return out;
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("FastaParser2");
    program.add_description("Regex search in a FASTA file.");

    program.add_argument("--help")
        .help("Show help.")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("--search")
        .help("Regex pattern to search (requires file).")
        .nargs(1);

    program.add_argument("--summary")
        .help("Summary mode (implemented in Task 3).")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("file")
        .help("FASTA file.")
        .nargs(argparse::nargs_pattern::optional);

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n\n";
        print_banner(argv[0]);
        std::cerr << program;
        return 1;
    }

    if (program.get<bool>("--help"))
    {
        print_banner(argv[0]);
        std::cout << program;
        return 0;
    }

    bool mode_search = program.is_used("--search");
    bool mode_summary = program.get<bool>("--summary");
    int modes = (int)mode_search + (int)mode_summary;

    if (modes == 0)
    {
        std::cerr << "Error: Must specify one of --search, --summary or --help.\n";
        return 1;
    }
    if (modes > 1)
    {
        std::cerr << "Error: --search and --summary are mutually exclusive.\n";
        return 1;
    }

    std::string file = program.is_used("file") ? program.get<std::string>("file") : "";

    try
    {
        if (mode_search)
        {
            if (file.empty())
            {
                std::cerr << "Error: --search requires a FASTA file.\n";
                return 1;
            }
            auto pat_vec = program.get<std::vector<std::string>>("--search");
            if (pat_vec.empty())
            {
                std::cerr << "Error: --search requires a pattern.\n";
                return 1;
            }
            std::string pattern = pat_vec.front();
            if (!fs::exists(file))
            {
                std::cerr << "Warning: File '" << file << "' does not exist.\n";
            }
            auto results = searchSequence(file, pattern);
            for (auto &row : results)
            {
                std::cout << std::get<0>(row) << "\t"
                          << std::get<1>(row) << "\t"
                          << (std::get<2>(row) ? "true" : "false") << "\n";
            }
            return 0;
        }

        if (mode_summary)
        {
            if (file.empty())
            {
                std::cerr << "Error: --summary requires a FASTA file.\n";
                return 1;
            }
            if (!fs::exists(file))
            {
                std::cerr << "Warning: File '" << file << "' does not exist.\n";
            }
            std::cout << "(Task 2) Summary mode placeholder – implemented in Task 3.\n";
            return 0;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
