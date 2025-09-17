#include "task3_utils.hpp"
#include <iostream>
#include <string>
#include <vector>

void printHelp() {
  std::cout << "Gene Ontology OBO File Parser - Task 3 (Advanced)\n";
  std::cout << "Usage: task3 [OPTION] [INPUT_FILE] [NAMESPACE]\n\n";
  std::cout << "Options:\n";
  std::cout << "  --consider-table             Show obsolete terms and their "
               "consider alternatives\n";
  std::cout << "  --replaced-by               Show obsolete terms and their "
               "replaced_by alternatives\n";
  std::cout
      << "  --obsolete-stats            Show statistics about obsolete terms\n";
  std::cout << "  --hierarchical-relationship Show GO terms with is_a and "
               "part_of relationships\n";
  std::cout
      << "  --subset                    Show subset information and counts\n";
  std::cout << "  --xref-search               Filter GO-ids and EC-ids from "
               "Reactome database\n\n";
  std::cout << "Input file: Must end with .obo or .obo.gz\n\n";
  std::cout << "Optional namespace (filter results to specific namespace):\n";
  std::cout << "  molecular_function\n";
  std::cout << "  cellular_component\n";
  std::cout << "  biological_process\n\n";
  std::cout << "Examples:\n";
  std::cout << "  task3 --obsolete-stats gene_ontology.obo\n";
  std::cout << "  task3 --hierarchical-relationship gene_ontology.obo "
               "molecular_function\n";
  std::cout << "  task3 --subset gene_ontology.obo\n";
  std::cout << "  task3 --xref-search gene_ontology.obo\n";
}

bool isValidOption(const std::string &option) {
  return (option == "--consider-table" || option == "--replaced-by" ||
          option == "--obsolete-stats" ||
          option == "--hierarchical-relationship" || option == "--subset" ||
          option == "--xref-search");
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

  // Parse the OBO file with extended functionality
  GOAnalyzer analyzer;
  if (!analyzer.parseFileExtended(filename)) {
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

    auto table = analyzer.considerTable(namespace_filter);
    std::vector<std::string> headers = {"GO ID", "Term Name",
                                        "Consider Alternative"};
    analyzer.printTable(table, headers);

    std::cout << "\nTotal obsolete terms with consider alternatives: "
              << table.size() << std::endl;
  } else if (option == "--replaced-by") {
    std::cout << "\nObsolete GO Terms with Replaced By Alternatives";
    if (!namespace_filter.empty()) {
      std::cout << " (Namespace: " << namespace_filter << ")";
    }
    std::cout << ":\n";
    std::cout << std::string(80, '=') << "\n";

    auto table = analyzer.replacedByTable(namespace_filter);
    std::vector<std::string> headers = {"GO ID", "Term Name", "Replaced By"};
    analyzer.printTable(table, headers);

    std::cout << "\nTotal obsolete terms with replaced_by alternatives: "
              << table.size() << std::endl;
  } else if (option == "--obsolete-stats") {
    auto stats = analyzer.getObsoleteStats(namespace_filter);
    analyzer.displayObsoleteStats(stats, namespace_filter);
  } else if (option == "--hierarchical-relationship") {
    auto relationships =
        analyzer.getHierarchicalRelationships(namespace_filter);
    analyzer.displayHierarchicalRelationships(relationships);
  } else if (option == "--subset") {
    auto subsets = analyzer.getSubsetCounts(namespace_filter);
    analyzer.displaySubsetInfo(subsets);
  } else if (option == "--xref-search") {
    std::cout << "\nSearching for Reactome cross-references:\n";
    auto reactome_xrefs = analyzer.xrefSearch("Reactome");
    analyzer.displayXrefResults(reactome_xrefs, "Reactome");

    std::cout << "\nSearching for EC cross-references:\n";
    auto ec_xrefs = analyzer.xrefSearch("EC");
    analyzer.displayXrefResults(ec_xrefs, "EC");
  }

  return 0;
}