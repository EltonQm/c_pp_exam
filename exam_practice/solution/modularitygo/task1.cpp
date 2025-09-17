#include <argparse/argparse.hpp> // Parse command-line args
#include <filesystem>            // Handle file operations
#include <iostream>              // Console input/output
#include <regex>                 // Pattern matching
#include <string>                // String operations
#include <vector>                // Dynamic arrays
#include "task_utils.hpp"        // Task utility declarations

namespace fs = std::filesystem; // Alias for filesystem

int main(int argc, char *argv[])
{                                                   // Program entry point
  std::vector<std::string> args(argv, argv + argc); // Convert args to vector

  argparse::ArgumentParser program("GOoboparser"); // Initialize parser
  program.add_description(                         // Set help description
      "Elton's GOoboParser - GOobo file processor\n"
      "Author: Elton Ugbogu, University of Potsdam, 2025");
  program.add_argument("--consider-table") // Add consider-table flag
      .help("Generate a table of obsolete GO terms with alternatives")
      .nargs(argparse::nargs_pattern::at_least_one); // Require ≥1 argument
  program.add_argument("--obsolete-stats")           // Add obsolete-stats flag
      .help("Print stats on obsolete GO terms")      // Help message
      .nargs(argparse::nargs_pattern::at_least_one); // Require ≥1 argument

  try
  {                                 // Try parsing args
    program.parse_args(argc, argv); // Parse command-line args
  }
  catch (const std::exception &err)
  {                                                 // Handle parse errors
    print_command_error(program, args, err.what()); // Print error
    return 1;                                       // Exit with error
  }

  std::vector<std::string> files_and_ns; // Store files and namespaces
  bool mode_consider = false;            // Flag for consider-table
  bool mode_obsolete = false;            // Flag for obsolete-stats

  if (program.is_used("--consider-table") && // Check both flags used
      program.is_used("--obsolete-stats"))
  {                           //
    print_command_usage(args, // Print error
                        "Choose either --consider-table or --obsolete-stats");
    return 1; // Exit with error
  }
  else if (program.is_used("--consider-table"))
  { // Check consider-table
    files_and_ns = program.get<std::vector<std::string>>("--consider-table");
    mode_consider = true; // Set consider-table mode
  }
  else if (program.is_used("--obsolete-stats"))
  { // Check obsolete-stats
    files_and_ns = program.get<std::vector<std::string>>("--obsolete-stats");
    mode_obsolete = true; // Set obsolete-stats mode
  }
  else
  {                 // No mode specified
    usage(args[0]); // Print usage
    return 0;       // Exit successfully
  }

  if (files_and_ns.empty())
  {                                                        // Check for empty input
    print_command_usage(args, "No input files provided."); // Print error
    return 1;                                              // Exit with error
  }

  auto valid_files = validate_files(files_and_ns);           // Validate files
  auto valid_namespaces = validate_namespaces(files_and_ns); // Validate namespaces

  print_invalid_filenames(args, files_and_ns); // Warn invalid files
  print_invalid_namespace(args, files_and_ns); // Warn invalid namespaces

  for (const auto &item : valid_files)
  {                                               // Loop valid files
    std::cout << "Valid file - " << item << "\n"; // Print valid file
  }

  for (const auto &n : valid_namespaces)
  {                                                 // Loop namespaces
    std::cout << "Valid namespace - " << n << "\n"; // Print valid namespace
  }

  std::cout << "Mode: " << (mode_consider ? // Print selected mode
                                "consider-table"
                                          : "obsolete-stats")
            << "\n";

  if (valid_files.empty())
  {                                                        // Check no valid files
    print_command_usage(args, "No input files provided."); // Print error
    return 1;                                              // Exit with error
  }

  if (mode_consider)
  { // Check consider-table mode
    std::vector<std::vector<std::string>> result = process_consider_table(valid_files, valid_namespaces);
    for (const auto &row : result)
    {                                                // Loop result pairs
      std::cout << row[0] << "\t" << row[1] << "\n"; // Print GO-ID pair
    }
  }
  else if (mode_obsolete)
  { // Check obsolete-stats mode
    std::vector<std::vector<std::string>> result = process_obsolete_stats(valid_files, valid_namespaces);
    for (const auto &row : result)
    {                                                // Loop result pairs
      std::cout << row[0] << "\t" << row[1] << "\n"; // Print GO-ID pair
    }
  }

  return 0; // Exit successfully
}