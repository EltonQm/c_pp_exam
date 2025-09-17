#include "task3_utils.hpp"       // Include task3 utilities header
#include "task_utils.hpp"        // Include general task utilities header
#include "task2_utils.hpp"       // Include task2 utilities header
#include <argparse/argparse.hpp> // Include argparse library for command-line parsing
#include <iostream>              // Include input/output stream library
#include <string>                // Include string library
#include <vector>                // Include vector library

int main(int argc, char *argv[]) // Main function with command-line arguments
{
  argparse::ArgumentParser program("GOoboParserApp"); // Create argument parser instance

  program.add_argument("--obsolete-stats")                                                           // Add --obsolete-stats argument
      .help("Show obsolete stats for the file(s). Usage: --obsolete-stats <input.obo> [output.tab]") // Set help message
      .nargs(argparse::nargs_pattern::any);                                                          // Allow any number of arguments (validated later)

  program.add_argument("--consider-table")                                                                // Add --consider-table argument
      .help("Write consider table to output .tab file. Usage: --consider-table <input.obo> <output.tab>") // Set help message
      .nargs(2);                                                                                          // Require exactly 2 arguments

  try // Begin try block for parsing arguments
  {
    program.parse_args(argc, argv); // Parse command-line arguments
  }
  catch (const std::exception &err) // Catch any parsing errors
  {
    std::vector<std::string> args(argv, argv + argc); // Convert argv to vector of strings
    print_command_error(program, args, err.what());   // Print error message with details
    return 1;                                         // Return error code
  }

  // prevent using both modes at once
  if (program.is_used("--consider-table") && program.is_used("--obsolete-stats")) // Check if both options used
  {
    std::vector<std::string> args(argv, argv + argc);                                           // Convert argv to vector of strings
    print_command_usage(args, "Choose either --consider-table or --obsolete-stats, not both."); // Print usage error
    return 1;                                                                                   // Return error code
  }

  GOoboParserApp app; // Create GOoboParserApp instance

  if (program.is_used("--consider-table")) // Check if --consider-table option used
  {
    const auto args = program.get<std::vector<std::string>>("--consider-table"); // Get arguments for --consider-table
    // nargs(2) guarantees args.size()==2
    app.writeConsiderTable({args[0]}, args[1]); // Run consider table with input and output files
    return 0;                                   // Return success code
  }

  if (program.is_used("--obsolete-stats")) // Check if --obsolete-stats option used
  {
    const auto args = program.get<std::vector<std::string>>("--obsolete-stats"); // Get arguments for --obsolete-stats
    if (args.size() == 0)                                                        // Check if no arguments provided
    {
      std::cerr << "Usage: --obsolete-stats <input.obo> [output.tab]\n"; // Print usage error
      return 1;                                                          // Return error code
    }
    else if (args.size() == 1) // Check if only input file provided
    {
      app.runObsoleteStats({args[0]}); // Run obsolete stats with input file
      return 0;                        // Return success code
    }
    else if (args.size() == 2) // Check if input and output files provided
    {
      app.runObsoleteStats({args[0]}, args[1]); // Run obsolete stats with input and output files
      return 0;                                 // Return success code
    }
    else // Handle invalid number of arguments
    {
      std::cerr << "Usage: --obsolete-stats <input.obo> [output.tab]\n"; // Print usage error
      return 1;                                                          // Return error code
    }
  }

  // no mode provided: print usage
  {
    std::vector<std::string> args(argv, argv + argc); // Convert argv to vector of strings
    usage(args[0]);                                   // Print usage information
  }
  return 0; // Return success code
}