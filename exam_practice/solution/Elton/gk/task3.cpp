#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <zlib.h>
#include <map>
#include "argparse/argparse.hpp"

class GooboParser
{
private:
    std::string filename;
    std::string namespace_filter;
    std::string outfile;

public:
    GooboParser(const std::string &fname, const std::string &ns = "", const std::string &out = "")
        : filename(fname), namespace_filter(ns), outfile(out) {}

    std::vector<std::vector<std::string>> parseTable(bool use_replaced_by)
    {
        std::vector<std::vector<std::string>> result;
        gzFile file = gzopen(filename.c_str(), "rb");
        if (!file)
        {
            std::cerr << "Error: Cannot open file '" << filename << "'\n";
            return result;
        }

        std::string line;
        char buffer[1024];
        std::string current_id;
        std::string current_namespace;
        std::vector<std::string> alt_ids;
        bool is_obsolete = false;

        while (gzgets(file, buffer, sizeof(buffer)))
        {
            line = buffer;
            line.erase(line.find_last_not_of("\n\r") + 1);
            if (line == "[Term]")
            {
                if (is_obsolete && (namespace_filter.empty() || namespace_filter == current_namespace))
                {
                    if (alt_ids.empty())
                    {
                        alt_ids.push_back("NA");
                    }
                    result.push_back({current_id});
                    result.back().insert(result.back().end(), alt_ids.begin(), alt_ids.end());
                }
                current_id.clear();
                current_namespace.clear();
                alt_ids.clear();
                is_obsolete = false;
            }
            else if (line.starts_with("id: "))
            {
                current_id = line.substr(4);
            }
            else if (line.starts_with("namespace: "))
            {
                current_namespace = line.substr(11);
            }
            else if (line == "is_obsolete: true")
            {
                is_obsolete = true;
            }
            else if (line.starts_with(use_replaced_by ? "replaced_by: " : "consider: "))
            {
                alt_ids.push_back(line.substr(use_replaced_by ? 12 : 10));
            }
        }

        if (is_obsolete && (namespace_filter.empty() || namespace_filter == current_namespace))
        {
            if (alt_ids.empty())
            {
                alt_ids.push_back("NA");
            }
            result.push_back({current_id});
            result.back().insert(result.back().end(), alt_ids.begin(), alt_ids.end());
        }

        gzclose(file);
        return result;
    }

