#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <tuple>
#include <string>
#include <filesystem>

// Use filesystem namespace for file existence check
namespace fs = std::filesystem;

// Print usage/help message for incorrect or missing arguments
void printHelp()
{
    std::cout << "Ugbogu Elton FastaParser2 - regex search in FASTA sequences\n"
              << "Usage: FastaParser2 --search PATTERN file.fasta\n"
              << "Example: ./FastaParser2 --search MAT sars-cov1.fasta\n";
}

/*
 * Function to search for a pattern in FASTA sequences
 * @param filename: path to the FASTA file
 * @param pattern: regular expression pattern to search
 * @return vector of tuples (id, pattern, match status)
 */
std::vector<std::tuple<std::string, std::string, bool>>
searchSequence(const std::string &filename, const std::string &pattern)
{
    std::ifstream infile(filename);
    if (!infile)
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;       // Holds current line from file
    std::string id;         // Holds current FASTA header (sequence ID)
    std::string sequence;   // Accumulates sequence lines
    std::regex re(pattern); // Compiled regex pattern
    std::vector<std::tuple<std::string, std::string, bool>> results;

    while (std::getline(infile, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '>')
        {
            // If there's an existing ID, process and store it before moving to new one
            if (!id.empty())
            {
                bool match = std::regex_search(sequence, re);
                results.emplace_back(id, pattern, match);
            }

            // Extract ID (remove '>' and anything after space)
            auto space_pos = line.find(' ');
            id = (space_pos != std::string::npos) ? line.substr(1, space_pos - 1)
                                                  : line.substr(1);
            sequence.clear(); // Reset for new sequence
        }
        else
        {
            // Accumulate sequence lines
            sequence += line;
        }
    }

    // Final check: make sure the last sequence is also processed
    if (!id.empty())
    {
        bool match = std::regex_search(sequence, re);
        results.emplace_back(id, pattern, match);
    }

    return results;
}

int main(int argc, char *argv[])
{
    // Expect exactly 4 arguments: program, --search, pattern, filename
    if (argc != 4 || std::string(argv[1]) != "--search")
    {
        printHelp();
        return 1;
    }

    std::string pattern = argv[2];
    std::string filename = argv[3];

    // Check if file exists using C++17 filesystem
    if (!fs::exists(filename))
    {
        std::cerr << "Error: File not found → " << filename << "\n";
        return 1;
    }

    try
    {
        // Perform regex search and print results
        auto results = searchSequence(filename, pattern);
        for (const auto &[id, pat, matched] : results)
        {
            std::cout << id << " " << pat << " " << (matched ? "true" : "false") << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Search failed: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
