// task2_utils.hpp
#ifndef TASK2_UTILS_HPP
#define TASK2_UTILS_HPP

#include <string>
#include <vector>

//
// Read all lines from a .obo or .obo.gz file.
// Throws std::runtime_error on failure.
//
std::vector<std::string> read_obo_lines(const std::string &filename);

//
// Build a table of obsolete GO IDs and their 'consider:' alternatives.
// Each inner vector layout: { GO_ID, NAME (if you add it later – optional), NAMESPACE (optional), ALT_IDs... }
// If no alternatives found for an obsolete term, include "NA" as a single alternative.
// If ns (namespace filter) is non-empty, only include terms whose namespace matches `ns`.
//
std::vector<std::vector<std::string>>
considerTable(const std::vector<std::string> &lines, const std::string &ns = "");

//
// Build a table of obsolete GO IDs and their 'replaced_by:' targets.
// Same conventions as considerTable() regarding filtering and NA handling.
//
std::vector<std::vector<std::string>>
replacedByTable(const std::vector<std::string> &lines, const std::string &ns = "");

#endif // TASK2_UTILS_HPP
