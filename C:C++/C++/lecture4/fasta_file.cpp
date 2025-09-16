#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>

namespace fs = std::filesystem;

// usage function
void usage()
{
    std::cout << "Usage: fastafile Pattern\n";
}

// receive a FASTA file regular expression as input

// Function to find matching sequence IDs based on regex pattern
std::vector<std::string> find_matching_ids(const std::string &filename, const std::string &pattern)
{
    // open the file in read mode

    // opens file here
    // This line does open the file (this is perfectly valid and idiomatic in C++) —
    // it combines construction and opening in one line
    std::ifstream infile(filename);

    /*
    more verbose
    std::ifstream infile;
    infile.open(filename);
    */

    // Check if the file opened successfully:
    if (!infile.is_open())
    {
        std::cerr << "Error: could not open file.\n";
        return {};
    }

    std::regex rx(pattern, std::regex::ECMAScript);
    std::vector<std::string> result;

    std::string line;
    std::string current_id;
    std::string current_sequence;
    std::unordered_set<std::string> unique_ids;

    while (std::getline(infile, line))
    {
        if (line.rfind(">", 0) == 0) // Start of new sequence
        {
            current_id = line.substr(1); // Remove ">"
            current_sequence.clear();
            // Check previous sequence before overwriting
            if (!current_id.empty() && std::regex_search(current_sequence, rx))
            {
                result.push_back(current_id);
            }
        }
        else
        {
            current_sequence += line;
        }
    }

    // Check the final sequence
    if (std::regex_search(current_sequence, rx))
    {
        if (unique_ids.insert(current_id).second)
        {
            result.push_back(current_id);
        }
    }

    infile.close();
    return result;
}

int main(int argc, char **argv)
{
    // Get input from the user
    std::string filename; // declare the filename
    std::cout << "Enter the path to the fasta file: ";
    std::getline(std::cin, filename);

    // Enter the pattern
    std::string pattern;
    std::cout << "Enter the regex pattern: ";
    std::getline(std::cin, pattern);

    // check if the fileexist
    if (!fs::exists(filename))
    {
        std::cout << "Given file does not exist\n";
        return 1;
    }

    // Search and print results
    auto res = find_matching_ids(filename, pattern);

    if (res.empty())
    {
        std::cout << "No matching sequences found.\n";
        usage();
    }
    else
    {
        std::cout << "\n Matching sequence IDs:\n";
        for (const auto &id : res)
        {
            std::cout << "-" << id << "\n";
        }
    }
}