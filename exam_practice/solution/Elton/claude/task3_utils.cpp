#include "task3_utils.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

std::string GOAnalyzer::extractSubset(const std::string &line) {
  size_t pos = line.find("subset:");
  if (pos != std::string::npos) {
    std::string subset = line.substr(pos + 7);
    return trim(subset);
  }
  return "";
}

std::string GOAnalyzer::extractXref(const std::string &line) {
  size_t pos = line.find("xref:");
  if (pos != std::string::npos) {
    std::string xref = line.substr(pos + 5);
    return trim(xref);
  }
  return "";
}

bool GOAnalyzer::isReactomeXref(const std::string &xref) {
  return xref.find("Reactome:") == 0 || xref.find("REACT:") == 0;
}

bool GOAnalyzer::isECXref(const std::string &xref) {
  return xref.find("EC:") == 0;
}

bool GOAnalyzer::parseFileExtended(const std::string &filename) {
  // First parse with base functionality
  if (!parseFile(filename)) {
    return false;
  }

  // Now parse again for extended features
  subset_counts.clear();
  xref_entries.clear();

  std::ifstream file(filename);
  if (!file.is_open()) {
  return