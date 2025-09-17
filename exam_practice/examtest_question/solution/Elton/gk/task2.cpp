#include <iostream>              // For std::cout, std::cerr
#include <string>                // For std::string
#include <vector>                // For std::vector
#include <filesystem>            // For checking file existence and extension
#include <zlib.h>                // For reading .gz files with gzopen, gzgets, gzclose
#include "argparse/argparse.hpp" // External header-only library for parsing command-line arguments

// Parses an OBO or OBO.GZ file to extract obsolete GO terms and their alternative GO IDs.
// If `namespace_filter` is specified, filters by namespace. If `use_replaced_by` is true,
// it looks for 'replaced_by' instead of 'consider' alternatives.
std::vector<std::vector<std::string>> parseTable(
    const std::string &filename,
    const std::string &namespace_filter,
    bool use_replaced_by)

{
    std::vector<std::vector<std::string>> result; // Stores output rows: {GO:ID, alternative1, alternative2, ...}
    gzFile file = gzopen(filename.c_str(), "rb"); // Open gzip file for reading
    if (!file)
    {
        std::cerr << "Error: Cannot open file '" << filename << "'\n";
        return result; // Return empty result if file opening fails
    }

    std::string line;                 // Current line read
    char buffer[1024];                // Buffer for gzgets
    std::string current_id;           // Holds current GO term ID
    std::string current_namespace;    // Holds current term's namespace
    std::vector<std::string> alt_ids; // List of 'consider:' or 'replaced_by:' GO IDs
    bool is_obsolete = false;         // Flag to track if current term is obsolete

    while (gzgets(file, buffer, sizeof(buffer)))
    {
        line = buffer;                                 // Convert C-string to C++ string
        line.erase(line.find_last_not_of("\n\r") + 1); // Trim newline characters

        if (line == "[Term]") // Start of a new GO term block
        {
            // If previous term is obsolete and matches the namespace, store it
            if (is_obsolete && (namespace_filter.empty() || namespace_filter == current_namespace))
            {
                if (alt_ids.empty())
                    alt_ids.push_back("NA"); // No alternative ID found

                result.push_back({current_id});                                            // Start row with GO:ID
                result.back().insert(result.back().end(), alt_ids.begin(), alt_ids.end()); // Append alternatives
            }

            // Reset for next term block
            current_id.clear();
            current_namespace.clear();
            is_obsolete = false;
            alt_ids.clear();
        }

        else if (line.starts_with("id: "))
        {
            current_id = line.substr(4); // Extract GO term ID
        }
        else if (line.starts_with("namespace: "))
        {
            current_namespace = line.substr(11); // Extract namespace
        }
        else if (line == "is_obsolete: true")
        {
            is_obsolete = true; // Mark term as obsolete
        }
        else if (line.starts_with(use_replaced_by ? "replaced_by: " : "consider: "))
        {
            // Extract alternative GO ID depending on the flag
            alt_ids.push_back(line.substr(use_replaced_by ? 12 : 10));
        }
    }

    // Handle last term in file if it was obsolete
    if (is_obsolete && (namespace_filter.empty() || namespace_filter == current_namespace))
    {
        if (alt_ids.empty())
            alt_ids.push_back("NA");
        result.push_back({current_id});
        result.back().insert(result.back().end(), alt_ids.begin(), alt_ids.end());
    }

    gzclose(file); // Close file handle
    return result; // Return parsed table
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("task2");
    program.add_argument("option")
        .help("Command-line option")
        .default_value(std::string(""))
        .choices({"--consider-table", "--replaced-by"});
    program.add_argument("filename")
        .help("Input .obo or .obo.gz file");
    program.add_argument("namespace")
        .help("Optional namespace (e.g., molecular_function, cellular_component, biological_process)")
        .nargs(argparse::nargs_pattern::optional)
        .default_value(std::string(""));

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program << std::endl;
        return 1;
    }

    std::string option = program.get<std::string>("option");
    std::string filename = program.get<std::string>("filename");
    std::string namespace_filter = program.get<std::string>("namespace");

    if (!std::filesystem::exists(filename) || (!filename.ends_with(".obo") && !filename.ends_with(".obo.gz")))
    {
        std::cerr << "Error: Invalid or non-existent file '" << filename << "'\n";
        std::cerr << program << std::endl;
        return 1;
    }

    bool use_replaced_by = (option == "--replaced-by");
    if (option == "--consider-table" || option == "--replaced-by")
    {
        auto table = parseTable(filename, namespace_filter, use_replaced_by);
        for (const auto &row : table)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                std::cout << row[i];
                if (i < row.size() - 1)
                    std::cout << "\t";
            }
            std::cout << "\n";
        }
    }

    return 0;
}