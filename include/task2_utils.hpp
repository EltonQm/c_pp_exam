// task2_utils.hpp — OBO parsing + consider-table (Task 2)
#pragma once
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

// A single result row:
// obsolete_id, comma-separated alternative_ids, parent_id (is_a/part_of parent if present)
struct ConsiderRow
{
    std::string obsolete_id;
    std::string alternatives_csv; // "GO:xxxx,GO:yyyy"
    std::string parent_id;        // from is_a/part_of; empty if none
};

std::vector<ConsiderRow> build_consider_table(
    const std::vector<std::string> &obo_files,
    const std::unordered_set<std::string> &ns_filter, // empty => all
    const std::regex *name_filter                     // nullptr => no filter
);
