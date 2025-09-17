#include <iostream>   // For input/output operations
#include <fstream>    // For reading from files
#include <regex>      // For regex-based searching
#include <vector>     // For using std::vector container
#include <tuple>      // For using std::tuple to store results
#include <string>     // For std::string manipulation
#include <filesystem> // For checking file existence

namespace fs = std::filesystem; // Alias for easier filesystem access

// Function to print usage/help information to the console
void printHelp()
{
    std::cout << "Ugbogu Elton FastaParser2 - regex search in FASTA sequences\n"
              << "Usage: FastaParser2 --search PATTERN file.fasta\n"
              << "Example: ./FastaParser2 --search MAT sars-cov1.fasta\n";
}

// Function to search for a regex pattern within sequences from a FASTA file
// Returns a vector of tuples: (sequence ID, pattern, match result)
std::vector<std::tuple<std::string, std::string, bool>> searchSequence(const std::string &filename, const std::string &pattern)
{
    std::ifstream infile(filename); // Open the FASTA file
    if (!infile)
    {
        // If file can't be opened, throw an error
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line, id, sequence;
    std::vector<std::tuple<std::string, std::string, bool>> results;
    std::regex re(pattern); // Compile regex pattern

    // Read the file line by line
    while (std::getline(infile, line))
    {
        if (line.empty())
            continue; // Skip empty lines

        if (line[0] == '>')
        {
            // If there's a previous sequence, check for regex match and store result
            if (!id.empty())
            {
                bool match = std::regex_search(sequence, re);
                results.emplace_back(id, pattern, match);
            }
            id = line.substr(1); // Store new sequence ID (remove '>')
            sequence.clear();    // Reset sequence for next entry
        }
        else
        {
            sequence += line; // Accumulate sequence lines
        }
    }

    // Handle last sequence in file
    if (!id.empty())
    {
        bool match = std::regex_search(sequence, re);
        results.emplace_back(id, pattern, match);
    }

    return results; // Return vector of results
}

int main(int argc, char *argv[])
{
    // Check for valid number of arguments and correct usage
    if (argc != 4 || std::string(argv[1]) != "--search")
    {
        printHelp(); // Print help message if usage is incorrect
        return 1;
    }

    std::string pattern = argv[2];  // Extract regex pattern
    std::string filename = argv[3]; // Extract FASTA filename

    // Check that the input file actually exists
    if (!fs::exists(filename))
    {
        std::cerr << "Error: File not found → " << filename << "\n";
        return 1;
    }

    try
    {
        // Perform the regex search
        auto results = searchSequence(filename, pattern);

        // Print the results (sequence ID, pattern, true/false)
        for (const auto &[id, pat, matched] : results)
        {
            std::cout << id << " " << pat << " " << (matched ? "true" : "false") << "\n";
        }
    }
    catch (const std::exception &e)
    {
        // Handle any error thrown during search
        std::cerr << "Search failed: " << e.what() << "\n";
        return 1;
    }

    return 0; // Successful execution
}
