#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class GOCommandLineParser {
private:
  std::string option;
  std::string filename;
  std::string namespace_filter;

  std::vector<std::string> valid_options = {
      "--consider-table", "--replaced-by",
      "--obsolete-stats", "--hierarchical-relationship",
      "--subset",         "--xref-search"};

  std::vector<std::string> valid_namespaces = {
      "molecular_function", "cellular_component", "biological_process"};

public:
  void printHelp() {
    std::cout << "Gene Ontology OBO File Parser\n";
    std::cout << "Usage: program [OPTION] [INPUT_FILE] [NAMESPACE]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --consider-table           Show obsolete terms and their "
                 "consider alternatives\n";
    std::cout << "  --replaced-by             Show obsolete terms and their "
                 "replaced_by alternatives\n";
    std::cout
        << "  --obsolete-stats          Show statistics about obsolete terms\n";
    std::cout << "  --hierarchical-relationship Show GO terms with is_a and "
                 "part_of relationships\n";
    std::cout << "  --subset                  Show subset information\n";
    std::cout << "  --xref-search             Filter GO-ids and EC-ids from "
                 "Reactome database\n\n";
    std::cout << "Input file: Must end with .obo or .obo.gz\n\n";
    std::cout << "Optional namespace (filter results to specific namespace):\n";
    std::cout << "  molecular_function\n";
    std::cout << "  cellular_component\n";
    std::cout << "  biological_process\n\n";
    std::cout << "Examples:\n";
    std::cout << "  program --consider-table gene_ontology.obo\n";
    std::cout
        << "  program --replaced-by gene_ontology.obo.gz molecular_function\n";
    std::cout << "  program --obsolete-stats gene_ontology.obo\n";
  }

  bool isValidOption(const std::string &opt) {
    return std::find(valid_options.begin(), valid_options.end(), opt) !=
           valid_options.end();
  }

  bool isValidNamespace(const std::string &ns) {
    return std::find(valid_namespaces.begin(), valid_namespaces.end(), ns) !=
           valid_namespaces.end();
  }

  bool isValidOBOFile(const std::string &filename) {
    // Check file extension
    if (filename.length() < 4)
      return false;

    std::string extension = filename.substr(filename.length() - 4);
    std::string gz_extension =
        filename.length() >= 7 ? filename.substr(filename.length() - 7) : "";

    if (extension != ".obo" && gz_extension != ".obo.gz") {
      return false;
    }

    // Check if file exists
    std::ifstream file(filename);
    return file.good();
  }

  bool parseArguments(int argc, char *argv[]) {
    if (argc < 3) {
      std::cerr << "Error: Insufficient arguments\n";
      return false;
    }

    option = argv[1];
    filename = argv[2];

    // Optional namespace argument
    if (argc >= 4) {
      namespace_filter = argv[3];
    }

    // Validate option
    if (!isValidOption(option)) {
      std::cerr << "Error: Invalid option '" << option << "'\n";
      return false;
    }

    // Validate filename
    if (!isValidOBOFile(filename)) {
      std::cerr << "Error: Invalid input file '" << filename << "'\n";
      std::cerr << "File must exist and end with .obo or .obo.gz\n";
      return false;
    }

    // Validate namespace if provided
    if (!namespace_filter.empty() && !isValidNamespace(namespace_filter)) {
      std::cerr << "Error: Invalid namespace '" << namespace_filter << "'\n";
      return false;
    }

    return true;
  }

  void processCommand() {
    std::cout << "Processing command:\n";
    std::cout << "Option: " << option << "\n";
    std::cout << "File: " << filename << "\n";
    if (!namespace_filter.empty()) {
      std::cout << "Namespace filter: " << namespace_filter << "\n";
    }
    std::cout << "Command validated successfully!\n";
  }

  // Getters
  std::string getOption() const { return option; }
  std::string getFilename() const { return filename; }
  std::string getNamespace() const { return namespace_filter; }
};

int main(int argc, char *argv[]) {
  GOCommandLineParser parser;

  // Check if help is requested or insufficient arguments
  if (argc < 2 || std::string(argv[1]) == "--help" ||
      std::string(argv[1]) == "-h") {
    parser.printHelp();
    return 0;
  }

  // Parse and validate arguments
  if (!parser.parseArguments(argc, argv)) {
    std::cerr << "\nUse --help for usage information\n";
    return 1;
  }

  // Process the command
  parser.processCommand();

  return 0;
}