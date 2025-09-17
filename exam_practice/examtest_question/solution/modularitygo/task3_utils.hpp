#ifndef TASK3_UTILS_HPP
#define TASK3_UTILS_HPP

#include <optional>
#include <string>
#include <vector>

class GOoboParserApp {
public:
  void
  runObsoleteStats(const std::vector<std::string> &files,
                   const std::optional<std::string> &outfile = std::nullopt);

  void writeConsiderTable(const std::vector<std::string> &files,
                          const std::string &outFilename);
};

#endif // TASK3_UTILS_HPP
