// File: task2.hpp
#ifndef TASK2_HPP
#define TASK2_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

// Parses a .obo file and returns a list of GO_ID → CONSIDER_ID pairs
// If no consider entry is found, pair will be (GO_ID, "NA")
std::vector<std::pair<std::string, std::string>> considerTable(const std::string &filename,
                                                               const std::string &namespace_filter = "");

#endif // TASK2_HPP
