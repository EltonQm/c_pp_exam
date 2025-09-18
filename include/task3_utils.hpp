// task3_utils.hpp — stats + optional tab output (Task 3)
#pragma once
#include <map>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

// Stats per namespace (or "all"): obsolete_count, with_alternatives_count
struct NamespaceStats
{
    std::size_t obsolete_total = 0;
    std::size_t with_alternatives = 0;
};

std::map<std::string, NamespaceStats> compute_obsolete_stats(
    const std::vector<std::string> &obo_files,
    const std::unordered_set<std::string> &ns_filter,
    const std::regex *name_filter);

// Writes out rows to .tab (validates .tab extension elsewhere)
bool write_tab_file(const std::string &path,
                    const std::vector<std::vector<std::string>> &rows);
