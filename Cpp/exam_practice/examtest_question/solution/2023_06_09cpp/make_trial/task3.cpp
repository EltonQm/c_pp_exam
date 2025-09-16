#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <tuple>
#include <stdexcept>
#include <argparse/argparse.hpp>
#include <zlib.h>

namespace fs = std::filesystem;

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <zlib.h> // for gzopen, gzgets, gzclose

// Function to read lines from one or more .obo or .obo.gz files
std::vector<std::string> read_obo_file(const std::vector<std::string> &filenames)
{
    // Container to hold all lines from all files
    std::vector<std::string> lines;

    // Buffer for reading gzipped lines
    char buffer[4096];

    // Loop through each file in the input list
    for (const auto &filename : filenames)
    {
        // Check for gzip-compressed files (.gz extension)
        if (filename.size() >= 3 && filename.substr(filename.size() - 3) == ".gz")
        {
            // Open gzipped file in binary read mode
            gzFile gzfile = gzopen(filename.c_str(), "rb");
            if (!gzfile)
                throw std::runtime_error("Could not open compressed file: " + filename);

            // Read each line using gzgets and store it in the lines vector
            while (gzgets(gzfile, buffer, sizeof(buffer)))
            {
                lines.emplace_back(buffer);
            }

            // Close the gzipped file
            gzclose(gzfile);
        }
        else
        {
            // Open plain text file
            std::ifstream infile(filename);
            if (!infile)
                throw std::runtime_error("Could not open file: " + filename);

            std::string line;
            while (std::getline(infile, line))
            {
                lines.push_back(line);
            }
        }
    }

    // Return all accumulated lines
    return lines;
}

// Parse the OBO file and return a vector of term blocks (each block is a vector of lines)
std::vector<std::vector<std::string>> extract_terms(const std::vector<std::string> &lines)
{
    std::vector<std::vector<std::string>> terms;
    std::vector<std::string> current_term;
    bool in_term = false;

    for (const auto &line : lines)
    {
        if (line == "[Term]\n" || line == "[Term]")
        {
            if (!current_term.empty())
            {
                terms.push_back(current_term);
                current_term.clear();
            }
            in_term = true;
        }
        else if (in_term)
        {
            if (line.empty() || line == "\n")
            {
                if (!current_term.empty())
                {
                    terms.push_back(current_term);
                    current_term.clear();
                }
                in_term = false;
            }
            else
            {
                current_term.push_back(line);
            }
        }
    }
    if (!current_term.empty())
    {
        terms.push_back(current_term);
    }
    return terms;
}

