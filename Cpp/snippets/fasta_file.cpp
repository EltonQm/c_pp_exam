#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

namespace fs = std::filesystem;

// Print usage instruction
void usage() { std::cout << "Usage: ./fastafile Pattern\n"; }

std::vector<std::string> find_matching_ids(const std::string &filename,
                                           const std::string &pattern) {
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    std::cerr << "Error: could not open file.\n";
    return {};
  }

  std::regex rx(pattern, std::regex::ECMAScript);
  std::unordered_set<std::string> seen_ids;
  std::vector<std::string> result;

  std::string line;
  std::string current_id;
  std::string current_sequence;

  while (std::getline(infile, line)) {
    if (line.rfind(">", 0) == 0) {
      // Check the previous entry
      if (!current_sequence.empty() &&
          std::regex_search(current_sequence, rx)) {
        if (seen_ids.insert(current_id).second)
          result.push_back(current_id);
      }

      current_id = line.substr(1); // Store header without '>'
      current_sequence.clear();
    } else {
      current_sequence += line;
    }
  }

  // Check the last entry
  if (!current_sequence.empty() && std::regex_search(current_sequence, rx)) {
    if (seen_ids.insert(current_id).second)
      result.push_back(current_id);
  }

  return result;
}

int main() {
  std::string filename;
  std::cout << "Enter the path to the fasta file: ";
  std::getline(std::cin, filename);

  std::string pattern;
  std::cout << "Enter the regex pattern: ";
  std::getline(std::cin, pattern);

  if (!fs::exists(filename)) {
    std::cout << "Given file does not exist\n";
    return 1;
  }

  auto res = find_matching_ids(filename, pattern);

  if (res.empty()) {
    std::cout << "No matching sequences found.\n";
    usage();
  } else {
    std::cout << "\nMatching sequence IDs:\n";
    for (const auto &id : res) {
      std::cout << "- " << id << "\n";
    }
  }
}
