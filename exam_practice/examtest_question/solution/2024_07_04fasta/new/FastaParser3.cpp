// FastaParser3.cpp
#include <argparse/argparse.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <cctype>

namespace fs = std::filesystem;

struct SearchHit
{
    std::string id;
    std::string pattern;
    bool match;
};

struct LengthInfo
{
    std::string id;
    std::size_t length;
};

class FastaParser
{
public:
    static std::vector<std::pair<std::string, std::string>>
    parseFile(const std::string &filename)
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

    static std::vector<SearchHit>
    search(const std::string &filename, const std::string &pattern)
    {
        auto recs = parseFile(filename);
        std::vector<SearchHit> hits;
        hits.reserve(recs.size());
        std::regex rgx(pattern);
        for (auto &p : recs)
        {
            bool m = std::regex_search(p.second, rgx);
            hits.push_back({p.first, pattern, m});
        }
        return hits;
    }

    static std::vector<LengthInfo>
    summary(const std::string &filename)
    {
        auto recs = parseFile(filename);
        std::vector<LengthInfo> lens;
        lens.reserve(recs.size());
        for (auto &p : recs)
        {
            lens.push_back({p.first, p.second.size()});
        }
        return lens;
    }
};

static void print_banner(const std::string &prog)
{
    std::cout
        << "Elton Ugbogu FastaParser3 search FASTA files\n"
        << "Author: Elton Ugbogu, University of Potsdam, 2025\n"
        << "Usage: " << prog
        << " --search|--summary|--help ?PATTERN? file1.fasta [file2.fasta ...]\n\n"
        << "Modes:\n"
        << "  --search PATTERN file1 [file2 ...]   Regex search per file.\n"
        << "  --summary file1 [file2 ...]          List sequence ID lengths.\n"
        << "  --help                               Show help.\n\n"
        << "Search output:  id<TAB>pattern<TAB>true|false\n"
        << "Summary output: id<TAB>length\n";
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("FastaParser3");
    program.add_description("Multi-file FASTA search & summary");

    program.add_argument("--help")
        .help("Show help.")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("--search")
        .help("Regex pattern to search. Provide after this flag.")
        .nargs(1);

    program.add_argument("--summary")
        .help("Summary mode (no pattern).")
        .default_value(false)
        .implicit_value(true);

    // Remaining positional FASTA files (0..N)
    program.add_argument("files")
        .help("FASTA files (one or more).")
        .remaining();

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
        std::cerr << "Error: Specify one of --search or --summary.\n";
        return 1;
    }
    if (modes > 1)
    {
        std::cerr << "Error: --search and --summary cannot be used together.\n";
        return 1;
    }

    auto files = program.get<std::vector<std::string>>("files");
    if (files.empty())
    {
        std::cerr << "Error: Provide at least one FASTA file.\n";
        return 1;
    }

    // Filter existing files (warn about missing)
    std::vector<std::string> existing;
    for (auto &f : files)
    {
        if (!fs::exists(f))
        {
            std::cerr << "Warning: File '" << f << "' does not exist (skipping).\n";
        }
        else
        {
            existing.push_back(f);
        }
    }
    if (existing.empty())
    {
        std::cerr << "Error: No existing FASTA files to process.\n";
        return 1;
    }

    try
    {
        if (mode_search)
        {
            std::string pattern = program.get<std::vector<std::string>>("--search").front();
            for (auto &f : existing)
            {
                auto hits = FastaParser::search(f, pattern);
                for (auto &h : hits)
                {
                    std::cout << h.id << "\t" << h.pattern << "\t"
                              << (h.match ? "true" : "false") << "\n";
                }
            }
        }
        else
        { // summary
            for (auto &f : existing)
            {
                auto lens = FastaParser::summary(f);
                for (auto &li : lens)
                {
                    std::cout << li.id << "\t" << li.length << "\n";
                }
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