    void printTable(bool use_replaced_by)
    {
        auto table = parseTable(use_replaced_by);
        std::ofstream out;
        if (!outfile.empty())
        {
            out.open(outfile);
            if (!out.is_open())
            {
                std::cerr << "Error: Cannot open output file '" << outfile << "'\n";
                return;
            }
        }
        std::ostream &output = outfile.empty() ? std::cout : out;

        for (const auto &row : table)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                output << row[i];
                if (i < row.size() - 1)
                    output << "\t";
            }
            output << "\n";
        }

        if (!outfile.empty())
            out.close();
    }

    void printObsoletesStats()
    {
        auto table = parseTable(false);
        int consider_count = 0, replaced_by_count = 0, no_alt_count = 0;

        for (const auto &row : table)
        {
            bool has_consider = false, has_replaced_by = false;
            for (size_t i = 1; i < row.size(); ++i)
            {
                if (row[i] != "NA")
                {
                    has_consider = true;
                    break;
                }
            }
            auto replaced_table = parseTable(true);
            for (const auto &r_row : replaced_table)
            {
                if (r_row[0] == row[0] && r_row.size() > 1 && r_row[1] != "NA")
                {
                    has_replaced_by = true;
                    break;
                }
            }
            if (has_consider)
                consider_count++;
            else if (has_replaced_by)
                replaced_by_count++;
            else
                no_alt_count++;
        }

        std::ofstream out;
        if (!outfile.empty())
        {
            out.open(outfile);
            if (!out.is_open())
            {
                std::cerr << "Error: Cannot open output file '" << outfile << "'\n";
                return;
            }
        }
        std::ostream &output = outfile.empty() ? std::cout : out;

        output << "consider\t" << consider_count << "\n"
               << "replaced_by\t" << replaced_by_count << "\n"
               << "no-alternative\t" << no_alt_count << "\n";

        if (!outfile.empty())
            out.close();
    }

    void printSubset()
    {
        std::map<std::string, int> subset_counts;
        gzFile file = gzopen(filename.c_str(), "rb");
        if (!file)
        {
            std::cerr << "Error: Cannot open file '" << filename << "'\n";
            return;
        }

        std::string line;
        char buffer[1024];
        std::string current_subset;

        while (gzgets(file, buffer, sizeof(buffer)))
        {
            line = buffer;
            line.erase(line.find_last_not_of("\n\r") + 1);
            if (line == "[Term]")
            {
                current_subset.clear();
            }
            else if (line.starts_with("subset: "))
            {
                current_subset = line.substr(8);
                subset_counts[current_subset]++;
            }
        }

        std::ofstream out;
        if (!outfile.empty())
        {
            out.open(outfile);
            if (!out.is_open())
            {
                std::cerr << "Error: Cannot open output file '" << outfile << "'\n";
                return;
            }
        }
        std::ostream &output = outfile.empty() ? std::cout : out;

        for (const auto &pair : subset_counts)
        {
            output << pair.first << "\t" << pair.second << "\n";
        }

        if (!outfile.empty())
            out.close();
        gzclose(file);
    }

    void printSpreadsheet()
    {
        std::vector<std::vector<std::string>> result;
        gzFile file = gzopen(filename.c_str(), "rb");
        if (!file)
        {
            std::cerr << "Error: Cannot open file '" << filename << "'\n";
            return;
        }

        std::string line;
        char buffer[1024];
        std::string current_id;
        std::string current_namespace;
        std::vector<std::string> xrefs;

        while (gzgets(file, buffer, sizeof(buffer)))
        {
            line = buffer;
            line.erase(line.find_last_not_of("\n\r") + 1);
            if (line == "[Term]")
            {
                if (!xrefs.empty() && (namespace_filter.empty() || namespace_filter == current_namespace))
                {
                    result.push_back({current_id});
                    result.back().insert(result.back().end(), xrefs.begin(), xrefs.end());
                }
                current_id.clear();
                current_namespace.clear();
                xrefs.clear();
            }
            else if (line.starts_with("id: "))
            {
                current_id = line.substr(4);
            }
            else if (line.starts_with("namespace: "))
            {
                current_namespace = line.substr(11);
            }
            else if (line.starts_with("xref: Reactome"))
            {
                xrefs.push_back(line.substr(6));
            }
        }

        if (!xrefs.empty() && (namespace_filter.empty() || namespace_filter == current_namespace))
        {
            result.push_back({current_id});
            result.back().insert(result.back().end(), xrefs.begin(), xrefs.end());
        }

        std::ofstream out;
        if (!outfile.empty())
        {
            out.open(outfile);
            if (!out.is_open())
            {
                std::cerr << "Error: Cannot open output file '" << outfile << "'\n";
                return;
            }
        }
        std::ostream &output = outfile.empty() ? std::cout : out;

        for (const auto &row : result)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                output << row[i];
                if (i < row.size() - 1)
                    output << "\t";
            }
            output << "\n";
        }

        if (!outfile.empty())
            out.close();
        gzclose(file);
    }
    /*
    void printHierarchicalRelationship()
    {
        std::vector<std::vector<std::string>> result;
        gzFile file = gzopen(filename.c_str(), "rb");
        if (!file)
        {
            std::cerr << "Error: Cannot open file '" << filename << "'\n";
            return;
        }

        std::string line;
        char buffer[1024];
        std::string current_id;
        std::string current_namespace;
        std::vector<std::string> relationships;

        while (gzgets(file, buffer, sizeof(buffer)))
        {
            line = buffer;
            line.erase(line.find_last_not_of("\n\r") + 1);
            if (line == "[Term]")
            {
                if (!relationships.empty() && (namespace_filter.empty() || namespace_filter == current_namespace))
                {
                    result.push_back({current_id});
                    result.back().insert(result.back().end(), relationships.begin(), relationships.end());
                }
                current_id.clear();
                current_namespace.clear();
                relationships.clear();
            }
            else if (line.starts_with("id: "))
            {
                current_id = line.substr(4);
            }
            else if (line.starts_with("namespace: "))
            {
                current_namespace = line.substr(11);
            }
            else if (line.starts_with("is_a: ") || line.starts_with("part_of: "))
            {
                relationships.push_back(line);
            }
        }

        if (!relationships.empty() && (namespace_filter.empty() || namespace_filter == current_namespace))
        {
            result.push_back({current_id});
            result.back().insert(result.back().end(), relationships.begin(), relationships.end());
        }

        std::ofstream out;
        if (!outfile.empty())
        {
            out.open(outfile);
            if (!out.is_open())
            {
                std::cerr << "Error: Cannot open output file '" << outfile << "'\n";
                return;
            }
        }
        std::ostream &output = outfile.empty() ? std::cout : out;

        for (const auto &row : result)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                output << row[i];
                if (i < row.size() - 1)
                    output << "\t";
            }
            output << "\n";
        }

        if (!outfile.empty())
            out.close();
        gzclose(file);
    }
    */
};

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("task3");
    program.add_argument("option")
        .help("Command-line option")
        .default_value(std::string(""))
        .choices({"--consider-table", "--replaced-by", "--obsoletes-stats", "--subset", "--spreadsheet", "--hierarchical_relationship"});
    program.add_argument("filename")
        .help("Input .obo or .obo.gz file");
    program.add_argument("namespace")
        .help("Optional namespace (e.g., molecular_function, cellular_component, biological_process)")
        .nargs(argparse::nargs_pattern::optional)
        .default_value(std::string(""));
    program.add_argument("outfile")
        .help("Optional output file with .tab extension")
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
    std::string outfile = program.get<std::string>("outfile");

    if (!std::filesystem::exists(filename) || (!filename.ends_with(".obo") && !filename.ends_with(".obo.gz")))
    {
        std::cerr << "Error: Invalid or non-existent file '" << filename << "'\n";
        std::cerr << program << std::endl;
        return 1;
    }

    if (!outfile.empty() && !outfile.ends_with(".tab"))
    {
        std::cerr << "Error: Output file must have .tab extension\n";
        std::cerr << program << std::endl;
        return 1;
    }

    GooboParser parser(filename, namespace_filter, outfile);
    if (option == "--consider-table")
    {
        parser.printTable(false);
    }
    else if (option == "--replaced-by")
    {
        parser.printTable(true);
    }
    else if (option == "--obsoletes-stats")
    {
        parser.printObsoletesStats();
    }
    else if (option == "--subset")
    {
        parser.printSubset();
    }
    else if (option == "--spreadsheet")
    {
        parser.printSpreadsheet();
    }
    else if (option == "--hierarchical_relationship")
    {
        parser.printHierarchicalRelationship();
    }

    return 0;
}
