// task_utils.cpp — Provides utility functions to parse GO OBO files (.obo or .obo.gz)

#include "task_utils.hpp" // Function declarations
#include <iostream>       // For std::cout, std::cerr
#include <fstream>        // For reading plain .obo files
#include <zlib.h>         // For reading compressed .gz files
#include <regex>          // (Unused — can be removed)
#include <cstring>        // For C string manipulation
#include <stdexcept>      // For throwing exceptions

// Reads all lines from a .obo or .obo.gz file and returns them as a vector of strings.
// Supports both plain text and gzipped files using zlib.
std::vector<std::string> read_obo_lines(const std::string &filename)
{
    std::vector<std::string> lines; // Vector to store the file lines
    char buffer[4096];              // Temporary buffer for reading lines from gzipped file

    // If file ends in ".gz", open it as a gzip file
    if (filename.size() >= 3 && filename.substr(filename.size() - 3) == ".gz")
    {
        gzFile gzfile = gzopen(filename.c_str(), "rb"); // Open in binary read mode
        if (!gzfile)
            throw std::runtime_error("Failed to open .gz file"); // Throw error if file can't be opened

        // Read file line by line and add to vector
        while (gzgets(gzfile, buffer, sizeof(buffer)))
            lines.emplace_back(buffer);

        gzclose(gzfile); // Close gzip file handle
    }
    else // For plain .obo files
    {
        std::ifstream infile(filename); // Open as standard input file stream
        if (!infile)
            throw std::runtime_error("Failed to open file");

        std::string line;
        while (std::getline(infile, line)) // Read line-by-line
            lines.push_back(line);
    }

    return lines;
}

// Parses GO term blocks and prints those that have a MetaCyc xref matching a given ID set.
// Displays GO ID, name, namespace, and MetaCyc ID.
void parse_terms_with_metacyc_lines(
    const std::vector<std::string> &lines,
    const std::unordered_set<std::string> &metacyc_ids)
{
    std::vector<std::string> current_term; // Accumulates lines for each term block

    for (const auto &line : lines)
    {
        if (line.empty()) // End of a GO term block
        {
            std::string go_id, name, ns, metacyc;
            for (const auto &l : current_term)
            {
                if (l.rfind("id: GO:", 0) == 0) // Extract GO ID
                    go_id = l.substr(4);
                else if (l.rfind("name:", 0) == 0) // Extract name
                    name = l.substr(6);
                else if (l.rfind("namespace:", 0) == 0) // Extract namespace
                    ns = l.substr(10);
                else if (l.rfind("xref:", 0) == 0) // Check for MetaCyc xref
                {
                    size_t pos = l.find("MetaCyc:");
                    if (pos != std::string::npos)
                    {
                        std::string id = l.substr(pos + 8);
                        id.erase(std::remove(id.begin(), id.end(), '\n'), id.end());
                        if (metacyc_ids.count(id)) // If matched MetaCyc ID
                            metacyc = "MetaCyc:" + id;
                    }
                }
            }

            // Print term only if both GO ID and MetaCyc ID are found
            if (!go_id.empty() && !metacyc.empty())
                std::cout << go_id << " " << name << " " << ns << " " << metacyc << "\n";

            current_term.clear(); // Clear buffer for next term
        }
        else
        {
            current_term.push_back(line); // Accumulate term lines
        }
    }
}

// Counts total number of GO terms and how many are marked as obsolete.
// Prints a summary to stdout.
void count_obsolete_terms(const std::vector<std::string> &lines)
{
    int total = 0, obsolete = 0; // Counters for total and obsolete terms
    bool in_term = false;        // Flag to track if we're inside a term block

    for (const auto &line : lines)
    {
        if (line == "[Term]" || line == "[Term]\n") // Start of a new term
        {
            ++total;
            in_term = true;
        }
        else if (in_term && (line.rfind("is_obsolete: true", 0) == 0)) // Obsolete marker
        {
            ++obsolete;
            in_term = false; // Counted, don't double-count
        }
        else if (line.empty()) // End of term block
        {
            in_term = false;
        }
    }

    // Print the stats
    std::cout << "Total terms: " << total << "\n";
    std::cout << "Obsolete terms: " << obsolete << "\n";
    std::cout << "Non-obsolete terms: " << total - obsolete << "\n";
}

// Prints all GO terms' ID and name in a simple tab-separated format.
void print_go_id_and_name(const std::vector<std::string> &lines)
{
    std::vector<std::string> current_term;

    for (const auto &line : lines)
    {
        if (line.empty()) // End of term
        {
            std::string go_id, name;
            for (const auto &l : current_term)
            {
                if (l.rfind("id: GO:", 0) == 0)
                    go_id = l.substr(4); // Extract GO ID
                else if (l.rfind("name:", 0) == 0)
                    name = l.substr(6); // Extract term name
            }

            if (!go_id.empty()) // If a valid GO ID was found
                std::cout << go_id << "\t" << name << "\n";

            current_term.clear(); // Prepare for next term
        }
        else
        {
            current_term.push_back(line); // Accumulate lines for the current term
        }
    }
}

/*
 filename.size() >= 3
 → Make sure the filename is at least 3 characters long (to avoid out-of-bounds when slicing).
 filename.substr(filename.size() - 3)
 → Take the last 3 characters of the filename.
 == ".gz"
 → Check if those last 3 characters are exactly .gz
 */
