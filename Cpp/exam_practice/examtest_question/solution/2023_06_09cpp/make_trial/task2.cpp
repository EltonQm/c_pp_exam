// GooboParser2.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <zlib.h>
#include <sstream>
#include <regex>
#include <argparse/argparse.hpp>
#include <filesystem>
#include <memory>

using ResultTuple = std::tuple<std::string, std::string, std::string, std::string>;

std::vector<std::string> read_obo_file(const std::string &filename)
{
    std::vector<std::string> lines;
    if (filename.ends_with(".gz"))
    {
        gzFile gz = gzopen(filename.c_str(), "rb");
        if (!gz)
            throw std::runtime_error("Cannot open gzipped file: " + filename);
        char buffer[4096];
        while (gzgets(gz, buffer, sizeof(buffer)))
        {
            lines.emplace_back(buffer);
        }
        gzclose(gz);
    }
    else
    {
        std::ifstream file(filename);
        if (!file)
            throw std::runtime_error("Cannot open file: " + filename);
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
    }
    return lines;
}

std::vector<ResultTuple> find_metacyc_entries(const std::string &filename, const std::string &id)
{
    auto lines = read_obo_file(filename);
    std::vector<ResultTuple> results;

    std::string go_id, name, namespace_, xref;
    bool in_term = false;

    for (const std::string &line : lines)
    {
        if (line == "[Term]")
        {
            in_term = true;
            go_id.clear();
            name.clear();
            namespace_.clear();
            xref.clear();
        }
        else if (in_term && line.empty())
        {
            if (xref.find("MetaCyc:" + id) != std::string::npos)
            {
                results.emplace_back(go_id, name, namespace_, xref);
            }
            in_term = false;
        }
        else if (in_term)
        {
            if (line.starts_with("id: GO:"))
                go_id = line.substr(4);
            else if (line.starts_with("name: "))
                name = line.substr(6);
            else if (line.starts_with("namespace: "))
                namespace_ = line.substr(11);
            else if (line.starts_with("xref: MetaCyc:") && line.find(id) != std::string::npos)
            {
                xref = line.substr(6);
            }
        }
    }

    return results;
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("GooboParser2");
    program.add_argument("--get-metacyc").nargs(1);
    program.add_argument("file");
    program.parse_args(argc, argv);

    std::string id = program.get<std::vector<std::string>>("--get-metacyc")[0];
    std::string file = program.get<std::string>("file");

    try
    {
        auto results = find_metacyc_entries(file, id);
        for (const auto &[go_id, name, ns, xref] : results)
        {
            std::cout << go_id << "\t" << name << "\t" << ns << "\t" << xref << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
