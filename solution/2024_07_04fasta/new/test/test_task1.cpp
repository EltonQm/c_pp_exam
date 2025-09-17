#include "test_helpers.hpp"
#include <string>
#include <iostream>

int main()
{
    banner("Task1 CLI & help");

    int code;
    // 1. No arguments → usage/help
    auto out = run_capture("./FastaParser1", code);
    assert_contains(out, "Usage:", "Task1 no-arg usage");
    // 2. --help
    out = run_capture("./FastaParser1 --help", code);
    assert_contains(out, "FastaParser1", "Task1 help header");
    assert_contains(out, "Author:", "Task1 help author");
    // 3. Missing file for --search
    out = run_capture("./FastaParser1 --search MAT", code);
    assert_contains(out, "Error: --search requires a FASTA filename", "Task1 search missing file");
    // 4. Search with file (warn if missing)
    out = run_capture("./FastaParser1 --search MAT test/data/sars_mock1.fasta", code);
    assert_contains(out, "Search requested", "Task1 search skeleton");
    // 5. Summary with file
    out = run_capture("./FastaParser1 --summary test/data/sars_mock1.fasta", code);
    assert_contains(out, "Summary requested", "Task1 summary skeleton");

    std::cout << "Task1 tests passed.\n";
    return 0;
}
