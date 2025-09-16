#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>

#include "argparse/argparse.hpp"

namespace fs = std::filesystem;

class FastaParser
{
public:
    static void summary(const std::vector<std::string> &files)
    {
        for (const auto &file : files)
        {
            std::ifstream infile(file);
            if (!infile.is_open())
            {
                std::cerr << "Cannot open file: " << file << "\n";
                continue;
            }

            std::string line, id, sequence;
            while (std::getline(infile, line))
            {
                if (line.empty())
                    continue;
                if (line[0] == '>')
                {
                    if (!id.empty())
                    {
                        std::cout << id << " " << sequence.length() << "\n";
                    }
                    auto space_pos = line.find(' ');
                    id = (space_pos != std::string::npos) ? line.substr(1, space_pos - 1) : line.substr(1);
                    sequence.clear();
                }
                else
                {
                    sequence += line;
                }
            }
            if (!id.empty())
            {
                std::cout << id << " " << sequence.length() << "\n";
            }
        }
    }

    static void search(const std::vector<std::string> &files, const std::string &pattern)
    {
        std::regex re(pattern);
        for (const auto &file : files)
        {
            std::ifstream infile(file);
            if (!infile.is_open())
            {
                std::cerr << "Cannot open file: " << file << "\n";
                continue;
            }

            std::string line, id, sequence;
            while (std::getline(infile, line))
            {
                if (line.empty())
                    continue;
                if (line[0] == '>')
                {
                    if (!id.empty())
                    {
                        bool matched = std::regex_search(sequence, re);
                        std::cout << id << " " << pattern << " " << (matched ? "true" : "false") << "\n";
                    }
                    auto space_pos = line.find(' ');
                    id = (space_pos != std::string::npos) ? line.substr(1, space_pos - 1) : line.substr(1);
                    sequence.clear();
                }
                else
                {
                    sequence += line;
                }
            }
            if (!id.empty())
            {
                bool matched = std::regex_search(sequence, re);
                std::cout << id << " " << pattern << " " << (matched ? "true" : "false") << "\n";
            }
        }
    }
};

void print_help()
{
    std::cout << "FastaParser3 - search and summarize FASTA sequences\n"
              << "Usage:\n"
              << "  ./task3 --summary file1.fasta [file2.fasta ...]\n"
              << "  ./task3 --search PATTERN file1.fasta [file2.fasta ...]\n";
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("FastaParser3");

    program.add_argument("--summary")
        .help("Summarize sequence lengths in FASTA files")
        .nargs(argparse::nargs_pattern::any);

    program.add_argument("--search")
        .help("Search regex pattern in FASTA sequences")
        .nargs(argparse::nargs_pattern::any);

    program.add_argument("--help")
        .default_value(false)
        .implicit_value(true)
        .nargs(0)
        .help("Show this help message");

    try
    {
        auto unknown_args = program.parse_known_args(argc, argv);

        if (!unknown_args.empty())
        {
            std::cerr << "Error: Unknown argument(s): ";
            for (const auto &arg : unknown_args)
                std::cerr << arg << " ";
            std::cerr << "\n";
            print_help();
            return 1;
        }

        if (program.get<bool>("--help"))
        {
            print_help();
            return 0;
        }

        const auto &summary_args = program.get<std::vector<std::string>>("--summary");
        const auto &search_args = program.get<std::vector<std::string>>("--search");

        if (!summary_args.empty() && search_args.empty())
        {
            std::vector<std::string> files;
            for (const auto &file : summary_args)
            {
                if (!fs::exists(file))
                {
                    std::cerr << "File not found: " << file << "\n";
                }
                else
                {
                    files.push_back(file);
                }
            }
            if (files.empty())
            {
                std::cerr << "No valid FASTA files provided.\n";
                return 1;
            }
            FastaParser::summary(files);
        }
        else if (!search_args.empty() && summary_args.empty())
        {
            if (search_args.size() < 2)
            {
                std::cerr << "Error: --search requires a pattern and at least one FASTA file\n";
                print_help();
                return 1;
            }
            std::string pattern = search_args[0];
            std::vector<std::string> files(search_args.begin() + 1, search_args.end());

            std::vector<std::string> valid_files;

            for (const auto &file : files)
            {
                if (!fs::exists(file))
                {
                    std::cerr << "File not found: " << file << "\n";
                }
                else
                {
                    valid_files.push_back(file);
                }
            }

            if (valid_files.empty())
            {
                std::cerr << "No valid FASTA files provided.\n";
                return 1;
            }

            FastaParser::search(valid_files, pattern);
        }
        else
        {
            std::cerr << "Error: Provide either --summary or --search, not both.\n";
            print_help();
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        print_help();
        return 1;
    }

    return 0;
}
