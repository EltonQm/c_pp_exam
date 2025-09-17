#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <tuple>
#include <string>
#include <filesystem>

#include "argparse/argparse.hpp"

namespace fs = std::filesystem;

void print_usage()
{
    std::cout << "Ugbogu Elton FastaParser2 - regex search in FASTA sequences\n"
              << "Usage:\n"
              << "  ./task --search PATTERN file.fasta\n"
              << "Example:\n"
              << "  ./task --search MAT sars-cov1.fasta\n";
}

bool file_exists(const std::string &path)
{
    return fs::exists(path);
}

std::vector<std::tuple<std::string, std::string, bool>> searchSequence(
    const std::string &filename,
    const std::string &pattern)
{
    std::ifstream infile(filename);
    if (!infile)
        throw std::runtime_error("Could not open file: " + filename);

    std::string line, id, sequence;
    std::vector<std::tuple<std::string, std::string, bool>> results;
    std::regex re(pattern);

    while (std::getline(infile, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '>')
        {
            if (!id.empty())
            {
                bool match = std::regex_search(sequence, re);
                results.emplace_back(id, pattern, match);
            }
            id = line.substr(1);
            sequence.clear();
        }
        else
        {
            sequence += line;
        }
    }

    if (!id.empty())
    {
        bool match = std::regex_search(sequence, re);
        results.emplace_back(id, pattern, match);
    }

    return results;
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("FastaParser2");

    program.add_argument("--search")
        .help("Search for regex pattern in FASTA sequence content")
        .remaining();

    program.add_argument("--help")
        .default_value(false)
        .implicit_value(true)
        .nargs(0)
        .help("Show help message");

    try
    {
        std::vector<std::string> unknown_args = program.parse_known_args(argc, argv);

        if (!unknown_args.empty())
        {
            std::cerr << "Error: Unknown argument(s): ";
            for (const auto &arg : unknown_args)
                std::cerr << arg << " ";
            std::cerr << "\n";
            print_usage();
            return 1;
        }

        if (program.get<bool>("--help"))
        {
            print_usage();
            return 0;
        }

        const auto &search_args = program.get<std::vector<std::string>>("--search");

        if (search_args.size() != 2)
        {
            std::cerr << "Error: --search requires exactly 2 arguments: PATTERN and FILENAME\n";
            print_usage();
            return 1;
        }

        std::string pattern = search_args[0];
        std::string filename = search_args[1];

        if (!file_exists(filename))
        {
            std::cerr << "Error: File not found → " << filename << "\n";
            return 1;
        }

        auto results = searchSequence(filename, pattern);
        for (const auto &[id, pat, matched] : results)
        {
            std::cout << id << " " << pat << " " << (matched ? "true" : "false") << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        print_usage();
        return 1;
    }

    return 0;
}
