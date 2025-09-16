#include "task2_utils.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <zlib.h>

std::string OBOParser::trim(const std::string &str) {
  size_t first = str.find_first_not_of(" \t\r\n");
  if (first == std::string::npos)
    return "";
  size_t last = str.find_last_not_of(" \t\r\n");
  return str.substr(first, (last - first + 1));
}

std::string OBOParser::extractValue(const std::string &line,
                                    const std::string &key) {
  size_t pos = line.find(key + ":");
  if (pos != std::string::npos) {
    std::string value = line.substr(pos + key.length() + 1);
    // Remove everything after first space or exclamation mark for relationships
    size_t space_pos = value.find(' ');
    size_t excl_pos = value.find('!');
    size_t end_pos = std::min(space_pos, excl_pos);
    if (end_pos != std::string::npos) {
      value = value.substr(0, end_pos);
    }
    return trim(value);
  }
  return "";
}

std::string OBOParser::extractGOId(const std::string &relationship) {
  size_t start = relationship.find("GO:");
  if (start != std::string::npos) {
    size_t end = relationship.find(' ', start);
    if (end == std::string::npos)
      end = relationship.find('!', start);
    if (end == std::string::npos)
      end = relationship.length();
    return relationship.substr(start, end - start);
  }
  return "";
}

std::ifstream OBOParser::openFile(const std::string &filename) {
  std::ifstream file(filename);
  return file;
}

bool OBOParser::parseFile(const std::string &filename) {
  terms.clear();
  term_map.clear();

  std::ifstream file = openFile(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Cannot open file " << filename << std::endl;
    return false;
  }

  std::string line;
  GOTerm current_term;
  bool in_term = false;
  int term_count = 0;

  while (std::getline(file, line)) {
    line = trim(line);

    if (line.empty() || line[0] == '!')
      continue;

    if (line == "[Term]") {
      if (in_term && !current_term.id.empty()) {
        terms.push_back(current_term);
        term_map[current_term.id] = current_term;
        term_count++;

        // Limit to first 100 entries for development
        // if (term_count >= 100) break;
      }
      current_term = GOTerm();
      in_term = true;
      continue;
    }

    if (line.find("[") == 0) {
      in_term = false;
      continue;
    }

    if (!in_term)
      continue;

    if (line.find("id:") == 0) {
      current_term.id = extractValue(line, "id");
    } else if (line.find("name:") == 0) {
      current_term.name = extractValue(line, "name");
    } else if (line.find("namespace:") == 0) {
      current_term.namespace_name = extractValue(line, "namespace");
    } else if (line.find("is_obsolete:") == 0) {
      std::string value = extractValue(line, "is_obsolete");
      current_term.is_obsolete = (value == "true");
    } else if (line.find("consider:") == 0) {
      std::string consider_id = extractValue(line, "consider");
      if (!consider_id.empty()) {
        current_term.consider.push_back(consider_id);
      }
    } else if (line.find("replaced_by:") == 0) {
      std::string replaced_id = extractValue(line, "replaced_by");
      if (!replaced_id.empty()) {
        current_term.replaced_by.push_back(replaced_id);
      }
    } else if (line.find("is_a:") == 0) {
      std::string is_a_id = extractGOId(line);
      if (!is_a_id.empty()) {
        current_term.is_a.push_back(is_a_id);
      }
    } else if (line.find("relationship: part_of") == 0) {
      std::string part_of_id = extractGOId(line);
      if (!part_of_id.empty()) {
        current_term.part_of.push_back(part_of_id);
      }
    }
  }

  // Add the last term
  if (in_term && !current_term.id.empty()) {
    terms.push_back(current_term);
    term_map[current_term.id] = current_term;
  }

  file.close();
  std::cout << "Parsed " << terms.size() << " terms from " << filename
            << std::endl;
  return true;
}

std::vector<GOTerm>
OBOParser::filterByNamespace(const std::string &namespace_filter) {
  std::vector<GOTerm> filtered;
  if (namespace_filter.empty()) {
    return terms;
  }

  for (const auto &term : terms) {
    if (term.namespace_name == namespace_filter) {
      filtered.push_back(term);
    }
  }
  return filtered;
}

std::vector<std::vector<std::string>>
OBOParser::considerTable(const std::string &namespace_filter) {
  std::vector<std::vector<std::string>> table;
  std::vector<GOTerm> filtered_terms = filterByNamespace(namespace_filter);

  for (const auto &term : filtered_terms) {
    if (term.is_obsolete) {
      if (term.consider.empty()) {
        table.push_back({term.id, term.name, "NA"});
      } else {
        for (const auto &consider_id : term.consider) {
          table.push_back({term.id, term.name, consider_id});
        }
      }
    }
  }

  return table;
}

std::vector<std::vector<std::string>>
OBOParser::replacedByTable(const std::string &namespace_filter) {
  std::vector<std::vector<std::string>> table;
  std::vector<GOTerm> filtered_terms = filterByNamespace(namespace_filter);

  for (const auto &term : filtered_terms) {
    if (term.is_obsolete) {
      if (term.replaced_by.empty()) {
        table.push_back({term.id, term.name, "NA"});
      } else {
        for (const auto &replaced_id : term.replaced_by) {
          table.push_back({term.id, term.name, replaced_id});
        }
      }
    }
  }

  return table;
}

void OBOParser::printTable(const std::vector<std::vector<std::string>> &table,
                           const std::vector<std::string> &headers) {
  if (table.empty()) {
    std::cout << "No data found." << std::endl;
    return;
  }

  // Calculate column widths
  std::vector<size_t> col_widths(headers.size(), 0);
  for (size_t i = 0; i < headers.size(); i++) {
    col_widths[i] = headers[i].length();
  }

  for (const auto &row : table) {
    for (size_t i = 0; i < row.size() && i < col_widths.size(); i++) {
      col_widths[i] = std::max(col_widths[i], row[i].length());
    }
  }

  // Print header
  for (size_t i = 0; i < headers.size(); i++) {
    std::cout << std::left << std::setw(col_widths[i] + 2) << headers[i];
  }
  std::cout << std::endl;

  // Print separator
  for (size_t i = 0; i < headers.size(); i++) {
    std::cout << std::string(col_widths[i] + 2, '-');
  }
  std::cout << std::endl;

  // Print data
  for (const auto &row : table) {
    for (size_t i = 0; i < row.size(); i++) {
      std::cout << std::left << std::setw(col_widths[i] + 2) << row[i];
    }
    std::cout << std::endl;
  }
}