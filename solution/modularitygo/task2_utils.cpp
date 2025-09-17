#include <filesystem>      // Include filesystem library
#include <fstream>         // Include file stream library
#include <string>          // Include string library
#include <vector>          // Include vector library
#include <regex>           // Include regex library
#include <zlib.h>          // Include zlib for gzip handling
#include "task2_utils.hpp" // Include custom utility header

namespace fs = std::filesystem; // Alias for filesystem namespace

std::vector<std::string> read_obo_lines(const std::string &filename)
{
    std::vector<std::string> lines; // Vector to store file lines
    char buffer[4096];              // Buffer for reading gzip file
    if (filename.size() >= 3 && filename.substr(filename.size() - 3) == ".gz")
    { // Check if file has .gz extension
        gzFile gzfile = gzopen(filename.c_str(), "rb");
        // Open gzip file for reading
        if (!gzfile) // Check if file opened successfully
        {
            throw std::runtime_error("Failed to open .gz file: " + filename);
            // Throw error if file opening fails
        }
        while (gzgets(gzfile, buffer, sizeof(buffer)))
        {                               // Read lines from gzip file
            lines.emplace_back(buffer); // Add line to vector
        }
        gzclose(gzfile); // Close gzip file
    }
    else
    {                                   // Handle non-gzip files
        std::ifstream infile(filename); // Open file stream
        if (!infile)                    // Check if file opened successfully
        {
            throw std::runtime_error("Failed to open file: " + filename);
            // Throw error if file opening fails
        }
        std::string line; // String to store each line
        while (std::getline(infile, line))
        {                          // Read lines from file
            lines.push_back(line); // Add line to vector
        }
    }
    return lines; // Return vector of lines
}

std::vector<std::vector<std::string>> process_consider_table(
    const std::vector<std::string> &files,
    const std::vector<std::string> &namespaces)
{
    std::vector<std::vector<std::string>> result;
    // Vector to store result pairs
    for (const auto &file : files) // Iterate over input files
    {
        auto lines = read_obo_lines(file);
        // Read lines from file
        bool in_term = false;          // Flag for [Term] section
        std::string current_id;        // Current term ID
        std::string current_namespace; // Current term namespace
        std::vector<std::string> consider_ids;
        // Vector for consider IDs
        bool is_obsolete = false;      // Flag for obsolete terms
        for (const auto &line : lines) // Iterate over lines
        {
            if (line == "[Term]\n" || line == "[Term]")
            {                       // Check for [Term] section start
                in_term = true;     // Set in_term flag
                current_id.clear(); // Clear current ID
                current_namespace.clear();
                // Clear current namespace
                consider_ids.clear(); // Clear consider IDs
                is_obsolete = false;  // Reset obsolete flag
                continue;             // Skip to next line
            }
            if (line.empty() && in_term)
            { // Check for end of term section
                if (is_obsolete && !current_id.empty())
                { // Process obsolete terms
                    if (namespaces.empty() ||
                        std::find(namespaces.begin(), namespaces.end(),
                                  current_namespace) != namespaces.end())
                    { // Check namespace filter
                        if (!consider_ids.empty())
                        { // Handle terms with consider IDs
                            for (const auto &alt_id : consider_ids)
                            {
                                result.push_back({current_id, alt_id});
                                // Add ID pair to result
                            }
                        }
                        else
                        { // Handle terms without consider IDs
                            result.push_back({current_id, "NA"});
                            // Add ID with NA to result
                        }
                    }
                }
                in_term = false; // Reset in_term flag
                continue;        // Skip to next line
            }
            if (!in_term) // Skip if not in term section
            {
                continue; // Skip to next line
            }
            if (line.rfind("id: ", 0) == 0)
            { // Check for ID line
                current_id = line.substr(4);
                // Extract ID
                current_id.erase(current_id.find_last_not_of(" \n\r\t") + 1);
                // Trim trailing whitespace
            }
            else if (line.rfind("namespace: ", 0) == 0)
            { // Check for namespace line
                current_namespace = line.substr(11);
                // Extract namespace
                current_namespace.erase(
                    current_namespace.find_last_not_of(" \n\r\t") + 1);
                // Trim trailing whitespace
            }
            else if (line.rfind("is_obsolete: true", 0) == 0)
            {                       // Check for obsolete flag
                is_obsolete = true; // Set obsolete flag
            }
            else if (line.rfind("consider: ", 0) == 0)
            { // Check for consider line
                auto alt = line.substr(9);
                // Extract consider ID
                alt.erase(alt.find_last_not_of(" \n\r\t") + 1);
                // Trim trailing whitespace
                consider_ids.push_back(alt);
                // Add to consider IDs
            }
        }
    }
    return result; // Return result pairs
}

