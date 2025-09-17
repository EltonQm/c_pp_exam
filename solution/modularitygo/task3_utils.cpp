#include "task3_utils.hpp" // Include task3 utilities header
#include "task2_utils.hpp" // Include task2 utilities header
#include <fstream>         // Include file stream library
#include <iostream>        // Include input/output stream library
#include <sstream>         // Include string stream library
#include <vector>          // Include vector library

// GOoboParserApp class implementation
void GOoboParserApp::runObsoleteStats(         // Define runObsoleteStats method
    const std::vector<std::string> &files,     // Input vector of file names
    const std::optional<std::string> &outfile) // Optional output file name
{                                              // Start of method body
  auto obsoleteTerms = process_consider_table(files, {});
  // Process files to get obsolete terms
  // Count terms based on whether they are considered or not
  // "consider" means the second element is a GO ID, "no-consider"
  int considerCount = 0;   // Initialize counter for terms with consider IDs
  int noConsiderCount = 0; // Initialize counter for terms without consider IDs

  for (const auto &term : obsoleteTerms)
  { // Iterate over obsolete terms
    if (term.size() >= 2)
    { // Check if term has at least 2 elements
      if (term[1] == "NA")
      {                                                       // Check if second element is "NA"
        std::cout << term[0] << "\t" << term[1] << std::endl; // Print term details
        noConsiderCount++;                                    // Increment no-consider counter
      }
      else if (term[1].rfind(" GO:", 0) == 0)                 // Check if second element starts with "GO:"
      {                                                       // Check if second element starts with "GO"
        std::cout << term[0] << "\t" << term[1] << std::endl; // Print term details
        considerCount++;                                      // Increment consider counter
      } // End of else if block
    } // End of if block
  } // End of for loop
  //
  std::ostringstream output;                           // Create output string stream
  output << "consider " << considerCount << "\n";      // Add consider count to output
  output << "no-consider " << noConsiderCount << "\n"; // Add no-consider count to output

  if (outfile)
  {                              // Check if output file is specified
    std::ofstream out(*outfile); // Open output file stream
    if (!out)
    {                                                                       // Check if file opened successfully
      std::cerr << "Failed to open output file: " << *outfile << std::endl; // Print error
      return;                                                               // Exit method
    } // End of if block
    out << output.str(); // Write output to file
  }
  else
  {                            // If no output file specified
    std::cout << output.str(); // Print output to console
  } // End of else block
} // End of method

// Write the consider table to a .tab file
void GOoboParserApp::writeConsiderTable(   // Define writeConsiderTable method
    const std::vector<std::string> &files, // Input vector of file names
    const std::string &outFilename)
{ // Output file name
  auto considerTerms = process_consider_table(files, {});
  // Process files to get consider terms
  std::ofstream out(outFilename); // Open output file stream
  if (!out)
  {                                                                          // Check if file opened successfully
    std::cerr << "Failed to open output file: " << outFilename << std::endl; // Print error
    return;                                                                  // Exit method
  } // End of if block

  for (const auto &term : considerTerms)
  { // Iterate over consider terms
    if (term.size() >= 2)
    {                                            // Check if term has at least 2 elements
      out << term[0] << "\t" << term[1] << "\n"; // Write term ID and consider ID to file
    } // End of if block
  } // End of for loop
  std::cout << "Consider table written to: " << outFilename << std::endl; // Print success message
} // End of method