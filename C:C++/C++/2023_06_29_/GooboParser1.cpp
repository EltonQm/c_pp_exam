// Include the argparse library for command-line argument parsing
#include <argparse/argparse.hpp> // Argument parsing library for C++

// Include standard input/output library for console operations
#include <iostream> // Standard I/O

// Include filesystem library for file existence checks
#include <filesystem> // To check if files exist

// Include regex library for validating filename format
#include <regex> // To validate the filename format

// File: GooboParser1.cpp
//
// Purpose: Console application to parse GO.obo files.
// Functionality: Validates command-line arguments using argparse,
// checks file existence and naming format, and handles two tasks:
// --get-metacyc (search by MetaCyc ID) and --tab-metacyc (print mappings).

// Use standard namespace for convenience
using namespace std;

// Alias for filesystem namespace to simplify file path handling
namespace fs = std::filesystem; // Alias for easier file path handling

// Function to print usage instructions for the program
void usage(const string &progname)
{
    // Output usage syntax to console
    cout << "Usage:\n"
         << "  " << progname << " FILENAME --get-metacyc METACYC_ID\n"
         << "  " << progname << " FILENAME --tab-metacyc\n";
}

// Function to print detailed help message with context
void help(const string &progname)
{
    // Print program name and author
    cout << "GooboParser - Author: Elton Ugbogu\n";
    // Call usage function to display syntax
    usage(progname);
    // Provide detailed argument descriptions
    cout << "\nArguments:\n"
         << "  FILENAME           Path to .obo.YYYY-MM-DD or .obo.YYYY-MM-DD.gz\n"
         << "  --get-metacyc ID   Search MetaCyc ID (must contain 'RXN' or 'PWY')\n"
         << "  --tab-metacyc      Print GO-EC-MetaCyc mapping table\n";
}

// Function to validate OBO filename format
// Expected format: *.obo.YYYY-MM-DD or *.obo.YYYY-MM-DD.gz
bool isValidOBOFilename(const string &filename)
{
    // Define regex pattern for filename validation
    // Breakdown:
    // .*                  : Any characters (for path)
    // \.obo\.             : Literal ".obo."
    // \d{4}-\d{2}-\d{2}  : YYYY-MM-DD date format
    // (\.gz)?             : Optional .gz extension
    regex pattern(R"(.*\.obo\.\d{4}-\d{2}-\d{2}(\.gz)?$)");

    // Return true if filename matches the pattern
    return regex_match(filename, pattern);
}

// Main function: entry point of the program
int main(int argc, char *argv[])
{
    // Initialize argparse object to handle command-line arguments
    argparse::ArgumentParser program("GooboParser");

    // Add required positional argument for the OBO file path
    program.add_argument("filename")
        // Provide help text for the filename argument
        .help("Path to the GO OBO file (.obo.YYYY-MM-DD or .obo.YYYY-MM-DD.gz)");

    // Add optional --get-metacyc flag that requires one argument (MetaCyc ID)
    program.add_argument("--get-metacyc")
        // Provide help text for the --get-metacyc flag
        .help("Search for a specific MetaCyc ID")
        // Specify that one value is required after the flag
        .nargs(1);

    // Add optional --tab-metacyc flag as a boolean switch
    program.add_argument("--tab-metacyc")
        // Provide help text for the --tab-metacyc flag
        .help("Print GO-EC-MetaCyc mappings")
        // Set default value to false if flag is not provided
        .default_value(false)
        // Set value to true if flag is present
        .implicit_value(true);

    // Attempt to parse command-line arguments
    try
    {
        // Parse arguments passed to the program
        program.parse_args(argc, argv);
    }
    // Handle parsing errors (e.g., missing or invalid arguments)
    catch (const std::exception &err)
    {
        // Output error message to stderr
        cerr << "Argument error: " << err.what() << endl;
        // Display help message
        help(argv[0]);
        // Return non-zero exit code to indicate failure
        return 1;
    }

    // Extract the filename from parsed arguments
    string filename = program.get<string>("filename");

    // Validate filename format using regex
    if (!isValidOBOFilename(filename))
    {
        // Output error if filename does not match required format
        cerr << "Error: File must end with .obo.YYYY-MM-DD or .obo.YYYY-MM-DD.gz\n";
        // Return non-zero exit code
        return 1;
    }

    // Check if the specified file exists on disk
    if (!fs::exists(filename))
    {
        // Output error if file does not exist
        cerr << "Error: File '" << filename << "' does not exist.\n";
        // Return non-zero exit code
        return 1;
    }

    // Check if --get-metacyc flag was used
    bool hasGet = program.is_used("--get-metacyc");
    // Get value of --tab-metacyc flag (true if specified)
    bool hasTab = program.get<bool>("--tab-metacyc");

    // Ensure at least one task flag is provided
    if (!hasGet && !hasTab)
    {
        // Output error if no task is specified
        cerr << "Error: You must specify either --get-metacyc or --tab-metacyc.\n";
        // Display usage instructions
        usage(argv[0]);
        // Return non-zero exit code
        return 1;
    }

    // Ensure only one task flag is used at a time
    if (hasGet && hasTab)
    {
        // Output error if both flags are used
        cerr << "Error: --get-metacyc and --tab-metacyc cannot be used together.\n";
        // Return non-zero exit code
        return 1;
    }

    // Handle --get-metacyc task
    if (hasGet)
    {
        // Extract MetaCyc ID from the vector of arguments
        string metaID = program.get<vector<string>>("--get-metacyc")[0];

        // Validate that MetaCyc ID contains 'RXN' or 'PWY'
        if (metaID.find("RXN") == string::npos && metaID.find("PWY") == string::npos)
        {
            // Output error if ID is invalid
            cerr << "Error: MetaCyc ID must contain 'RXN' or 'PWY'.\n";
            // Return non-zero exit code
            return 1;
        }

        // Output success message for --get-metacyc task
        cout << "[OK] Task selected: --get-metacyc " << metaID << endl;
    }

    // Handle --tab-metacyc task
    if (hasTab)
    {
        // Output success message for --tab-metacyc task
        cout << "[OK] Task selected: --tab-metacyc" << endl;
    }

    // Return zero to indicate successful execution
    return 0;
}