// GooboParser2.cpp - Gene Ontology OBO File Parser
// Purpose: Parse GO OBO files to extract terms linked to specific MetaCyc IDs
// OBO Format: Open Biomedical Ontologies format used for biological ontologies

#include <argparse/argparse.hpp> // Command-line argument parsing library
#include <iostream>              // Standard input/output streams
#include <fstream>               // File stream operations
#include <filesystem>            // File system utilities (checking file existence)
#include <vector>                // Dynamic arrays
#include <tuple>                 // Fixed-size heterogeneous containers
#include <string>                // String operations
#include <algorithm>             // STL algorithms (all_of)

using namespace std;
namespace fs = std::filesystem; // Shorthand for filesystem namespace

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

/**
 * Check if a string contains only digit characters
 * @param s - String to validate
 * @return true if all characters are digits, false otherwise
 * Used to validate date components (year, month, day)
 */
bool isAllDigits(const string &s)
{
    // Check string is not empty AND all characters satisfy isdigit predicate
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

/**
 * Validate OBO filename format
 * Expected formats:
 *   - filename.obo.YYYY-MM-DD
 *   - filename.obo.YYYY-MM-DD.gz
 * @param filename - Path to validate
 * @return true if filename matches expected format
 */
bool isValidOBOFilename(const string &filename)
{
    // Find the last occurrence of ".obo." in the filename
    size_t pos = filename.rfind(".obo.");
    if (pos == string::npos)
        return false; // ".obo." not found

    // Extract everything after ".obo." (should be date and optional .gz)
    string datePart = filename.substr(pos + 5);

    // Remove .gz extension if present to isolate the date
    if (datePart.size() > 3 && datePart.substr(datePart.size() - 3) == ".gz")
    {
        datePart = datePart.substr(0, datePart.size() - 3);
    }

    // Date should be exactly 10 characters: YYYY-MM-DD
    // Check for proper hyphen positions at indices 4 and 7
    if (datePart.size() != 10 || datePart[4] != '-' || datePart[7] != '-')
        return false;

    // Extract and validate individual date components
    string year = datePart.substr(0, 4);  // Characters 0-3
    string month = datePart.substr(5, 2); // Characters 5-6
    string day = datePart.substr(8, 2);   // Characters 8-9

    // All components must be numeric
    return isAllDigits(year) && isAllDigits(month) && isAllDigits(day);
}

// =============================================================================
// GOOBOPARSER CLASS - Main parsing logic
// =============================================================================

/**
 * GooboParser class
 * Responsible for parsing Gene Ontology OBO files and extracting
 * information about GO terms linked to specific MetaCyc identifiers
 */
class GooboParser
{
private:
    string filename; // Path to the OBO file to parse

public:
    /**
     * Constructor
     * @param file - Path to the OBO file
     */
    GooboParser(const string &file) : filename(file) {}

    /**
     * Extract all GO terms that reference a specific MetaCyc ID
     *
     * OBO File Structure:
     * [Term]
     * id: GO:0000001
     * name: mitochondrion inheritance
     * namespace: biological_process
     * xref: MetaCyc:RXN-12345
     * ...
     *
     * @param metacycID - The MetaCyc identifier to search for (e.g., "RXN-12345")
     * @return Vector of tuples containing (GO_ID, GO_Name, Namespace, Xref_line)
     */
    vector<tuple<string, string, string, string>> getMetaCycEntries(const string &metacycID)
    {
        // Container for results - each tuple holds one matching GO term's data
        vector<tuple<string, string, string, string>> results;

        // Open the OBO file for reading
        ifstream fin(filename);
        if (!fin)
        {
            cerr << "Error: Unable to open file: " << filename << endl;
            return results; // Return empty vector on file open failure
        }

        // Variables to store current term's data as we parse
        string line;    // Current line being processed
        string go_id;   // GO identifier (e.g., "GO:0000001")
        string go_name; // Human-readable name of the GO term
        string go_ns;   // Namespace (biological_process, molecular_function, cellular_component)
        string go_xref; // Cross-reference line containing MetaCyc ID

        bool insideTerm = false; // Flag: are we currently parsing a [Term] block?
        int entryCount = 0;      // Counter for development/debugging purposes

        // Read file line by line
        while (getline(fin, line))
        {
            // Check if we're starting a new term block
            if (line == "[Term]")
            {
                insideTerm = true;
                // Reset all variables for the new term
                go_id = go_name = go_ns = go_xref = "";
                continue; // Move to next line
            }

            // Empty line signals end of current term block
            if (line.empty() && insideTerm)
            {
                insideTerm = false;

                // If this term has a matching MetaCyc xref, save it
                if (!go_xref.empty())
                {
                    results.emplace_back(go_id, go_name, go_ns, go_xref);
                    // Uncomment next line during development to limit results:
                    // if (++entryCount >= 1000) break;
                }
                continue; // Move to next line
            }

            // Parse fields within a [Term] block
            if (insideTerm)
            {
                // Check for "id: " at start of line (rfind with pos 0 = starts with)
                if (line.rfind("id: ", 0) == 0)
                    go_id = line.substr(4); // Extract everything after "id: "

                // Check for "name: " field
                else if (line.rfind("name: ", 0) == 0)
                    go_name = line.substr(6); // Extract everything after "name: "

                // Check for "namespace: " field
                else if (line.rfind("namespace: ", 0) == 0)
                    go_ns = line.substr(11); // Extract everything after "namespace: "

                // Check for MetaCyc cross-reference containing our target ID
                else if (line.rfind("xref: MetaCyc:", 0) == 0 && line.find(metacycID) != string::npos)
                {
                    go_xref = line.substr(6); // Extract everything after "xref: "
                }
            }
        }

        return results;
    }
};

// =============================================================================
// COMMAND-LINE INTERFACE
// =============================================================================

/**
 * Print usage instructions
 * @param progname - Name of the executable
 */
void usage(const string &progname)
{
    cout << "Usage:\n"
         << "  " << progname << " FILENAME --get-metacyc METACYC_ID\n";
}

/**
 * Print detailed help message
 * @param progname - Name of the executable
 */
void help(const string &progname)
{
    cout << "GooboParser - Author: YourName\n";
    usage(progname);
    cout << "\nArguments:\n"
         << "  FILENAME           Path to .obo.YYYY-MM-DD or .obo.YYYY-MM-DD.gz\n"
         << "  --get-metacyc ID   Search MetaCyc ID (must contain 'RXN' or 'PWY')\n";
}

// =============================================================================
// MAIN PROGRAM ENTRY POINT
// =============================================================================

int main(int argc, char *argv[])
{
    // Initialize argument parser with program name
    argparse::ArgumentParser program("GooboParser");

    // Define positional argument for filename
    program.add_argument("filename")
        .help("Path to the .obo file");

    // Define optional argument for MetaCyc ID search
    program.add_argument("--get-metacyc")
        .help("MetaCyc ID to search for")
        .nargs(1); // Expects exactly one value after the flag

    // Attempt to parse command-line arguments
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const exception &err)
    {
        // Handle parsing errors (missing required args, invalid format, etc.)
        cerr << "Argument error: " << err.what() << endl;
        help(argv[0]);
        return 1; // Exit with error code
    }

    // Extract filename from parsed arguments
    string filename = program.get<string>("filename");

    // Validate filename format (must end with .obo.YYYY-MM-DD[.gz])
    if (!isValidOBOFilename(filename))
    {
        cerr << "Error: File must end with .obo.YYYY-MM-DD or .obo.YYYY-MM-DD.gz\n";
        return 1;
    }

    // Check that file actually exists on disk
    if (!fs::exists(filename))
    {
        cerr << "Error: File '" << filename << "' does not exist.\n";
        return 1;
    }

    // ==========================================================================
    // TASK EXECUTION: --get-metacyc
    // ==========================================================================

    // Check if --get-metacyc flag was provided
    if (program.is_used("--get-metacyc"))
    {
        // Extract MetaCyc ID from arguments (stored as vector, take first element)
        string metaID = program.get<vector<string>>("--get-metacyc")[0];

        // Validate MetaCyc ID format - must contain either "RXN" or "PWY"
        // RXN = reaction identifiers, PWY = pathway identifiers
        if (metaID.find("RXN") == string::npos && metaID.find("PWY") == string::npos)
        {
            cerr << "Error: MetaCyc ID must contain 'RXN' or 'PWY'.\n";
            return 1;
        }

        // Create parser instance and search for the MetaCyc ID
        GooboParser parser(filename);
        auto results = parser.getMetaCycEntries(metaID);

        // Display results
        if (results.empty())
        {
            cout << "No results found for MetaCyc ID: " << metaID << "\n";
        }
        else
        {
            // Print each result as tab-separated values
            // Format: GO_ID<tab>Name<tab>Namespace<tab>Xref_line
            for (const auto &[go_id, name, ns, xref] : results)
            {
                cout << go_id << "\t" << name << "\t" << ns << "\t" << xref << "\n";
            }
        }
    }
    else
    {
        // No valid task specified
        cerr << "Error: You must use --get-metacyc for Task 2.\n";
        usage(argv[0]);
        return 1;
    }

    return 0; // Successful execution
}