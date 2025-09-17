#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <tuple>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// Class to encapsulate FASTA search and summary functionalities
class FastaParser
{
public:
    // Print lengths of sequences from one or more FASTA files
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
                    // Print ID and length of completed sequence
                    if (!id.empty())
                    {
                        std::cout << id << " " << sequence.length() << "\n";
                    }

                    // Extract ID up to first space (omit full description)
                    auto space_pos = line.find(' ');
                    id = (space_pos != std::string::npos) ? line.substr(1, space_pos - 1) : line.substr(1);
                    sequence.clear();
                }
                else
                {
                    sequence += line;
                }
            }

            // Print last sequence
            if (!id.empty())
            {
                std::cout << id << " " << sequence.length() << "\n";
            }
        }
    }

    // Perform regex search on sequences from multiple FASTA files
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
                    // Apply regex to previous sequence
                    if (!id.empty())
                    {
                        bool matched = std::regex_search(sequence, re);
                        std::cout << id << " " << pattern << " " << (matched ? "true" : "false") << "\n";
                    }

                    // Extract ID without description
                    auto space_pos = line.find(' ');
                    id = (space_pos != std::string::npos) ? line.substr(1, space_pos - 1) : line.substr(1);
                    sequence.clear();
                }
                else
                {
                    sequence += line;
                }
            }

            // Apply regex to last sequence
            if (!id.empty())
            {
                bool matched = std::regex_search(sequence, re);
                std::cout << id << " " << pattern << " " << (matched ? "true" : "false") << "\n";
            }
        }
    }
};

// Print usage help for the user
void printHelp()
{
    std::cout << "FastaParser3 - search and summarize FASTA sequences\n"
              << "Usage:\n"
              << "  ./task3 --summary file1.fasta file2.fasta [...]\n"
              << "  ./task3 --search PATTERN file1.fasta file2.fasta [...]\n";
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printHelp();
        return 1;
    }

    std::string option = argv[1];

    // Handle summary mode
    if (option == "--summary")
    {
        std::vector<std::string> files;
        for (int i = 2; i < argc; ++i)
        {
            if (!fs::exists(argv[i]))
            {
                std::cerr << "File not found: " << argv[i] << "\n";
                continue;
            }
            files.push_back(argv[i]);
        }

        if (files.empty())
        {
            std::cerr << "No valid FASTA files provided.\n";
            return 1;
        }

        FastaParser::summary(files);
    }
    // Handle search mode
    else if (option == "--search")
    {
        if (argc < 4)
        {
            std::cerr << "Error: Missing search pattern or file(s).\n";
            printHelp();
            return 1;
        }

        std::string pattern = argv[2];
        std::vector<std::string> files;
        for (int i = 3; i < argc; ++i)
        {
            if (!fs::exists(argv[i]))
            {
                std::cerr << "File not found: " << argv[i] << "\n";
                continue;
            }
            files.push_back(argv[i]);
        }

        if (files.empty())
        {
            std::cerr << "No valid FASTA files provided.\n";
            return 1;
        }

        FastaParser::search(files, pattern);
    }
    else
    {
        std::cerr << "Unknown option: " << option << "\n";
        printHelp();
        return 1;
    }

    return 0;
}
