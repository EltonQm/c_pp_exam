// Include necessary headers
#include <argparse/argparse.hpp> // For command-line argument parsing
#include <filesystem>            // For file existence checking
#include <iostream>              // For printing to std::cout and std::cerr
#include <regex>                 // For regex-based file/namespace validation
#include <string>
#include <vector>         // For handling argument lists
#include "task_utils.hpp" // Project-specific utility declarations

// Define namespace alias for filesystem to avoid long names
namespace fs = std::filesystem;

// ---------------------------------------------------------------
//                     MAIN FUNCTION
// ---------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Convert raw C-style argv to a more convenient C++ std::vector
    std::vector<std::string> args(argv, argv + argc);

    // ----------------------------
    // Setup CLI ArgumentParser
    // ----------------------------
    argparse::ArgumentParser program("GOoboparser");

    // Set a helpful description for --help output
    program.add_description(
        "Elton's GOoboParser - GOobo file processor\n"
        "Author: Elton Ugbogu, University of Potsdam, 2025");

    // Declare the --consider-table option
    program.add_argument("--consider-table")
        .help("Generate a table of obsolete GO terms with consider alternatives")
        .nargs(argparse::nargs_pattern::at_least_one); // Accepts ≥1 argument

    // Declare the --obsolete-stats option (a separate mode)
    program.add_argument("--obsolete-stats")
        .help("Print stats on obsolete GO terms (with/without consider alternatives)")
        .nargs(argparse::nargs_pattern::at_least_one); // Accepts ≥1 argument

    // ------------------------------------
    // Try to parse the command-line input
    // ------------------------------------
    try
    {
        program.parse_args(argc, argv); // May throw exception if args are malformed
    }
    catch (const std::exception &err)
    {
        // If parsing fails, print error message + usage help and exit
        print_command_error(program, args, err.what());
        return 1;
    }

    // -----------------------------------
    // Determine which mode was selected
    // -----------------------------------
    std::vector<std::string> files_and_ns; // Combined list of input files and namespaces
    bool mode_consider = false;            // Flag for --consider-table mode
    bool mode_obsolete = false;            // Flag for --obsolete-stats mode

    if (program.is_used("--consider-table") && program.is_used("--obsolete-stats"))
    {
        // Show a helpful message and usage info
        print_command_usage(args, "Choose either --consider-table or --obsolete-stats (not both)");

        // Exit the program with an error code
        return 1;
    }
    // Check if user selected --consider-table
    else if (program.is_used("--consider-table"))
    {
        files_and_ns = program.get<std::vector<std::string>>("--consider-table");
        mode_consider = true;
    }
    // Check if user selected --obsolete-stats
    else if (program.is_used("--obsolete-stats"))
    {
        files_and_ns = program.get<std::vector<std::string>>("--obsolete-stats");
        mode_obsolete = true;
    }
    // ───────────────────────────────────────────────────────────────
    // Check for mutually exclusive options: --consider-table and --obsolete-stats
    // The program should allow only one of them at a time, not both.
    // If both options are used, print a usage error and exit.
    // ───────────────────────────────────────────────────────────────

    else
    {
        // If neither mode is specified, print general usage and exit
        usage();
        return 0;
    }

    // If no files or namespaces were provided, show an error
    if (files_and_ns.empty())
    {
        print_command_usage(args, "No input files provided.");
        return 1;
    }

    // -------------------------------------------------------
    // Split inputs into valid files and valid namespaces
    // -------------------------------------------------------
    auto valid_files = validate_files(files_and_ns);           // Keep only files with valid extensions and existence
    auto valid_namespaces = validate_namespaces(files_and_ns); // Keep only valid GO namespaces

    // -------------------------------------------------------
    // Print invalid items for feedback (not used later)
    // -------------------------------------------------------
    print_invalid_filenames(args, files_and_ns); // Print warnings for bad filenames
    print_invalid_namespace(args, files_and_ns); // Print warnings for bad namespaces

    // -------------------------------
    // Echo valid files and namespaces
    // -------------------------------
    for (const auto &item : valid_files)
    {
        std::cout << "Valid file - " << item << std::endl;
    }

    for (const auto &n : valid_namespaces)
    {
        std::cout << "Valid namespace - " << n << std::endl;
    }

    // -----------------------------
    // Print selected mode for trace
    // -----------------------------
    std::cout << "Mode: " << (mode_consider ? "consider-table" : "obsolete-stats") << std::endl;

    // ----------------------------------
    // Execute logic based on mode
    // ----------------------------------

    // -------------------------------------------------------
    // Exit if no valid file
    // -------------------------------------------------------
    if (valid_files.empty())
    {
        print_command_usage(args, "No input files provided.");
        return 1;
    }

    // Run table-generating logic for obsolete terms with "consider" suggestions
    if (mode_consider)
    {
        process_consider_table(valid_files, valid_namespaces);
    }

    // Run statistical summary of obsolete terms (with/without alternatives)
    if (mode_obsolete)
    {
        process_obsolete_stats(valid_files, valid_namespaces);
    }

    // Normal exit
    return 0;
}
