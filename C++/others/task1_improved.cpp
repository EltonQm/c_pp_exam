#include <iostream>   // for input and output streams
#include <filesystem> // for checking if file exists
#include <string>     // for std::string
#include <regex>      // for regex validation (optional)

namespace fs = std::filesystem;

// Multi-line help message template with dynamic app name insertion
static const std::string HELP =
    R"(Ugbogu Elton FastaParser1 - search FASTA files
Author: Ugbogu Elton, University of Potsdam, 2024

Usage:
  __APPNAME__ --search PATTERN file.fasta
  __APPNAME__ --summary file.fasta
  __APPNAME__ --help

Arguments:
  --help     Show this help message
  --summary  Print lengths of all sequences
  --search   Search for pattern in sequence IDs
)";

// Display help or usage based on input flag
void usage(const std::string &appname, bool help = true)
{
    std::cout << std::regex_replace(HELP, std::regex("__APPNAME__"), appname) << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage(argv[0]);
        return 1;
    }

    std::string option = argv[1];
    std::string pattern;
    std::string filename;

    if (option == "--help" || option == "-h")
    {
        usage(argv[0]);
        return 0;
    }

    if (option != "--search" && option != "--summary")
    {
        std::cerr << "Error: Invalid option '" << option << "'.\n";
        usage(argv[0], false);
        return 1;
    }

    if (option == "--search")
    {
        if (argc < 4)
        {
            std::cerr << "Error: Missing pattern or filename for --search.\n";
            usage(argv[0], false);
            return 1;
        }
        pattern = argv[2];
        filename = argv[3];
    }
    else // --summary
    {
        if (argc < 3)
        {
            std::cerr << "Error: Missing filename for --summary.\n";
            usage(argv[0], false);
            return 1;
        }
        filename = argv[2];
    }

    // Explicit check for file existence
    if (!fs::exists(filename))
    {
        std::cerr << "Error: File does not exist → " << filename << "\n";
        return 1;
    }
    else
    {
        std::cout << "File found: " << filename << "\n";
    }

    return 0;
}
