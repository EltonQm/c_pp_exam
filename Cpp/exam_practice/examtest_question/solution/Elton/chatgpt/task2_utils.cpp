#include "task2_utils.hpp"
// Include standard input/output for error messages and file operations
#include <iostream>
// Include file stream for reading uncompressed .obo files
#include <fstream>
// Include string for handling text lines and namespaces
#include <string>
// Include vector for storing lines and results
#include <vector>
// Include filesystem for checking file extensions
#include <filesystem>
// Include zlib for reading compressed .obo.gz files
#include <zlib.h>
#include <stdexcept>
#include <algorithm>

// Function: considerTable
// Purpose: Parses a vector of OBO file lines to extract obsolete terms and their consider IDs,
// filtered by an optional namespace. Returns a table of GO IDs, namespaces, and consider IDs.
std::vector<std::vector<std::string>>
considerTable(const std::vector<std::string> &lines, const std::string &ns)
{
    // Initialize a vector to store the result table, where each row contains [GO_ID, namespace, consider_IDs]
    std::vector<std::vector<std::string>> results;
    // Initialize a vector to collect lines within a single [Term] block
    std::vector<std::string> block;
    // Iterate through each line in the input lines vector
    for (const auto &line : lines)
    {
        // Check if the line is empty, indicating the end of a [Term] block
        if (line.empty())
        {
            // Initialize variables to store GO ID, namespace, and concatenated consider IDs
            std::string go_id, namespace_value, consider_ids;
            // Process each line in the current block
            for (const auto &l : block)
            {
                // Check if the line starts with "id: GO:" to extract the GO ID
                if (l.rfind("id: GO:", 0) == 0)
                    // Extract the GO ID by removing the "id: " prefix (4 characters)
                    go_id = l.substr(4);
                // Check if the line is "is_obsolete: true" (no action needed, just confirm obsolescence)
                else if (l.rfind("is_obsolete: true", 0) == 0)
                {
                    // No action needed; presence of this line is checked implicitly
                }
                // Check if the line starts with "namespace:" to extract the namespace
                else if (l.rfind("namespace:", 0) == 0)
                    // Extract the namespace by removing the "namespace: " prefix (10 characters)
                    namespace_value = l.substr(10);
                // Check if the line starts with "consider: GO:" to extract consider IDs
                else if (l.rfind("consider: GO:", 0) == 0)
                    // Append the consider ID (remove "consider: " prefix, 9 characters)
                    // Concatenate with a comma if consider_ids is not empty
                    consider_ids += (consider_ids.empty() ? "" : ",") + l.substr(9);
            }
            // Check if the term has a GO ID and matches the namespace filter (if provided)
            if (!go_id.empty() && (!ns.empty() ? namespace_value == ns : true))
            {
                // Add a row to results with GO ID, namespace, and consider IDs (or "NA" if none)
                results.push_back({go_id, namespace_value, consider_ids.empty() ? "NA" : consider_ids});
            }
            // Clear the block for the next [Term]
            block.clear();
        }
        else
            // Add non-empty lines to the current block
            block.push_back(line);
    }
    // Return the table of obsolete terms with their consider IDs
    return results;
}

// Function: replacedByTable
// Purpose: Parses a vector of OBO file lines to extract obsolete terms and their replaced_by IDs,
// filtered by an optional namespace. Returns a table of GO IDs, namespaces, and replaced_by IDs.
std::vector<std::vector<std::string>> replacedByTable(const std::vector<std::string> &lines, const std::string &ns)
{
    // Initialize a vector to store the result table, where each row contains [GO_ID, namespace, replaced_by_IDs]
    std::vector<std::vector<std::string>> results;
    // Initialize a vector to collect lines within a single [Term] block
    std::vector<std::string> block;
    // Iterate through each line in the input lines vector
    for (const auto &line : lines)
    {
        // Check if the line is empty, indicating the end of a [Term] block
        if (line.empty())
        {
            // Initialize variables to store GO ID, namespace, and concatenated replaced_by IDs
            std::string go_id, namespace_value, replaced_by_ids;
            // Process each line in the current block
            for (const auto &l : block)
            {
                // Check if the line starts with "id: GO:" to extract the GO ID
                if (l.rfind("id: GO:", 0) == 0)
                    // Extract the GO ID by removing the "id: " prefix (4 characters)
                    go_id = l.substr(4);
                // Check if the line starts with "namespace:" to extract the namespace
                else if (l.rfind("namespace:", 0) == 0)
                    // Extract the namespace by removing the "namespace: " prefix (10 characters)
                    namespace_value = l.substr(10);
                // Check if the line starts with "replaced_by: GO:" to extract replaced_by IDs
                else if (l.rfind("replaced_by: GO:", 0) == 0)
                    // Append the replaced_by ID (remove "replaced_by: " prefix, 14 characters)
                    // Concatenate with a comma if replaced_by_ids is not empty
                    replaced_by_ids += (replaced_by_ids.empty() ? "" : ",") + l.substr(14);
            }
            // Check if the term has a GO ID and matches the namespace filter (if provided)
            if (!go_id.empty() && (!ns.empty() ? namespace_value == ns : true))
            {
                // Add a row to results with GO ID, namespace, and replaced_by IDs (or "NA" if none)
                results.push_back({go_id, namespace_value, replaced_by_ids.empty() ? "NA" : replaced_by_ids});
            }
            // Clear the block for the next [Term]
            block.clear();
        }
        else
            // Add non-empty lines to the current block
            block.push_back(line);
    }
    // Return the table of obsolete terms with their replaced_by IDs
    return results;
}

// Function: read_obo_lines
// Purpose: Reads all lines from a .obo or .obo.gz file into a vector of strings,
// supporting both compressed and uncompressed files.
std::vector<std::string> read_obo_lines(const std::string &filename)
{
    // Initialize a vector to store all lines from the file
    std::vector<std::string> lines;
    // Define a fixed-size buffer for reading compressed files
    char buffer[4096];

    // Check if the filename ends with ".gz" to determine if it's compressed
    if (filename.size() >= 3 && filename.substr(filename.size() - 3) == ".gz")
    {
        // Open the compressed file in binary read mode using zlib
        gzFile gzfile = gzopen(filename.c_str(), "rb");
        // Check if the file was opened successfully
        if (!gzfile)
        {
            // Throw an exception if the file cannot be opened
            throw std::runtime_error("Failed to open gzipped file: " + filename);
        }

        // Read lines from the compressed file using gzgets
        while (gzgets(gzfile, buffer, sizeof(buffer)))
        {
            // Add each line to the lines vector, converting buffer to std::string
            lines.emplace_back(buffer);
        }

        // Close the compressed file
        gzclose(gzfile);
    }
    else
    {
        // Open the uncompressed file using std::ifstream
        std::ifstream infile(filename);
        // Check if the file was opened successfully
        if (!infile.is_open())
        {
            // Throw an exception if the file cannot be opened
            throw std::runtime_error("Failed to open file: " + filename);
        }

        // Read lines from the uncompressed file using std::getline
        std::string line;
        while (std::getline(infile, line))
        {
            // Add each line to the lines vector
            lines.push_back(line);
        }
    }

    // Return the vector of all lines read from the file
    return lines;
}
