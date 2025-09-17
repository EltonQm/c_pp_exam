// task1.cpp
#include <iostream>
#include <filesystem>
#include <string>

void printHelp()
{
    std::cout << "Usage: ./task1 <option> <filename.obo[.gz]> [namespace]\n"
              << "Options:\n"
              << "  --consider-table : Display obsolete GO terms and their consider/replaced_by IDs.\n"
              << "  --obsoletes-stats : Show statistics of obsolete GO terms.\n"
              << "  --hierarchical_relationship : Show GO terms with is_a/part_of relationships.\n"
              << "Filename: Path to a valid .obo or .obo.gz file.\n"
              << "Namespace (optional): e.g., molecular_function, cellular_component, biological_process.\n"
              << "Example: ./task1 --consider-table go-2020-01.obo molecular_function\n";
}

bool isValidOption(const std::string &opt)
{
    return opt == "--consider-table" || opt == "--obsoletes-stats" || opt == "--hierarchical_relationship";
}

bool isValidFile(const std::string &filename)
{
    return std::filesystem::exists(filename) && (filename.ends_with(".obo") || filename.ends_with(".obo.gz"));
}

bool isValidNamespace(const std::string &ns)
{
    // Allow any namespace if provided, as exam specifies "or any other namespace"
    return true;
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        printHelp();
        return 1;
    }

    std::string option = argv[1];
    std::string filename = argv[2];
    std::string namespace_filter = (argc == 4) ? argv[3] : "";

    if (!isValidOption(option))
    {
        std::cerr << "Error: Invalid option '" << option << "'\n";
        printHelp();
        return 1;
    }

    if (!isValidFile(filename))
    {
        std::cerr << "Error: Invalid or non-existent file '" << filename << "'\n";
        printHelp();
        return 1;
    }

    if (!isValidNamespace(namespace_filter))
    {
        std::cerr << "Error: Invalid namespace '" << namespace_filter << "'\n";
        printHelp();
        return 1;
    }

    std::cout << "Arguments validated:\n"
              << "Option: " << option << "\n"
              << "Filename: " << filename << "\n"
              << "Namespace: " << (namespace_filter.empty() ? "all" : namespace_filter) << "\n";

    return 0;
}