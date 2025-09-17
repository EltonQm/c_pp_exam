
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

// Include task2 and task3 implementations for testing
#include "../task2.cpp"
#include "../task3.cpp"

TEST_CASE("Task 1: Argument Validation", "[task1]")
{
    // Simulate command-line arguments
    SECTION("Valid arguments")
    {
        char *argv[] = {(char *)"task1", (char *)"--consider-table", (char *)"go.obo"};
        REQUIRE(main(3, argv) == 0);
    }
    SECTION("Invalid option")
    {
        char *argv[] = {(char *)"task1", (char *)"--invalid", (char *)"go.obo"};
        REQUIRE(main(3, argv) == 1);
    }
    SECTION("Invalid file")
    {
        char *argv[] = {(char *)"task1", (char *)"--consider-table", (char *)"dummy.obo"};
        REQUIRE(main(3, argv) == 1);
    }
    SECTION("Too few arguments")
    {
        char *argv[] = {(char *)"task1", (char *)"--consider-table"};
        REQUIRE(main(2, argv) == 1);
    }
}

TEST_CASE("Task 2: Parse Consider/Replaced_by Table", "[task2]")
{
    // Create a sample .obo file for testing
    std::ofstream out("test.obo");
    out << "[Term]\n"
        << "id: GO:0000001\n"
        << "namespace: molecular_function\n"
        << "is_obsolete: true\n"
        << "consider: GO:0000002\n"
        << "replaced_by: GO:0000003\n"
        << "[Term]\n"
        << "id: GO:0000004\n"
        << "namespace: biological_process\n"
        << "is_obsolete: true\n"
        << "[Term]\n"
        << "id: GO:0000005\n"
        << "namespace: cellular_component\n";
    out.close();

    SECTION("Consider table, no namespace filter")
    {
        auto table = parseTable("test.obo", "", false);
        REQUIRE(table.size() == 2);
        REQUIRE(table[0] == std::vector<std::string>{"GO:0000001", "GO:0000002"});
        REQUIRE(table[1] == std::vector<std::string>{"GO:0000004", "NA"});
    }
    SECTION("Replaced_by table, molecular_function filter")
    {
        auto table = parseTable("test.obo", "molecular_function", true);
        REQUIRE(table.size() == 1);
        REQUIRE(table[0] == std::vector<std::string>{"GO:0000001", "GO:0000003"});
    }

    std::filesystem::remove("test.obo");
}

TEST_CASE("Task 3: Advanced Functionalities", "[task3]")
{
    GooboParser parser("test.obo", "", "");
    std::ofstream out("test.obo");
    out << "[Term]\n"
        << "id: GO:0000001\n"
        << "namespace: molecular_function\n"
        << "is_obsolete: true\n"
        << "consider: GO:0000002\n"
        << "replaced_by: GO:0000003\n"
        << "subset: goslim_generic\n"
        << "xref: Reactome:12345\n"
        << "is_a: GO:0000006\n"
        << "part_of: GO:0000007\n"
        << "[Term]\n"
        << "id: GO:0000004\n"
        << "namespace: biological_process\n"
        << "is_obsolete: true\n"
        << "subset: goslim_plant\n";
    out.close();

    SECTION("Obsoletes Stats")
    {
        parser = GooboParser("test.obo", "", "stats.tab");
        parser.printObsoletesStats();
        std::ifstream in("stats.tab");
        std::string line;
        std::getline(in, line);
        REQUIRE(line == "consider\t1");
        std::getline(in, line);
        REQUIRE(line == "replaced_by\t1");
        std::getline(in, line);
        REQUIRE(line == "no-alternative\t1");
        in.close();
        std::filesystem::remove("stats.tab");
    }

    SECTION("Subset Counts")
    {
        parser = GooboParser("test.obo", "", "subset.tab");
        parser.printSubset();
        std::ifstream in("subset.tab");
        std::string line;
        std::getline(in, line);
        REQUIRE(line == "goslim_generic\t1");
        std::getline(in, line);
        REQUIRE(line == "goslim_plant\t1");
        in.close();
        std::filesystem::remove("subset.tab");
    }

    SECTION("Spreadsheet (Reactome)")
    {
        parser = GooboParser("test.obo", "molecular_function", "");
        auto table = parser.parseTable(false); // Use parseTable for simplicity
        REQUIRE(table.size() == 1);
        REQUIRE(table[0][0] == "GO:0000001");
    }

    SECTION("Hierarchical Relationships")
    {
        parser = GooboParser("test.obo", "molecular_function", "rel.tab");
        parser.printHierarchicalRelationship();
        std::ifstream in("rel.tab");
        std::string line;
        std::getline(in, line);
        REQUIRE(line == "GO:0000001\tis_a: GO:0000006\tpart_of: GO:0000007");
        in.close();
        std::filesystem::remove("rel.tab");
    }

    std::filesystem::remove("test.obo");
}
