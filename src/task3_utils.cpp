// task3_utils.cpp — Task 3 impl stubs (compile-ready)
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <unordered_set>
#include <vector>
#include "task3_utils.hpp"

namespace fs = std::filesystem;

std::map<std::string, NamespaceStats> compute_obsolete_stats(
    const std::vector<std::string> &obo_files,
    const std::unordered_set<std::string> &ns_filter,
    const std::regex *name_filter)
{
    std::map<std::string, NamespaceStats> out;
    // TODO: Implement streaming parse; aggregate per-namespace counts
    // Hint: if ns_filter empty, aggregate into "all" only or into actual namespaces then also sum to "all".
    return out;
}

bool write_tab_file(const std::string &path,
                    const std::vector<std::vector<std::string>> &rows)
{
    if (path.size() < 4 || path.substr(path.size() - 4) != ".tab")
    {
        std::cerr << "Error: --output must end with .tab\n";
        return false;
    }
    std::ofstream out(path);
    if (!out)
    {
        std::cerr << "Error: cannot open output: " << path << "\n";
        return false;
    }
    for (const auto &r : rows)
    {
        for (size_t i = 0; i < r.size(); ++i)
        {
            if (i)
                out << '\t';
            out << r[i];
        }
        out << '\n';
    }
    return true;
}
