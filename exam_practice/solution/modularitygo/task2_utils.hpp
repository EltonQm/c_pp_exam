#ifndef TASK2_UTILS_HPP
#define TASK2_UTILS_HPP

#include "task_utils.hpp"
#include <vector>
#include <string>

std::vector<std::string> read_obo_lines(const std::string &filename);
std::vector<std::vector<std::string>> process_consider_table(
    const std::vector<std::string> &files,
    const std::vector<std::string> &namespaces);
std::vector<std::vector<std::string>> process_obsolete_stats(
    const std::vector<std::string> &files,
    const std::vector<std::string> &namespaces);

#endif // TASK2_UTILS_HPP