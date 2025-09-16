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

    // Declare the --go option
    program.add_argument("--go")
        .help("Generate a table of obsolete GO terms with consider alternatives")
        .nargs(argparse::nargs_pattern::at_least_one); // Accepts ≥1 argument

    // Declare the --doi option (a separate mode)
    program.add_argument("--doi")
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
    bool mode_go = false;                  // Flag for --go mode
    bool mode_doi = false;                 // Flag for --doi mode

    // Check if user selected --go
    if (program.is_used("--go"))
    {
        files_and_ns = program.get<std::vector<std::string>>("--go");
        mode_go = true;
    }
    // Check if user selected --doi
    else if (program.is_used("--doi"))
    {
        files_and_ns = program.get<std::vector<std::string>>("--doi");
        mode_doi = true;
    }
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

    // ───────────────────────────────────────────────────────────────
    // Check for mutually exclusive options: --go and --doi
    // The program should allow only one of them at a time, not both.
    // If both options are used, print a usage error and exit.
    // ───────────────────────────────────────────────────────────────
    if (program.is_used("--go") && program.is_used("--doi"))
    {
        // Show a helpful message and usage info
        print_command_usage(args, "Choose either --go or --doi (not both)");

        // Exit the program with an error code
        return 1;
    }

    // -------------------------------------------------------
    // Split inputs into valid files and valid namespaces
    // -------------------------------------------------------
    auto valid_files = validate_files(program, files_and_ns); // Keep only files with valid extensions and existence

    // -------------------------------------------------------
    // Print invalid items for feedback (not used later)
    // -------------------------------------------------------
    print_invalid_filenames(program, args, files_and_ns); // Print warnings for bad filenames

    // -------------------------------
    // Echo valid files and namespaces
    // -------------------------------
    for (const auto &item : valid_files)
    {
        std::cout << "Valid file - " << item << "\n";
    }

    // -----------------------------
    // Print selected mode for trace
    // -----------------------------
    std::cout << "Mode: " << (mode_go ? "--go" : "--doi") << "\n";

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
    if (mode_go)
    {
        get_go_ids(valid_files);
    }

    // Run statistical summary of obsolete terms (with/without alternatives)
    if (mode_doi)
    {
        get_kegg_ids(valid_files);
    }

    // Normal exit
    return 0;
}
