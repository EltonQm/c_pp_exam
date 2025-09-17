#ifndef TASK3_UTILS_HPP
#define TASK3_UTILS_HPP

#include "task2_utils.hpp"
#include <map>
#include <set>

struct ObsoleteStats {
  int total_obsolete;
  int with_consider;
  int with_replaced_by;
  int with_no_alternatives;

  ObsoleteStats()
      : total_obsolete(0), with_consider(0), with_replaced_by(0),
        with_no_alternatives(0) {}
};

struct SubsetInfo {
  std::string subset_name;
  int count;

  SubsetInfo(const std::string &name, int c) : subset_name(name), count(c) {}
};

class GOAnalyzer : public OBOParser {
private:
  std::map<std::string, int> subset_counts;
  std::vector<std::string> xref_entries;

public:
  // Task 3 functionality
  std::vector<SubsetInfo>
  getSubsetCounts(const std::string &namespace_filter = "");
  std::vector<std::string> xrefSearch(const std::string &database = "Reactome");
  ObsoleteStats getObsoleteStats(const std::string &namespace_filter = "");
  std::vector<std::vector<std::string>>
  getHierarchicalRelationships(const std::string &namespace_filter = "");

  // Enhanced parsing for Task 3
  bool parseFileExtended(const std::string &filename);

  // Display functions
  void displayObsoleteStats(const ObsoleteStats &stats,
                            const std::string &namespace_filter = "");
  void displaySubsetInfo(const std::vector<SubsetInfo> &subsets);
  void displayXrefResults(const std::vector<std::string> &xrefs,
                          const std::string &database);
  void displayHierarchicalRelationships(
      const std::vector<std::vector<std::string>> &relationships);

protected:
  // Helper functions for parsing additional fields
  std::string extractSubset(const std::string &line);
  std::string extractXref(const std::string &line);
  bool isReactomeXref(const std::string &xref);
  bool isECXref(const std::string &xref);
};

#endif // TASK3_UTILS_HPP