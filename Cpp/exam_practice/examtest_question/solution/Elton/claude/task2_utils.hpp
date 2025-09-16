#ifndef TASK2_UTILS_HPP
#define TASK2_UTILS_HPP

#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

struct GOTerm {
  std::string id;
  std::string name;
  std::string namespace_name;
  bool is_obsolete;
  std::vector<std::string> consider;
  std::vector<std::string> replaced_by;
  std::vector<std::string> is_a;
  std::vector<std::string> part_of;

  GOTerm() : is_obsolete(false) {}
};

class OBOParser {
private:
  std::vector<GOTerm> terms;
  std::map<std::string, GOTerm> term_map;

  std::string trim(const std::string &str);
  std::string extractValue(const std::string &line, const std::string &key);
  std::string extractGOId(const std::string &relationship);
  std::ifstream openFile(const std::string &filename);

public:
  bool parseFile(const std::string &filename);
  std::vector<std::vector<std::string>>
  considerTable(const std::string &namespace_filter = "");
  std::vector<std::vector<std::string>>
  replacedByTable(const std::string &namespace_filter = "");

  // Getters
  const std::vector<GOTerm> &getTerms() const { return terms; }
  const std::map<std::string, GOTerm> &getTermMap() const { return term_map; }

  // Utility functions
  void printTable(const std::vector<std::vector<std::string>> &table,
                  const std::vector<std::string> &headers);
  std::vector<GOTerm> filterByNamespace(const std::string &namespace_filter);
};

#endif // TASK2_UTILS_HPP