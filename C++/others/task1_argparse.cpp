#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <regex>
#include "argparse/argparse.hpp"

namespace fs = std::filesystem;

bool file_exists(const std::string &path)
{
    return fs::exists(path);
}

void print_usage()
{
    std::cout << "Ugbogu Elton FastaParser1 - search FASTA files\n"
              << "Author: Ugbogu Elton, University of Potsdam, 2024\n\n"
              << "Usage:\n"
              << "  ./task --search PATTERN file.fasta [file2.fasta ...]\n"
              << "  ./task --summary file.fasta\n"
              << "  ./task --help\n\n"
              << "Arguments:\n"
              << "  --help     Show this help message\n"
              << "  --summary  Print lengths of all sequences\n"
              << "  --search   Search for pattern in sequence IDs\n";
}

void search_fasta(const std::string &pattern, const std::vector<std::string> &files)
{
    for (const auto &file : files)
    {
        if (!file_exists(file))
        {
            std::cerr << "Error: File does not exist → " << file << "\n";
            continue;
        }

        std::ifstream input(file);
        std::string line;
        bool match = false;

        while (std::getline(input, line))
        {
            if (!line.empty() && line[0] == '>')
            {
                match = line.find(pattern) != std::string::npos;
                if (match)
                    std::cout << line << "\n";
            }
            else if (match)
            {
                std::cout << line << "\n";
            }
        }
    }
}

void summarize_fasta(const std::string &file)
{
    if (!file_exists(file))
    {
        std::cerr << "Error: File does not exist → " << file << "\n";
        return;
    }

    std::ifstream input(file);
    std::string line, current_id;
    size_t length = 0;

    while (std::getline(input, line))
    {
        if (!line.empty() && line[0] == '>')
        {
            if (!current_id.empty())
            {
                std::cout << current_id << ": " << length << " aa\n";
            }
            current_id = line.substr(1);
            length = 0;
        }
        else
        {
            length += line.size();
        }
    }
    if (!current_id.empty())
    {
        std::cout << current_id << ": " << length << " aa\n";
    }
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("FastaParser1");

    program.add_argument("--search")
        .help("Search for pattern in sequence IDs")
        .remaining();

    program.add_argument("--summary")
        .help("Print lengths of all sequences")
        .nargs(1)
        .default_value<std::vector<std::string>>({});

    program.add_argument("--help")
        .default_value(false)
        .implicit_value(true)
        .nargs(0)
        .help("Show this help message");

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
        const auto &summary_args = program.get<std::vector<std::string>>("--summary");

        if (!search_args.empty() && summary_args.empty())
        {
            std::string pattern = search_args[0];
            std::vector<std::string> files(search_args.begin() + 1, search_args.end());

            if (pattern.empty() || std::regex_match(pattern, std::regex(".*\\.fasta$")))
            {
                std::cerr << "Error: Pattern is missing or mistaken for a filename.\n";
                print_usage();
                return 1;
            }

            if (files.empty())
            {
                std::cerr << "Error: Missing filename(s) for --search.\n";
                print_usage();
                return 1;
            }

            for (const auto &file : files)
            {
                fs::path filepath = file;
                if (!fs::exists(filepath))
                {
                    std::cerr << "Error: File does not exist → " << file << "\n";
                    return 1;
                }
                std::cout << "File found: " << file << "\n";
            }
            std::cout << files.size() << " files provided:\n";

            search_fasta(pattern, files);
        }
        else if (!summary_args.empty() && search_args.empty())
        {
            if (summary_args.size() > 1)
            {
                std::cerr << "Error: Maximum number of positional arguments exceeded\n";
                print_usage();
                return 1;
            }

            summarize_fasta(summary_args[0]);
        }
        else if (search_args.empty() && summary_args.empty())
        {
            std::cerr << "Error: No valid option provided.\n";
            print_usage();
            return 1;
        }
        else
        {
            std::cerr << "Error: Options --search and --summary cannot be used together.\n";
            print_usage();
            return 1;
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