// Extract GO, EC, and MetaCyc IDs for all terms in TSV format
void print_metacyc_table(const std::vector<std::vector<std::string>> &terms)
{
    std::regex ec_pattern(R"(EC:(\d+\.\d+\.\d+\.\d+))");
    for (const auto &term : terms)
    {
        std::string go_id, ec_number = "NA", metacyc_id;
        for (const auto &line : term)
        {
            if (line.rfind("id:", 0) == 0)
            {
                go_id = line.substr(4);
            }
            else if (line.rfind("xref:", 0) == 0)
            {
                if (line.find("MetaCyc:") != std::string::npos)
                {
                    metacyc_id = line.substr(line.find("MetaCyc:") + 8);
                    metacyc_id.erase(std::remove(metacyc_id.begin(), metacyc_id.end(), '\n'), metacyc_id.end());
                }
                if (line.find("EC:") != std::string::npos)
                {
                    std::smatch match;
                    if (std::regex_search(line, match, ec_pattern))
                    {
                        ec_number = match[0].str();
                    }
                }
            }
        }
        if (!go_id.empty() && !metacyc_id.empty())
        {
            std::cout << go_id << "\t" << ec_number << "\t" << metacyc_id << "\n";
        }
    }
}
// Function: search_metacyc
// Purpose: Search through one or more .obo files for GO terms that reference
//          any MetaCyc ID in the provided set, and print matching entries.
// Output Format: GO_ID <tab> term name <tab> namespace <tab> MetaCyc entry
void search_metacyc(const std::unordered_set<std::string> &metacyc_ids,
                    const std::vector<std::string> &filenames)
{
    // (Optional) Define a regex to match EC numbers (not used here, but could be reused)
    std::regex ec_pattern(R"(EC:(\d+\.\d+\.\d+\.\d+))");

    // Loop over each input file (can be .obo or .obo.gz — only .obo handled here)
    for (const auto &filename : filenames)
    {
        // Open the file using an input file stream
        std::ifstream infile(filename);

        // Check if the file was successfully opened
        if (!infile.is_open())
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
            continue; // skip to the next file if opening fails
        }

        // Vector to hold lines for one GO term block (delimited by blank lines)
        std::vector<std::string> current_term;
        std::string line;

        // Read the file line-by-line
        while (std::getline(infile, line))
        {
            if (line.empty())
            {
                // Reached the end of a GO term block, time to parse and process it

                // Variables to store components of the GO term
                std::string go_id;         // e.g. GO:0008800
                std::string name;          // e.g. beta-lactamase activity
                std::string ns;            // namespace, e.g. molecular_function
                std::string metacyc_entry; // full MetaCyc entry (MetaCyc:BETA-LACTAMASE-RXN)

                // Loop through each line of the term block
                for (const auto &term_line : current_term)
                {
                    // Extract GO ID
                    if (term_line.rfind("id: GO:", 0) == 0)
                    {
                        go_id = term_line.substr(4); // Remove "id: " (length 4)
                    }
                    // Extract name (GO term name)
                    else if (term_line.rfind("name:", 0) == 0)
                    {
                        name = term_line.substr(6); // Remove "name: " (length 6)
                    }
                    // Extract namespace (e.g. molecular_function, biological_process)
                    else if (term_line.rfind("namespace:", 0) == 0)
                    {
                        ns = term_line.substr(10); // Remove "namespace: " (length 10)
                    }
                    // Check if xref line contains a MetaCyc reference
                    else if (term_line.rfind("xref:", 0) == 0)
                    {
                        size_t pos = term_line.find("MetaCyc:");
                        if (pos != std::string::npos)
                        {
                            // Extract just the MetaCyc ID (e.g., BETA-LACTAMASE-RXN)
                            std::string metacyc_id = term_line.substr(pos + 8);

                            // Remove any trailing newline characters
                            metacyc_id.erase(std::remove(metacyc_id.begin(), metacyc_id.end(), '\n'), metacyc_id.end());

                            // If this ID is one of the user-provided IDs, store it
                            if (metacyc_ids.count(metacyc_id))
                            {
                                metacyc_entry = "MetaCyc:" + metacyc_id;
                            }
                        }
                    }
                }

                // Only print if both GO ID and MetaCyc entry are found and valid
                if (!go_id.empty() && !metacyc_entry.empty())
                {
                    std::cout << go_id << " " << name << " " << ns << " " << metacyc_entry << "\n";
                }

                // Clear the term buffer for the next GO term block
                current_term.clear();
            }
            else
            {
                // Keep accumulating lines for the current GO term
                current_term.push_back(line);
            }
        }

        // Check if there's still a GO term block left in memory after file reading ends.
        // This happens if the file doesn't end with an empty line (which normally triggers block parsing).
        if (!current_term.empty())
        {
            // Variables to store parsed components from the last GO term block.
            std::string go_id;         // GO term ID, e.g., "GO:0008800"
            std::string name;          // Name of the GO term, e.g., "beta-lactamase activity"
            std::string ns;            // Namespace, e.g., "molecular_function"
            std::string metacyc_entry; // MetaCyc reference, e.g., "MetaCyc:BETA-LACTAMASE-RXN"

            // Loop through each line in the final term block
            for (const auto &term_line : current_term)
            {
                // Identify and extract GO term ID
                if (term_line.rfind("id: GO:", 0) == 0)
                {
                    // Skip "id: " prefix (4 characters) and get the rest
                    go_id = term_line.substr(4);
                }
                // Identify and extract the name of the GO term
                else if (term_line.rfind("name:", 0) == 0)
                {
                    // Skip "name: " prefix (6 characters)
                    name = term_line.substr(6);
                }
                // Identify and extract the namespace of the GO term
                else if (term_line.rfind("namespace:", 0) == 0)
                {
                    // Skip "namespace: " prefix (10 characters)
                    ns = term_line.substr(10);
                }
                // Look for cross-references to MetaCyc within the term
                else if (term_line.rfind("xref:", 0) == 0)
                {
                    // Search for the position of "MetaCyc:" in the line
                    size_t pos = term_line.find("MetaCyc:"); // This gives the index where "MetaCyc:" starts in the string.
                    if (pos != std::string::npos)            // std::string::npos means "not found".
                    {
                        // Extract the actual MetaCyc ID (after "MetaCyc:")
                        std::string metacyc_id = term_line.substr(pos + 8);

                        // Clean up: remove any newline characters (precautionary)
                        metacyc_id.erase(std::remove(metacyc_id.begin(), metacyc_id.end(), '\n'), metacyc_id.end());

                        // If this MetaCyc ID is among those the user is searching for
                        if (metacyc_ids.count(metacyc_id))
                        {
                            // Store it in the formatted way for output
                            metacyc_entry = "MetaCyc:" + metacyc_id;
                        }
                    }
                }
            }

            // If a valid GO ID and a matched MetaCyc ID were found, print the result
            if (!go_id.empty() && !metacyc_entry.empty())
            {
                // Output format: GO_ID  name  namespace  MetaCyc:ID
                std::cout << go_id << " " << name << " " << ns << " " << metacyc_entry << "\n";
            }
        }
    }
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("GooboParser");

    program.add_argument("--version")
        .default_value(false)
        .implicit_value(true)
        .help("prints version information and exits");

    program.add_argument("--get-metacyc")
        .nargs(argparse::nargs_pattern::at_least_one)
        .help("MetaCyc IDs followed by input file(s): *.obo or *.obo.gz");

    program.add_argument("--tab-metacyc")
        .nargs(argparse::nargs_pattern::at_least_one)
        .help("Output one or more TSV files");

    try
    {
        program.parse_args(argc, argv);

        const auto &tab_metacyc_args = program.get<std::vector<std::string>>("--tab-metacyc");
        const auto &get_meta_cyc_args = program.get<std::vector<std::string>>("--get-metacyc");

        if (!tab_metacyc_args.empty() && get_meta_cyc_args.empty())
        {
            std::vector<std::string> tab_valid_files;
            std::vector<std::string> files(tab_metacyc_args.begin(), tab_metacyc_args.end());

            for (const auto &file : files)
            {
                if (!fs::exists(file))
                {
                    std::cerr << "File not found: " << file << "\n";
                }
                else
                {
                    tab_valid_files.push_back(file);
                }
            }

            if (tab_valid_files.empty())
            {
                std::cerr << "No valid obo files provided.\n";
                return 1;
            }

            const auto lines = read_obo_file(tab_valid_files);
            const auto terms = extract_terms(lines);

            print_metacyc_table(terms);
        }

        // Handle --get-metacyc arguments if provided

        else if (!get_meta_cyc_args.empty() && tab_metacyc_args.empty())
        {
            // Define regex pattern to match filenames ending with .obo or .obo.gz
            std::regex obo_pattern(R"(.*\.obo(\.gz)?)");

            // Containers for separating MetaCyc IDs and filenames
            std::vector<std::string> meta_ids;
            std::vector<std::string> filenames;
            std::vector<std::string> valid_files;

            // Iterate over each argument passed to --get-metacyc
            for (const auto &arg : get_meta_cyc_args)
            {
                if (std::regex_match(arg, obo_pattern))
                {
                    // If it matches the .obo/.obo.gz pattern, treat as input file
                    filenames.push_back(arg);

                    for (const auto &file : filenames)
                    {
                        if (!fs::exists(file))
                        {
                            std::cerr << "File not found: " << file << "\n";
                        }
                        else
                        {
                            valid_files.push_back(file);
                        }
                    }
                    if (valid_files.empty())
                    {
                        std::cerr << "No valid obo files provided.\n";
                        return 1;
                    }
                }
                else
                {
                    // Otherwise, treat as a MetaCyc ID
                    meta_ids.push_back(arg);
                }
            }

            // Remove duplicates and enable fast lookup using an unordered set
            std::unordered_set<std::string> ids(meta_ids.begin(), meta_ids.end());

            // Call your function to perform the MetaCyc search
            search_metacyc(ids, valid_files);
        }

        else
        {
            throw std::runtime_error("Error: Either --tab-metacyc or --get-metacyc must be specified.");
        }
    }
    catch (const std::exception &err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program << std::endl;
        return 1;
    }

    return 0;
}
