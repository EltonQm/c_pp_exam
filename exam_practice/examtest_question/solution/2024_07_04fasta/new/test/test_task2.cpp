#include "test_helpers.hpp"
#include <string>
#include <iostream>

int main()
{
    banner("Task2 search functionality");

    int code;
    // Help
    auto out = run_capture("./FastaParser2 --help", code);
    assert_contains(out, "Usage:", "Task2 help");

    // Missing pattern
    out = run_capture("./FastaParser2 --search test/data/sars_mock1.fasta", code);
    assert_contains(out, "requires a pattern", "Task2 missing pattern");

    // Proper search
    out = run_capture("./FastaParser2 --search MAT test/data/sars_mock1.fasta", code);
    // Expect to see pattern column and at least one true, one false
    assert_contains(out, "MAT", "Task2 pattern in output");
    assert_true(out.find("true") != std::string::npos, "Task2 at least one true match");
    assert_true(out.find("false") != std::string::npos, "Task2 at least one false match");

    // Summary placeholder
    out = run_capture("./FastaParser2 --summary test/data/sars_mock1.fasta", code);
    assert_contains(out, "placeholder", "Task2 summary placeholder");

    std::cout << "Task2 tests passed.\n";
    return 0;
}
