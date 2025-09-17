#ifndef TASK3_UTILS_HPP // Prevent multiple inclusions of header
#define TASK3_UTILS_HPP // Define header guard

#include <optional> // Include optional library
#include <string>   // Include string library
#include <vector>   // Include vector library

namespace argparse // Declare argparse namespace
{
  class ArgumentParser; // Forward declare ArgumentParser class
} // End of argparse namespace

class GOoboParserApp                                                          // Define GOoboParserApp class
{                                                                             // Start of class definition
public:                                                                       // Public access specifier
  void                                                                        // Declare runObsoleteStats method
  runObsoleteStats(const std::vector<std::string> &files,                     // Parameter: vector of input files
                   const std::optional<std::string> &outfile = std::nullopt); // Parameter: optional output file

  void                                                      // Declare writeConsiderTable method
  writeConsiderTable(const std::vector<std::string> &files, // Parameter: vector of input files
                     const std::string &outFilename);       // Parameter: output file name
}; // End of class definition

#endif // TASK3_UTILS_HPP                     // End of header guard