std::vector<std::vector<std::string>> process_obsolete_stats(
    const std::vector<std::string> &files,
    const std::vector<std::string> &namespaces)
{
    std::vector<std::vector<std::string>> result;
    // Vector to store result pairs
    for (const auto &file : files) // Iterate over input files
    {
        auto lines = read_obo_lines(file);
        // Read lines from file
        bool in_term = false;          // Flag for [Term] section
        std::string current_id;        // Current term ID
        std::string current_namespace; // Current term namespace
        std::vector<std::string> consider_ids;
        // Vector for consider IDs
        bool is_obsolete = false;      // Flag for obsolete terms
        for (const auto &line : lines) // Iterate over lines
        {
            if (line == "[Term]\n" || line == "[Term]")
            {                       // Check for [Term] section start
                in_term = true;     // Set in_term flag
                current_id.clear(); // Clear current ID
                current_namespace.clear();
                // Clear current namespace
                consider_ids.clear(); // Clear consider IDs
                is_obsolete = false;  // Reset obsolete flag
                continue;             // Skip to next line
            }
            if (line.empty() && in_term)
            { // Check for end of term section
                if (is_obsolete && !current_id.empty())
                { // Process obsolete terms
                    if (namespaces.empty() ||
                        std::find(namespaces.begin(), namespaces.end(),
                                  current_namespace) != namespaces.end())
                    { // Check namespace filter
                        if (!consider_ids.empty())
                        { // Handle terms with consider IDs
                            for (const auto &alt_id : consider_ids)
                            {
                                result.push_back({current_id, alt_id});
                                // Add ID pair to result
                            }
                        }
                        else
                        { // Handle terms without consider IDs
                            result.push_back({current_id, "NA"});
                            // Add ID with NA to result
                        }
                    }
                }
                in_term = false; // Reset in_term flag
                continue;        // Skip to next line
            }
            if (!in_term) // Skip if not in term section
            {
                continue; // Skip to next line
            }
            if (line.rfind("id: ", 0) == 0)
            { // Check for ID line
                current_id = line.substr(4);
                // Extract ID
                current_id.erase(current_id.find_last_not_of(" \n\r\t") + 1);
                // Trim trailing whitespace
            }
            else if (line.rfind("namespace: ", 0) == 0)
            { // Check for namespace line
                current_namespace = line.substr(11);
                // Extract namespace
                current_namespace.erase(
                    current_namespace.find_last_not_of(" \n\r\t") + 1);
                // Trim trailing whitespace
            }
            else if (line.rfind("is_obsolete: true", 0) == 0)
            {                       // Check for obsolete flag
                is_obsolete = true; // Set obsolete flag
            }
            else if (line.rfind("consider: ", 0) == 0)
            { // Check for consider line
                auto alt = line.substr(9);
                // Extract consider ID
                alt.erase(alt.find_last_not_of(" \n\r\t") + 1);
                // Trim trailing whitespace
                consider_ids.push_back(alt);
                // Add to consider IDs
            }
        }
    }
    return result; // Return result pairs
}