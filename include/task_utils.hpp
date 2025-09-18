// task_utils.hpp — shared utilities for GO parser tasks
#pragma once
#include <argparse/argparse.hpp>
#include <regex>
#include <string>
#include <vector>
#include <unordered_set>
#include <optional>

struct CLIOptions
{
    // Common across tasks
    bool consider_table = false;
    bool obsolete_stats = false;
    std::vector<std::string> obo_files;         // required ≥1
    std::unordered_set<std::string> namespaces; // optional filter
    std::optional<std::regex> name_pattern;     // optional name filter
    std::optional<std::string> output_tab;      // task3 optional
};

// ---- Namespaces helpers ----
bool is_valid_namespace(const std::string &ns);
void normalize_and_validate_namespaces(const std::vector<std::string> &raw,
                                       std::unordered_set<std::string> &out);

// ---- Files / extensions ----
bool has_obo_ext_ci(const std::string &path); // .obo (case-insensitive)
void validate_input_files(const std::vector<std::string> &files,
                          std::vector<std::string> &valid,
                          std::vector<std::string> &invalid_ext,
                          std::vector<std::string> &missing);

// ---- Usage & CLI parsing ----
void print_usage(const std::string &progname);
CLIOptions parse_task1_cli(int argc, char **argv);
