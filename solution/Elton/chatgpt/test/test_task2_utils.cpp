#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../task2_utils.hpp"

TEST_CASE("considerTable extracts obsolete terms correctly")
{
    std::vector<std::string> lines = {
        "[Term]",
        "id: GO:0000001",
        "name: mitochondrion inheritance",
        "namespace: biological_process",
        "is_obsolete: true",
        "consider: GO:0007005",
        "",
        "[Term]",
        "id: GO:0000002",
        "name: mitochondrial genome maintenance",
        "namespace: biological_process",
        "is_obsolete: true",
        "",
        "[Term]",
        "id: GO:0000003",
        "name: reproduction",
        "namespace: biological_process",
        "",
        "[Term]",
        "id: GO:0000004",
        "name: mitochondrial DNA replication",
        "namespace: molecular_function",
        "is_obsolete: true",
        "consider: GO:0007006",
        "consider: GO:0007007",
        ""};

    SECTION("All obsolete terms without namespace filtering")
    {
        auto table = considerTable(lines, "");
        REQUIRE(table.size() == 3);
        CHECK(table[0][0] == "GO:0000001");
        CHECK(table[0][1] == "GO:0007005");
        CHECK(table[1][0] == "GO:0000002");
        CHECK(table[1][1] == "NA");
        CHECK(table[2][0] == "GO:0000004");
        CHECK(table[2][1] == "GO:0007006,GO:0007007");
    }

    SECTION("Only biological_process namespace")
    {
        auto table = considerTable(lines, "biological_process");
        REQUIRE(table.size() == 2);
        CHECK(table[0][0] == "GO:0000001");
        CHECK(table[1][0] == "GO:0000002");
    }

    SECTION("Only molecular_function namespace")
    {
        auto table = considerTable(lines, "molecular_function");
        REQUIRE(table.size() == 1);
        CHECK(table[0][0] == "GO:0000004");
        CHECK(table[0][1] == "GO:0007006,GO:0007007");
    }
}
