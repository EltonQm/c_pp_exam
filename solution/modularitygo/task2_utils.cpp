#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <zlib.h>
#include "task2_utils.hpp"

namespace fs = std::filesystem;

std::vector<std::string> read_obo_lines(const std::string &filename)
{
    std::vector<std::string> lines;
    char buffer[4096];
    if (filename.size() >= 3 && filename.substr(filename.size() - 3) == ".gz")
    {
        gzFile gzfile = gzopen(filename.c_str(), "rb");
        if (!gzfile)
        {
            throw std::runtime_error("Failed to open .gz file: " + filename);
        }
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
        {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        std::string line;
        while (std::getline(infile, line))
        {
            lines.push_back(line);
        }
    }
    return lines;
}

std::vector<std::vector<std::string>> process_consider_table(
    const std::vector<std::string> &files,
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
                        std::find(namespaces.begin(), namespaces.end(),
                                  current_namespace) != namespaces.end())
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
            {
                continue;
            }
            if (line.rfind("id: ", 0) == 0)
            {
                current_id = line.substr(4);
                current_id.erase(current_id.find_last_not_of(" \n\r\t") + 1);
            }
            else if (line.rfind("namespace: ", 0) == 0)
            {
                current_namespace = line.substr(11);
                current_namespace.erase(
                    current_namespace.find_last_not_of(" \n\r\t") + 1);
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

std::vector<std::vector<std::string>> process_obsolete_stats(
    const std::vector<std::string> &files,
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
                        std::find(namespaces.begin(), namespaces.end(),
                                  current_namespace) != namespaces.end())
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
            {
                continue;
            }
            if (line.rfind("id: ", 0) == 0)
            {
                current_id = line.substr(4);
                current_id.erase(current_id.find_last_not_of(" \n\r\t") + 1);
            }
            else if (line.rfind("namespace: ", 0) == 0)
            {
                current_namespace = line.substr(11);
                current_namespace.erase(
                    current_namespace.find_last_not_of(" \n\r\t") + 1);
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