#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../task_utils.hpp"
#include <sstream>
#include <fstream>
#include <unordered_set>

// Helper: create temporary OBO file
void write_temp_obo(const std::string &filename, const std::string &content)
{
    std::ofstream out(filename);
    out << content;
    out.close();
}

TEST_CASE("read_obo_lines reads lines from plain text file")
{
    std::string test_file = "test.obo";
    std::string content = "[Term]\nid: GO:0000001\nname: alpha\nnamespace: bio\n\n";
    write_temp_obo(test_file, content);

    auto lines = read_obo_lines(test_file);
    REQUIRE(lines.size() >= 4); // 4 lines + possibly EOF
    REQUIRE(lines[1].find("GO:0000001") != std::string::npos);

    std::remove(test_file.c_str());
}

TEST_CASE("print_go_id_and_name extracts GO ID and name")
{
    std::vector<std::string> lines = {
        "[Term]",
        "id: GO:1234567",
        "name: test name",
        "namespace: test",
        "",
    };

    std::ostringstream out;
    std::streambuf *old_cout = std::cout.rdbuf(out.rdbuf());

    print_go_id_and_name(lines);
    std::cout.rdbuf(old_cout);

    REQUIRE(out.str().find("GO:1234567") != std::string::npos);
    REQUIRE(out.str().find("test name") != std::string::npos);
}

TEST_CASE("parse_terms_with_metacyc_lines filters by MetaCyc")
{
    std::vector<std::string> lines = {
        "[Term]",
        "id: GO:0001111",
        "name: beta",
        "namespace: molecular_function",
        "xref: MetaCyc:RXN-1234",
        "",

        "[Term]",
        "id: GO:0002222",
        "name: gamma",
        "namespace: bio",
        "xref: MetaCyc:NOT-MATCHED",
        "",
    };

    std::unordered_set<std::string> ids = {"RXN-1234"};

    std::ostringstream out;
    std::streambuf *old_cout = std::cout.rdbuf(out.rdbuf());

    parse_terms_with_metacyc_lines(lines, ids);
    std::cout.rdbuf(old_cout);

    std::string result = out.str();
    REQUIRE(result.find("GO:0001111") != std::string::npos);
    REQUIRE(result.find("MetaCyc:RXN-1234") != std::string::npos);
    REQUIRE(result.find("GO:0002222") == std::string::npos);
}

TEST_CASE("count_obsolete_terms counts correct statistics")
{
    std::vector<std::string> lines = {
        "[Term]",
        "id: GO:1",
        "is_obsolete: true",
        "",
        "[Term]",
        "id: GO:2",
        "",
        "[Term]",
        "id: GO:3",
        "is_obsolete: true",
        "",
    };

    std::ostringstream out;
    std::streambuf *old_cout = std::cout.rdbuf(out.rdbuf());

    count_obsolete_terms(lines);
    std::cout.rdbuf(old_cout);

    std::string result = out.str();
    REQUIRE(result.find("Total terms: 3") != std::string::npos);
    REQUIRE(result.find("Obsolete terms: 2") != std::string::npos);
    REQUIRE(result.find("Non-obsolete terms: 1") != std::string::npos);
}
