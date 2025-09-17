#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Function to print usage/help message
void printHelp()
{
    std::cout << "Ugbogu Elton FastaParser1 - search FASTA files\n"
              << "Author: Ugbogu Elton, University of Potsdam, 2024\n\n"
              << "Usage:\n"
              << "  ./task --search PATTERN file.fasta\n"
              << "  ./task --summary file.fasta\n"
              << "  ./task --help\n\n"
              << "Arguments:\n"
              << "  --help     Show this help message\n"
              << "  --summary  Print lengths of all sequences\n"
              << "  --search   Search for pattern in sequence IDs\n";
}

int main(int argc, char *argv[])
{
    // Show help if no arguments
    if (argc < 2)
    {
        printHelp();
        return 1;
    }

    std::string option = argv[1];

    if (option == "--help")
    {
        printHelp();
        return 0;
    }

    if (option != "--search" && option != "--summary")
    {
        std::cerr << "Error: Invalid option '" << option << "'.\n";
        printHelp();
        return 1;
    }

    if (option == "--search")
    {
        // Needs at least 2 more arguments: pattern and one or more files
        if (argc < 4)
        {
            std::cerr << "Error: Missing pattern or filename for --search.\n";
            printHelp();
            return 1;
        }

        std::string pattern = argv[2];

        // Iterate through all remaining arguments as filenames
        for (int i = 3; i < argc; ++i)
        {
            std::string filename = argv[i];
            fs::path filePath(filename);

            if (!fs::exists(filePath))
            {
                std::cerr << "Error: File does not exist → " << filename << "\n";
            }
            else
            {
                std::cout << "File found: " << filename << "\n";
            }
        }
    }

    else if (option == "--summary")
    {
        // Should be exactly one filename after --summary
        if (argc != 3)
        {
            std::cerr << "Error: Missing filename for --summary.\n";
            printHelp();
            return 1;
        }

        std::string filename = argv[2];
        fs::path filePath(filename);

        if (!fs::exists(filePath))
        {
            std::cerr << "Error: File does not exist → " << filename << "\n";
        }
        else
        {
            std::cout << "File found: " << filename << "\n";
        }
    }

    return 0;
}
