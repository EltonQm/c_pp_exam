#include <iostream>   // for input/output streams
#include <fstream>    // for file input streams
#include <string>     // for std::string
#include <vector>     // for std::vector
#include <regex>      // for regular expressions
#include <filesystem> // for checking file existence

// Class for parsing FASTA files
class FastaParser
{
public:
    // Function to summarize sequence lengths in one or more FASTA files
    static void summary(const std::vector<std::string> &files)
    {
        for (const auto &file : files)
        {
            std::ifstream infile(file);
            if (!infile.is_open())
            {
                std::cerr << "Cannot open file: " << file << "\n";
                continue; // Skip to next file if this one can't be opened
            }

            std::string line, id, sequence;

            while (std::getline(infile, line))
            {
                if (line.empty())
                    continue; // Skip empty lines

                if (line[0] == '>')
                {
                    // If a sequence was accumulated previously, output its ID and length
                    if (!id.empty())
                    {
                        std::cout << id << " " << sequence.length() << "\n";
                    }

                    // Extract ID from line (exclude '>' and stop at first space if present)
                    auto space_pos = line.find(' ');
                    if (space_pos != std::string::npos)
                        id = line.substr(1, space_pos - 1);
                    else
                        id = line.substr(1);

                    sequence.clear(); // Reset sequence string
                }
                else
                {
                    sequence += line; // Append sequence line
                }
            }

            // Handle the last sequence in the file
            if (!id.empty())
            {
                std::cout << id << " " << sequence.length() << "\n";
            }
        }
    }

    // Function to perform regex search on sequences in one or more FASTA files
    static void search(const std::vector<std::string> &files, const std::string &pattern)
    {
        std::regex re(pattern); // Compile the regex pattern

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
                    // If a sequence was accumulated previously, apply regex search
                    if (!id.empty())
                    {
                        bool matched = std::regex_search(sequence, re);
                        std::cout << id << " " << pattern << " " << (matched ? "true" : "false") << "\n";
                    }

                    // Extract ID (same logic as in summary)
                    auto space_pos = line.find(' ');
                    if (space_pos != std::string::npos)
                        id = line.substr(1, space_pos - 1);
                    else
                        id = line.substr(1);

                    sequence.clear();
                }
                else
                {
                    sequence += line;
                }
            }

            // Final match check for the last sequence
            if (!id.empty())
            {
                bool matched = std::regex_search(sequence, re);
                std::cout << id << " " << pattern << " " << (matched ? "true" : "false") << "\n";
            }
        }
    }
};

// Prints usage/help instructions
void printHelp()
{
    std::cout << "FastaParser3 - search and summarize FASTA sequences\n"
              << "Usage:\n"
              << "  ./task3 --summary file1.fasta file2.fasta [...]\n"
              << "  ./task3 --search PATTERN file1.fasta file2.fasta [...]\n";
}

int main(int argc, char *argv[])
{
    // Check for minimum required arguments
    if (argc < 3)
    {
        printHelp();
        return 1;
    }

    std::string option = argv[1]; // Extract operation: --summary or --search

    // Handle summary mode
    if (option == "--summary")
    {
        std::vector<std::string> files;
        for (int i = 2; i < argc; ++i)
        {
            if (!std::filesystem::exists(argv[i]))
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

        std::string pattern = argv[2]; // Extract regex pattern
        std::vector<std::string> files;

        for (int i = 3; i < argc; ++i)
        {
            if (!std::filesystem::exists(argv[i]))
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
        // Handle unknown option
        std::cerr << "Unknown option: " << option << "\n";
        printHelp();
        return 1;
    }

    return 0; // Successful execution
}
