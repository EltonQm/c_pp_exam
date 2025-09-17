#ifndef TASK_UTILS_HPP
#define TASK_UTILS_HPP

#include <vector>
#include <string>
#include <unordered_set>

// Reads .obo or .obo.gz lines
std::vector<std::string> read_obo_lines(const std::string &filename);

// Print GO terms containing MetaCyc xrefs
void parse_terms_with_metacyc_lines(const std::vector<std::string> &lines,
                                    const std::unordered_set<std::string> &metacyc_ids);

// Count and display obsolete term statistics
void count_obsolete_terms(const std::vector<std::string> &lines);

// Print basic GO ID and name info
void print_go_id_and_name(const std::vector<std::string> &lines);

#endif // TASK_UTILS_HPP