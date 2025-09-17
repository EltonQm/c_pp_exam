#include "task2_utils.hpp"
#include <iostream>
#include <string>
#include <vector>

void printHelp() {
  std::cout << "Gene Ontology OBO File Parser - Task 2\n";
  std::cout << "Usage: task2 [OPTION] [INPUT_FILE] [NAMESPACE]\n\n";
  std::cout << "Options:\n";
  std::cout << "  --consider-table          Show obsolete terms and their "
               "consider alternatives\n";
  std::cout << "  --replaced-by            Show obsolete terms and their "
               "replaced_by alternatives\n\n";
  std::cout << "Input file: Must end with .obo or .obo.gz\n\n";
  std::cout << "Optional namespace (filter results to specific namespace):\n";
  std::cout << "  molecular_function\n";
  std::cout << "  cellular_component\n";
  std::cout << "  biological_process\n\n";
  std::cout << "Examples:\n";
  std::cout << "  task2 --consider-table gene_ontology.obo\n";
  std::cout
      << "  task2 --replaced-by gene_ontology.obo.gz molecular_function\n";
}

bool isValidOption(const std::string &option) {
  return (option == "--consider-table" || option == "--replaced-by");
}

bool isValidNamespace(const std::string &ns) {
  return (ns == "molecular_function" || ns == "cellular_component" ||
          ns == "biological_process");
}

bool isValidOBOFile(const std::string &filename) {
  if (filename.length() < 4)
    return false;

  std::string extension = filename.substr(filename.length() - 4);
  std::string gz_extension =
      filename.length() >= 7 ? filename.substr(filename.length() - 7) : "";

  return (extension == ".obo" || gz_extension == ".obo.gz");
}

int main(int argc, char *argv[]) {
  // Check for help or insufficient arguments
  if (argc < 2 || std::string(argv[1]) == "--help" ||
      std::string(argv[1]) == "-h") {
    printHelp();
    return 0;
  }

  if (argc < 3) {
    std::cerr << "Error: Insufficient arguments\n";
    printHelp();
    return 1;
  }

  std::string option = argv[1];
  std::string filename = argv[2];
  std::string namespace_filter = "";

  if (argc >= 4) {
    namespace_filter = argv[3];
  }

  // Validate arguments
  if (!isValidOption(option)) {
    std::cerr << "Error: Invalid option '" << option << "'\n";
    printHelp();
    return 1;
  }

  if (!isValidOBOFile(filename)) {
    std::cerr << "Error: Invalid input file '" << filename << "'\n";
    std::cerr << "File must end with .obo or .obo.gz\n";
    return 1;
  }

  if (!namespace_filter.empty() && !isValidNamespace(namespace_filter)) {
    std::cerr << "Error: Invalid namespace '" << namespace_filter << "'\n";
    printHelp();
    return 1;
  }

  // Parse the OBO file
  OBOParser parser;
  if (!parser.parseFile(filename)) {
    std::cerr << "Error: Failed to parse OBO file\n";
    return 1;
  }

  // Process the request
  if (option == "--consider-table") {
    std::cout << "\nObsolete GO Terms with Consider Alternatives";
    if (!namespace_filter.empty()) {
      std::cout << " (Namespace: " << namespace_filter << ")";
    }
    std::cout << ":\n";
    std::cout << std::string(80, '=') << "\n";

    auto table = parser.considerTable(namespace_filter);
    std::vector<std::string> headers = {"GO ID", "Term Name",
                                        "Consider Alternative"};
    parser.printTable(table, headers);

    std::cout << "\nTotal obsolete terms with consider alternatives: "
              << table.size() << std::endl;
  } else if (option == "--replaced-by") {
    std::cout << "\nObsolete GO Terms with Replaced By Alternatives";
    if (!namespace_filter.empty()) {
      std::cout << " (Namespace: " << namespace_filter << ")";
    }
    std::cout << ":\n";
    std::cout << std::string(80, '=') << "\n";

    auto table = parser.replacedByTable(namespace_filter);
    std::vector<std::string> headers = {"GO ID", "Term Name", "Replaced By"};
    parser.printTable(table, headers);

    std::cout << "\nTotal obsolete terms with replaced_by alternatives: "
              << table.size() << std::endl;
  }

  return 0;
}