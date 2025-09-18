// task2_utils.cpp — streaming OBO parsing (stub to compile)
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "task2_utils.hpp"

static bool stanza_starts(const std::string &line)
{
    return !line.empty() && line.front() == '[' && line.back() == ']';
}

std::vector<ConsiderRow> build_consider_table(
    const std::vector<std::string> &obo_files,
    const std::unordered_set<std::string> &ns_filter,
    const std::regex *name_filter)
{
    std::vector<ConsiderRow> out;
    out.reserve(1024); // will grow

    // TODO: Implement streaming parser:
    // - iterate files
    // - detect [Term] stanzas
    // - capture id, name, is_obsolete, consider*, alt_id*, is_a/relationship:part_of
    // - filter by namespace(s) if provided
    // - if name_filter != nullptr, apply regex on name
    // - push ConsiderRow

    // Stub return (compiles): user will implement later.
    return out;
}
