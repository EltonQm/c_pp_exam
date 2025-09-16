X#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <zlib.h>  // Make sure you link with -lz
#include "task2_utils.hpp" // Function declarations


namespace fs = std::filesystem;

void usage(const argparse::ArgumentParser &program)
{
    std::cout << R"(
Example usage:

  ./GOoboparser --consider-table file1.obo [file2.obo ...] [namespace]
  ./GOoboparser --obsolete-stats file1.obo [file2.obo ...] [namespace]

Notes:
- Namespace is optional but must be one of: molecular_function, cellular_component, biological_process
- Files must have .obo or .obo.gz extension and exist
)";
}

// Print error with usage
void print_command_error(const argparse::ArgumentParser &program,
                         const std::vector<std::string> & /*args*/,
                         const std::string &error_message)
{
    std::cerr << "Error: " << error_message << "\n\n";
    std::cerr << program;
}

// Check if file has .obo or .obo.gz extension (case-insensitive)
bool has_obo_ext(const std::string &path)
{
    static const std::regex pattern(R"(.*\.obo(\.gz)?$)", std::regex_constants::icase);
    return std::regex_match(path, pattern);
}

// Validate input files for OBO format and existence
std::vector<std::string> validate_files(const argparse::ArgumentParser &program,
                                        const std::vector<std::string> &files)
{
    std::vector<std::string> valid_files;
    static const std::regex dot_checker_regex(R"(\.)");

    for (const auto &filepath : files)
    {
        if (std::regex_search(filepath, dot_checker_regex) && fs::exists(filepath))
        {
            valid_files.push_back(filepath);
        }
        else
        {
            continue;
        }
    }
    return valid_files;
}

// Validate namespaces against allowed list
std::vector<std::string> validate_namespaces(const argparse::ArgumentParser &program,
                                             const std::vector<std::string> &namespaces)
{
    static const std::vector<std::string> allowed = {
        "molecular_function", "cellular_component", "biological_process"};

    std::vector<std::string> valid_namespace;

    for (const auto &ns : namespaces)
    {
        if (std::find(allowed.begin(), allowed.end(), ns) != allowed.end())
        {
            valid_namespace.push_back(ns);
        }
        else
        {
            continue;
        }
    }
    return valid_namespace;
}

// Check and print invalid arguments
void print_invalid_arguments(const argparse::ArgumentParser &program,
                             const std::vector<std::string> &args,
                             const std::vector<std::string> &invalid_items)
{
    static const std::regex dot_checker_regex(R"(\.)");

    for (const auto &item : invalid_items)
    {
        // Case 1 — If it contains a dot but file does not exist
        if (std::regex_search(item, dot_checker_regex) && (!fs::exists(item)))
        {
            auto pos = item.rfind('.');
            std::string ext = item.substr(pos + 1);
            std::cout << "Invalid extension: " << ext << " Expected '.obo' or '.obo.gz'"<< std::endl;
        }
        // Case 2 — If no dot and namespace is invalid
        else if (!std::regex_search(item, dot_checker_regex) &&
                 validate_namespaces(program, {item}).empty())
        {
            std::cout << "Invalid namespace: " << item << " Allowed: molecular_function, cellular_component, biological_process" << std::endl;
        }
    }
}


// Reads all lines from a .obo or .obo.gz file and returns them as a vector of strings.
std::vector<std::string> read_obo_lines(const std::string &filename)
{
    std::vector<std::string> lines;
    char buffer[4096];

    if (filename.size() >= 3 && filename.substr(filename.size() - 3) == ".gz")
    {
        gzFile gzfile = gzopen(filename.c_str(), "rb");
        if (!gzfile)
            throw std::runtime_error("Failed to open .gz file: " + filename);

        while (gzgets(gzfile, buffer, sizeof(buffer)))
            lines.emplace_back(buffer);

        gzclose(gzfile);
    }
    else
    {
        std::ifstream infile(filename);
        if (!infile)
            throw std::runtime_error("Failed to open file: " + filename);

        std::string line;
        while (std::getline(infile, line))
            lines.push_back(line);
    }

    return lines;
}

