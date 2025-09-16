// ---------------------------------------------------------------
// GOfastaParser Main - task1.cpp
// ---------------------------------------------------------------
// Author: Elton Ugbogu, University of Potsdam
// Purpose: Parse GO .obo files for MetaCyc ID links using CLI options
// Modes: --get-metacyc <MetaCyc-ID> <OBOFILE>, --tab-metacyc <OBOFILE>
// ---------------------------------------------------------------

#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "task_utils.hpp" // Header for utility declarations

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    // Convert raw C-style argv into a std::vector for convenient usage
    std::vector<std::string> args(argv, argv + argc);

    // ---------------------------
    // Setup argument parser
    // ---------------------------
    argparse::ArgumentParser program("GOfastaParser");

    program.add_description(
        "GOfastaParser: Process GO .obo files for MetaCyc references\n"
        "Author: Elton Ugbogu, University of Potsdam, 2025");

    // --get-metacyc: expects MetaCyc ID and OBO file
    program.add_argument("--get-metacyc")
        .help("Extract GO terms linked to the given MetaCyc ID (must contain RXN or PWY)")
        .nargs(2);

    // --tab-metacyc: expects only a single OBO file
    program.add_argument("--tab-metacyc")
        .help("Generate a table of MetaCyc → GO term mappings")
        .nargs(1);

      // ---------------------------
    // Parse input arguments
    // ---------------------------
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        print_command_error(program, args, err.what());
        return 1;
    }

    // -----------------------------------------
    // Validate selected mode
    // -----------------------------------------
    bool mode_get = program.is_used("--get-metacyc");
    bool mode_tab = program.is_used("--tab-metacyc");

    if (mode_get && mode_tab)
    {
        print_command_usage(args, "Please specify only one of --get-metacyc or --tab-metacyc");
        return 1;
    }

    if (!mode_get && !mode_tab)
    {
        usage();
        return 1;
    }

    // -----------------------------------------
    // Prepare containers for validated input
    // -----------------------------------------
    std::vector<std::string> valid_files;
    std::vector<std::string> metacyc_ids;

    // Handle --get-metacyc mode
    if (mode_get)
    {
        auto params = program.get<std::vector<std::string>>("--get-metacyc");

        if (params.size() != 2)
        {
            print_command_usage(args, "Expected MetaCyc ID and OBO file for --get-metacyc");
            return 1;
        }

        const std::string &meta_id = params[0];
        const std::string &file = params[1];

        // Validate MetaCyc ID format
        if (!std::regex_search(meta_id, std::regex(R"((RXN|PWY)-\d+)", std::regex::icase)))
        {
            print_command_usage(args, "MetaCyc ID must match format RXN-xxxx or PWY-xxxx");
            return 1;
        }

        // Check file existence and extension
        if (!fs::exists(file))
        {
            print_command_usage(args, "File does not exist: " + file);
            return 1;
        }

        if (!(file.ends_with(".obo") || file.ends_with(".obo.gz")))
        {
            print_command_usage(args, "Invalid file extension. Only .obo or .obo.gz allowed.");
            return 1;
        }

        valid_files.push_back(file);
        metacyc_ids.push_back(meta_id);
    }

    // Handle --tab-metacyc mode
    if (mode_tab)
    {
        auto params = program.get<std::vector<std::string>>("--tab-metacyc");

        if (params.size() != 1)
        {
            print_command_usage(args, "Expected one OBO file for --tab-metacyc");
            return 1;
        }

        const std::string &file = params[0];

        if (!fs::exists(file))
        {
            print_command_usage(args, "File does not exist: " + file);
            return 1;
        }

        if (!(file.ends_with(".obo") || file.ends_with(".obo.gz")))
        {
            print_command_usage(args, "Invalid file extension. Only .obo or .obo.gz allowed.");
            return 1;
        }

        valid_files.push_back(file);
    }

    // -----------------------------------------
    // Execute selected mode logic
    // -----------------------------------------
    if (mode_get)
    {
        process_get_metacyc(valid_files, metacyc_ids);
    }
    else if (mode_tab)
    {
        process_tab_metacyc(valid_files, {}); // meta ID not needed in tab mode
    }

    return 0;
}
