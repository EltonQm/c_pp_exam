#include "task3_utils.hpp"
#include "task2_utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// GOoboParserApp class implementation
void GOoboParserApp::runObsoleteStats(
    const std::vector<std::string> &files,
    const std::optional<std::string> &outfile) {
  auto obsoleteTerms = process_consider_table(files, {});
  // Count terms based on whether they are considered or not
  // "consider" means the second element is a GO ID, "no-consider"
  int considerCount = 0;
  int noConsiderCount = 0;

  for (const auto &term : obsoleteTerms) {
    if (term.size() >= 2) {
      if (term[1] == "NA") {
        noConsiderCount++;
      } else if (term[1].rfind("GO", 0) == 0) {
        considerCount++;
      }
    }
  }
  //
  std::ostringstream output;
  output << "consider " << considerCount << "\n";
  output << "no-consider " << noConsiderCount << "\n";

  if (outfile) {
    std::ofstream out(*outfile);
    if (!out) {
      std::cerr << "Failed to open output file: " << *outfile << std::endl;
      return;
    }
    out << output.str();
  } else {
    std::cout << output.str();
  }
}

// Write the consider table to a .tab file
void GOoboParserApp::writeConsiderTable(const std::vector<std::string> &files,
                                        const std::string &outFilename) {
  auto considerTerms = process_consider_table(files, {});
  std::ofstream out(outFilename);
  if (!out) {
    std::cerr << "Failed to open output file: " << outFilename << std::endl;
    return;
  }

  for (const auto &term : considerTerms) {
    if (term.size() >= 2) {
      out << term[0] << "\t" << term[1] << "\n";
    }
  }
  std::cout << "Consider table written to: " << outFilename << std::endl;
}
