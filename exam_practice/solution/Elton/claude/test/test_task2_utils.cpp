#define CATCH_CONFIG_MAIN
#include "../task2_utils.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

// Helper function to create a test OBO file
void createTestOBOFile(const std::string &filename,
                       const std::string &content) {
  std::ofstream file(filename);
  file << content;
  file.close();
}

TEST_CASE("OBOParser basic functionality", "[OBOParser]") {
  // Create a test OBO file
  std::string test_content = R"(
format-version: 1.2
data-version: test

[Term]
id: GO:0000001
name: mitochondrion inheritance
namespace: biological_process
def: "The distribution of mitochondria."
is_obsolete: true
consider: GO:0048308
consider: GO:0048311

[Term]
id: GO:0000002
name: mitochondrial genome maintenance
namespace: biological_process
def: "The maintenance of the structure."
is_a: GO:0007005 ! mitochondrion organization

[Term]
id: GO:0000003
name: reproduction
namespace: biological_process
def: "The production of new individuals."
is_obsolete: true
replaced_by: GO:0000004

[Term]
id: GO:0000004
name: biological reproduction
namespace: biological_process
def: "A reproductive process."
)";

  createTestOBOFile("test_file.obo", test_content);

  SECTION("File parsing") {
    OBOParser parser;
    REQUIRE(parser.parseFile("test_file.obo"));

    const auto &terms = parser.getTerms();
    REQUIRE(terms.size() == 4);

    // Check first term
    auto &term1 = terms[0];
    REQUIRE(term1.id == "GO:0000001");
    REQUIRE(term1.name == "mitochondrion inheritance");
    REQUIRE(term1.namespace_name == "biological_process");
    REQUIRE(term1.is_obsolete == true);
    REQUIRE(term1.consider.size() == 2);
    REQUIRE(term1.consider[0] == "GO:0048308");
    REQUIRE(term1.consider[1] == "GO:0048311");
  }

  SECTION("Consider table functionality") {
    OBOParser parser;
    parser.parseFile("test_file.obo");

    auto consider_table = parser.considerTable();
    REQUIRE(consider_table.size() == 2); // Two consider entries for GO:0000001

    REQUIRE(consider_table[0][0] == "GO:0000001");
    REQUIRE(consider_table[0][2] == "GO:0048308");
    REQUIRE(consider_table[1][0] == "GO:0000001");
    REQUIRE(consider_table[1][2] == "GO:0048311");
  }

  SECTION("Replaced by table functionality") {
    OBOParser parser;
    parser.parseFile("test_file.obo");

    auto replaced_by_table = parser.replacedByTable();
    REQUIRE(replaced_by_table.size() ==
            1); // One replaced_by entry for GO:0000003

    REQUIRE(replaced_by_table[0][0] == "GO:0000003");
    REQUIRE(replaced_by_table[0][2] == "GO:0000004");
  }

  SECTION("Namespace filtering") {
    OBOParser parser;
    parser.parseFile("test_file.obo");

    auto filtered_terms = parser.filterByNamespace("biological_process");
    REQUIRE(filtered_terms.size() ==
            4); // All terms in this test are biological_process

    auto filtered_terms_empty = parser.filterByNamespace("molecular_function");
    REQUIRE(filtered_terms_empty.size() == 0); // No molecular_function terms
  }

  // Clean up
  std::remove("test_file.obo");
}

TEST_CASE("OBOParser edge cases", "[OBOParser]") {
  SECTION("Empty file") {
    createTestOBOFile("empty_file.obo", "");

    OBOParser parser;
    REQUIRE(parser.parseFile("empty_file.obo"));
    REQUIRE(parser.getTerms().empty());

    std::remove("empty_file.obo");
  }

  SECTION("Non-existent file") {
    OBOParser parser;
    REQUIRE_FALSE(parser.parseFile("non_existent_file.obo"));
  }

  SECTION("Obsolete terms with no alternatives") {
    std::string test_content = R"(
[Term]
id: GO:0000001
name: test term
namespace: biological_process
is_obsolete: true
)";

    createTestOBOFile("no_alternatives.obo", test_content);

    OBOParser parser;
    parser.parseFile("no_alternatives.obo");

    auto consider_table = parser.considerTable();
    REQUIRE(consider_table.size() == 1);
    REQUIRE(consider_table[0][2] == "NA");

    auto replaced_by_table = parser.replacedByTable();
    REQUIRE(replaced_by_table.size() == 1);
    REQUIRE(replaced_by_table[0][2] == "NA");

    std::remove("no_alternatives.obo");
  }
}

TEST_CASE("String utilities", "[utilities]") {
  OBOParser parser;

  SECTION("Table printing with empty data") {
    std::vector<std::vector<std::string>> empty_table;
    std::vector<std::string> headers = {"Header1", "Header2"};

    // This should not crash
    parser.printTable(empty_table, headers);
  }

  SECTION("Table printing with data") {
    std::vector<std::vector<std::string>> table = {{"Row1Col1", "Row1Col2"},
                                                   {"Row2Col1", "Row2Col2"}};
    std::vector<std::string> headers = {"Header1", "Header2"};

    // This should not crash
    parser.printTable(table, headers);
  }
}