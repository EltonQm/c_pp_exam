#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

namespace fs = std::filesystem;

// Usage
void usage(argparse::ArgumentParser &program)
{
    std::cout << R"(

Example usage:

  ./GOoboparser --consider-table file1.obo [file2.obo ...] [namespace]
  ./GOoboparser --obsolete-stats file1.obo [file2.obo ...] [namespace]

Notes:
- Namespace is optional but must be one of: molecular_function, cellular_component, biological_process
- Files must have .obo or .obo.gz extension and exist
)";
}

// Print error and usage
void print_command_error(const argparse::ArgumentParser &program,
                         const std::vector<std::string> & /*args*/,
                         const std::string &error_message)
{
    std::cerr << "Error: " << error_message << "\n\n";
}

/*
 * Checks if a file path has an OBO extension (.obo or .obo.gz), case-insensitive.
 * path The file path to check
 *  True if path ends with .obo or .obo.gz (any case), false otherwise
 * Uses regex for comprehensive matching - handles cases like '/path/to/file.ObO.Gz'
 */
// Check if file has .obo or .obo.gz extension (case-insensitive)
bool has_obo_ext(const std::string &path)
{
    static const std::regex pattern(R"(.*\.obo(\.gz)?$)", std::regex_constants::icase);
    return std::regex_match(path, pattern);
}

// Validate input files for OBO format and existence
std::vector<std::string> validate_files(const argparse::ArgumentParser &program,
                                      const std::vector<std::string> &args,
                                      const std::vector<std::string> &files)
{
    if (files.empty())
    {
        print_command_error(program, args, "No input files provided");
        
    }

    std::vector<std::string> valid_files;
    static const std::regex dot_checker_regex(R"(\.)");
    
    for (const auto &filepath : files)
    {
        if (std::regex_search(filepath, dot_checker_regex)) {
            if (!has_obo_ext(filepath))
            {
                auto pos = filepath.rfind('.');
                std::string ext = filepath.substr(pos + 1);
                print_command_error(program, args,
                    "Invalid extension '" + ext + "'. Expected '.obo' or '.obo.gz'");
               
            }
            
            if (!fs::exists(filepath))
            {
                print_command_error(program, args, "File not found: '" + filepath + "'");
                
            }
            
            if (has_obo_ext(filepath) && fs::exists(filepath))
            {
                // Only add if both checks pass
                valid_files.push_back(filepath);
            }

            
        }
        else
        {
            continue;
        }
    }
    return valid_files;
}


// Validate namespaces against allowed list and check for invalid characters
std::vector<std::string> validate_namespaces(const argparse::ArgumentParser &program,
                                           const std::vector<std::string> &args,
                                           const std::vector<std::string> &namespaces)
{
    static const std::vector<std::string> allowed = {
        "molecular_function", "cellular_component", "biological_process"};

    std::vector<std::string> valid;
    std::vector<std::string> invalid;

    for (const auto &ns : namespaces) {
        // Check against allowed namespaces first
        if (std::find(allowed.begin(), allowed.end(), ns) != allowed.end()) {
            valid.push_back(ns);
        }
        else {
            // Additional validation checks
            static const std::regex dot_checker_regex(R"(\.)");
            if (std::regex_search(ns, dot_checker_regex)) {
                continue;
            }
            else{
                print_command_error(program, args,
                    "Invalid namespace:");
                std::cout << ns << std::endl;
            }
           
                
            }
        }
    

    if (valid.empty() && !namespaces.empty()) {
        print_command_error(program, args,
            "No valid namespaces provided. Allowed:molecular_function, cellular_component, biological_process");
    }

    return valid;
}


int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    
    argparse::ArgumentParser program("GOoboparser");
    program.add_description("Elton's GOoboParser - GOobo file processor\nAuthor: Elton Ugbogu, University of Potsdam, 2025");
    
    program.add_argument("--consider-table")
        .help("Search for obsolete GO ids")
        .nargs(argparse::nargs_pattern::at_least_one);
    
    program.add_argument("--obsolete-stats")
        .help("Alias for --consider-table")
        .nargs(argparse::nargs_pattern::at_least_one);
    
    try {
        program.parse_args(argc, argv);  // Attempt to parse command-line arguments
    }
    catch (const std::exception &err) {  // If parsing fails
        print_command_error(program, args, err.what());  // Show error message
        return 1;  // Exit program with error code
    }
    // Collect whichever option is used
    std::vector<std::string> files_and_ns;
    if (program.is_used("--consider-table")) {
        files_and_ns = program.get<std::vector<std::string>>("--consider-table");
    } else if (program.is_used("--obsolete-stats")) {
        files_and_ns = program.get<std::vector<std::string>>("--obsolete-stats");
    } else {
        usage(program);
        return 0;
    }
    
    if (files_and_ns.empty()) {
        print_command_error(program, args, "No input files provided.");
        return 1;
    }
    
    bool mode_consider = program.is_used("--consider-table");
    bool mode_obsolete = program.is_used("--obsolete-stats");
    
    if (!mode_consider && !mode_obsolete)
    {
        print_command_error(program, args, "No mode selected. Use --consider-table or --obsolete-stats");
        return 1;
    }
    
    std::vector<std::string> input_files;
    std::vector<std::string> namespaces;
    auto valid_files = validate_files(program, args, files_and_ns);
    auto valid_namespaces = validate_namespaces(program, args, files_and_ns);
    
    for (const auto &item : valid_files)
    {
        std::cout << "Valid file - " << item << "\n";
        
        
    }
    for (const auto &n : valid_namespaces)
    {
        std::cout << "Valid namespace - " << n << "\n";
    
    }
    
    
    if (valid_files.empty())
        {
            return 1;
        }
    // Validate files and namespaces
    
    
    std::cout << "Mode: " << (mode_consider ? "consider-table" : "obsolete-stats") << "\n";

    return 0;
}