// Return type: vector of { GO-ID, Consider-ID or "NA" }
std::vector<std::vector<std::string>> process_consider_table(const std::vector<std::string> &files,
                                                    const std::vector<std::string> &namespaces)
{
    std::vector<std::vector<std::string>> result;

    for (const auto &file : files)
    {
        auto lines = read_obo_lines(file);

        bool in_term = false;
        std::string current_id;
        std::string current_namespace;
        std::vector<std::string> consider_ids;
        bool is_obsolete = false;

        for (const auto &line : lines)
        {
            if (line == "[Term]\n" || line == "[Term]")
            {
                in_term = true;
                current_id.clear();
                current_namespace.clear();
                consider_ids.clear();
                is_obsolete = false;
                continue;
            }

            if (line.empty() && in_term)
            {
                if (is_obsolete && !current_id.empty())
                {
                    if (namespaces.empty() ||
                        std::find(namespaces.begin(), namespaces.end(), current_namespace) != namespaces.end())
                    {
                        if (!consider_ids.empty())
                        {
                            for (const auto &alt_id : consider_ids)
                            {
                                result.push_back({current_id, alt_id});
                            }
                        }
                        else
                        {
                            result.push_back({current_id, "NA"});
                        }
                    }
                }
                in_term = false;
                continue;
            }

            if (!in_term)
                continue;

            if (line.rfind("id: ", 0) == 0)
            {
                current_id = line.substr(4);
                current_id.erase(current_id.find_last_not_of(" \n\r\t") + 1);
            }
            else if (line.rfind("namespace: ", 0) == 0)
            {
                current_namespace = line.substr(11);
                current_namespace.erase(current_namespace.find_last_not_of(" \n\r\t") + 1);
            }
            else if (line.rfind("is_obsolete: true", 0) == 0)
            {
                is_obsolete = true;
            }
            else if (line.rfind("consider: ", 0) == 0)
            {
                auto alt = line.substr(9);
                alt.erase(alt.find_last_not_of(" \n\r\t") + 1);
                consider_ids.push_back(alt);
            }
        }
    }

    return result;
}



// Return type: vector of { GO-ID, Consider-ID or "NA" }
std::vector<std::vector<std::string>> process_obsolete_stats(const std::vector<std::string> &files,
                                                    const std::vector<std::string> &namespaces)
{
    std::vector<std::vector<std::string>> result;

    for (const auto &file : files)
    {
        auto lines = read_obo_lines(file);

        bool in_term = false;
        std::string current_id;
        std::string current_namespace;
        std::vector<std::string> consider_ids;
        bool is_obsolete = false;

        for (const auto &line : lines)
        {
            if (line == "[Term]\n" || line == "[Term]")
            {
                in_term = true;
                current_id.clear();
                current_namespace.clear();
                consider_ids.clear();
                is_obsolete = false;
                continue;
            }

            if (line.empty() && in_term)
            {
                if (is_obsolete && !current_id.empty())
                {
                    if (namespaces.empty() ||
                        std::find(namespaces.begin(), namespaces.end(), current_namespace) != namespaces.end())
                    {
                        if (!consider_ids.empty())
                        {
                            for (const auto &alt_id : consider_ids)
                            {
                                result.push_back({current_id, alt_id});
                            }
                        }
                        else
                        {
                            result.push_back({current_id, "NA"});
                        }
                    }
                }
                in_term = false;
                continue;
            }

            if (!in_term)
                continue;

            if (line.rfind("id: ", 0) == 0)
            {
                current_id = line.substr(4);
                current_id.erase(current_id.find_last_not_of(" \n\r\t") + 1);
            }
            else if (line.rfind("namespace: ", 0) == 0)
            {
                current_namespace = line.substr(11);
                current_namespace.erase(current_namespace.find_last_not_of(" \n\r\t") + 1);
            }
            else if (line.rfind("is_obsolete: true", 0) == 0)
            {
                is_obsolete = true;
            }
            else if (line.rfind("consider: ", 0) == 0)
            {
                auto alt = line.substr(9);
                alt.erase(alt.find_last_not_of(" \n\r\t") + 1);
                consider_ids.push_back(alt);
            }
        }
    }

    return result;
}
