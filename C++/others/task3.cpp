// GooboParser: Parses .obo or .obo.gz files to extract GO terms that reference MetaCyc reactions
// Supports two modes:
//   --tab-metacyc: Outputs a table (GO ID, EC, MetaCyc ID) for all matching terms
//   --get-metacyc: Filters by specific MetaCyc IDs and outputs GO ID, name, namespace, MetaCyc ID

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
#include <filesystem>

namespace fs = std::filesystem;

// Utility: Remove newline character from the end of a string if present
inline void trim_newline(std::string &str)
{
    if (!str.empty() && str.back() == '\n')
    {
        str.pop_back();
    }
}

// Reads lines from one or more .obo or .obo.gz files
std::vector<std::string> read_obo_file(const std::vector<std::string> &filenames)
{
    std::vector<std::string> lines;
    char buffer[4096]; // For reading gzipped content

    for (const auto &filename : filenames)
    {
        if (filename.ends_with(".gz"))
        {
            gzFile gzfile = gzopen(filename.c_str(), "rb");
            if (!gzfile)
                throw std::runtime_error("Could not open compressed file: " + filename);
            while (gzgets(gzfile, buffer, sizeof(buffer)))
            {
                lines.emplace_back(buffer);
            }
            gzclose(gzfile);
        }
        else
        {
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
    return lines;
}

// Parses raw lines into blocks of [Term] entries
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
                terms.push_back(std::move(current_term));
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
                    terms.push_back(std::move(current_term));
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
        terms.push_back(std::move(current_term));
    return terms;
}

// Outputs MetaCyc-related terms in TSV format: GO ID \t EC \t MetaCyc
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
                if (auto pos = line.find("MetaCyc:"); pos != std::string::npos)
                {
                    metacyc_id = line.substr(pos + 8);
                    trim_newline(metacyc_id);
                }
                std::smatch match;
                if (std::regex_search(line, match, ec_pattern))
                {
                    ec_number = match[0].str();
                }
            }
        }

        if (!go_id.empty() && !metacyc_id.empty())
        {
            std::cout << go_id << "\t" << ec_number << "\t" << metacyc_id << "\n";
        }
    }
}

// Filters GO terms by MetaCyc ID and prints matching GO term info
void search_metacyc(const std::unordered_set<std::string> &metacyc_ids,
                    const std::vector<std::string> &filenames)
{
    for (const auto &filename : filenames)
    {
        std::ifstream infile(filename);
        if (!infile.is_open())
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
            continue;
        }

        std::vector<std::string> current_term;
        std::string line;

        while (std::getline(infile, line))
        {
            if (line.empty())
            {
                std::string go_id, name, ns, metacyc_entry;
                for (const auto &term_line : current_term)
                {
                    if (term_line.rfind("id: GO:", 0) == 0)
                        go_id = term_line.substr(4);
                    else if (term_line.rfind("name:", 0) == 0)
                        name = term_line.substr(6);
                    else if (term_line.rfind("namespace:", 0) == 0)
                        ns = term_line.substr(10);
                    else if (term_line.rfind("xref:", 0) == 0)
                    {
                        if (auto pos = term_line.find("MetaCyc:"); pos != std::string::npos)
                        {
                            std::string metacyc_id = term_line.substr(pos + 8);
                            trim_newline(metacyc_id);
                            if (metacyc_ids.count(metacyc_id))
                            {
                                metacyc_entry = "MetaCyc:" + metacyc_id;
                            }
                        }
                    }
                }
                if (!go_id.empty() && !metacyc_entry.empty())
                {
                    std::cout << go_id << " " << name << " " << ns << " " << metacyc_entry << "\n";
                }
                current_term.clear();
            }
            else
            {
                current_term.push_back(line);
            }
        }

        // Handle case where file doesn’t end with empty line
        if (!current_term.empty())
        {
            std::string go_id, name, ns, metacyc_entry;
            for (const auto &term_line : current_term)
            {
                if (term_line.rfind("id: GO:", 0) == 0)
                    go_id = term_line.substr(4);
                else if (term_line.rfind("name:", 0) == 0)
                    name = term_line.substr(6);
                else if (term_line.rfind("namespace:", 0) == 0)
                    ns = term_line.substr(10);
                else if (term_line.rfind("xref:", 0) == 0)
                {
                    if (auto pos = term_line.find("MetaCyc:"); pos != std::string::npos)
                    {
                        std::string metacyc_id = term_line.substr(pos + 8);
                        trim_newline(metacyc_id);
                        if (metacyc_ids.count(metacyc_id))
                        {
                            metacyc_entry = "MetaCyc:" + metacyc_id;
                        }
                    }
                }
            }
            if (!go_id.empty() && !metacyc_entry.empty())
            {
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
        .help("Output MetaCyc to GO mapping in TSV format");

    try
    {
        program.parse_args(argc, argv);

        const auto &tab_metacyc_args = program.present<std::vector<std::string>>("--tab-metacyc");
        const auto &get_meta_cyc_args = program.present<std::vector<std::string>>("--get-metacyc");

        if (tab_metacyc_args && !get_meta_cyc_args)
        {
            std::vector<std::string> valid_files;
            for (const auto &file : *tab_metacyc_args)
            {
                if (fs::exists(file))
                    valid_files.push_back(file);
                else
                    std::cerr << "File not found: " << file << "\n";
            }
            if (valid_files.empty())
                return 1;
            const auto lines = read_obo_file(valid_files);
            const auto terms = extract_terms(lines);
            print_metacyc_table(terms);
        }
        else if (get_meta_cyc_args && !tab_metacyc_args)
        {
            std::regex obo_pattern(R"(.*\.obo(\.gz)?)");
            std::vector<std::string> meta_ids, filenames, valid_files;

            for (const auto &arg : *get_meta_cyc_args)
            {
                if (std::regex_match(arg, obo_pattern))
                    filenames.push_back(arg);
                else
                    meta_ids.push_back(arg);
            }

            for (const auto &file : filenames)
            {
                if (fs::exists(file))
                    valid_files.push_back(file);
                else
                    std::cerr << "File not found: " << file << "\n";
            }
            if (valid_files.empty())
                return 1;

            std::unordered_set<std::string> ids(meta_ids.begin(), meta_ids.end());
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