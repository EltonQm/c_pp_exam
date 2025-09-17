// Include necessary headers
#include <argparse/argparse.hpp> // For command-line argument parsing
#include <filesystem>            // For file existence checking
#include <iostream>              // For printing to std::cout and std::cerr
#include <regex>                 // For regex-based file/uniprot_id validation
#include <string>
#include <vector>         // For handling argument lists
#include "task_utils.hpp" // Project-specific utility declarations

// Define uniprot_id alias for filesystem to avoid long names
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
    argparse::ArgumentParser program("GOdatparser");

    // Set a helpful description for --help output
    program.add_description(
        "Elton's GOdatParser - GOdat file processor\n"
        "Author: Elton Ugbogu, University of Potsdam, 2025");

    // Declare the --seq-start option
    program.add_argument("--seq-start")
        .help("Generate a table of obsolete GO terms with consider alternatives")
        .nargs(argparse::nargs_pattern::at_least_one); // Accepts ≥1 argument

    // Declare the --get-entry option (a separate mode)
    program.add_argument("--get-entry")
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
    std::vector<std::string> files_and_ns; // Combined list of input files and uniprot_ids
    bool mode_seq_start = false;           // Flag for --seq-start mode
    bool mode_get_entry = false;           // Flag for --get-entry mode

    if (program.is_used("--seq-start") && program.is_used("--get-entry"))
    {
        // Show a helpful message and usage info
        print_command_usage(args, "Choose either --seq-start or --get-entry (not both)");

        // Exit the program with an error code
        return 1;
    }
    // Check if user selected --seq-start
    else if (program.is_used("--seq-start"))
    {
        files_and_ns = program.get<std::vector<std::string>>("--seq-start");
        mode_seq_start = true;
    }
    // Check if user selected --get-entry
    else if (program.is_used("--get-entry"))
    {
        files_and_ns = program.get<std::vector<std::string>>("--get-entry");
        mode_get_entry = true;
    }
    // ───────────────────────────────────────────────────────────────
    // Check for mutually exclusive options: --seq-start and --get-entry
    // The program should allow only one of them at a time, not both.
    // If both options are used, print a usage error and exit.
    // ───────────────────────────────────────────────────────────────

    else
    {
        // If neither mode is specified, print general usage and exit
        usage();
        return 0;
    }

    // If no files or uniprot_ids were provided, show an error
    if (files_and_ns.empty())
    {
        print_command_usage(args, "No input files provided.");
        return 1;
    }

    // -------------------------------------------------------
    // Split inputs into valid files and valid uniprot_ids
    // -------------------------------------------------------
    auto valid_files = validate_files(files_and_ns);             // Keep only files with valid extensions and existence
    auto valid_uniprot_ids = validate_uniprot_ids(files_and_ns); // Keep only valid GO uniprot_ids

    // -------------------------------------------------------
    // Print invalid items for feedback (not used later)
    // -------------------------------------------------------
    print_invalid_filenames(args, files_and_ns);  // Print warnings for bad filenames
    print_invalid_uniprot_id(args, files_and_ns); // Print warnings for bad uniprot_ids

    // -------------------------------
    // Echo valid files and uniprot_ids
    // -------------------------------
    for (const auto &item : valid_files)
    {
        std::cout << "Valid file - " << item << std::endl;
    }

    for (const auto &n : valid_uniprot_ids)
    {
        std::cout << "Valid uniprot_id - " << n << std::endl;
    }

    // -----------------------------
    // Print selected mode for trace
    // -----------------------------
    std::cout << "Mode: " << (mode_seq_start ? "seq-start" : "get-entry") << std::endl;

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
    if (mode_seq_start)
    {
        process_seq_start(valid_files, valid_uniprot_ids);
    }

    // Run statistical summary of obsolete terms (with/without alternatives)
    if (mode_get_entry)
    {
        process_get_entry(valid_files, valid_uniprot_ids);
    }

    // Normal exit
    return 0;
}
