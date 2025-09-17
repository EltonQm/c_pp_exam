#include "test_helpers.hpp"
#include <string>
#include <iostream>

int main()
{
    banner("Task3 multi-file search & summary");

    int code;
    // Help
    auto out = run_capture("./FastaParser3 --help", code);
    assert_contains(out, "Usage:", "Task3 help banner");

    // Search across two files
    out = run_capture("./FastaParser3 --search MAT test/data/sars_mock1.fasta test/data/sars_mock2.fasta", code);
    assert_contains(out, "MAT", "Task3 search output contains pattern");
    // Should have at least one 'true' (MAT present)
    assert_true(out.find("true") != std::string::npos, "Task3 search at least one match");

    // Summary across two files
    out = run_capture("./FastaParser3 --summary test/data/sars_mock1.fasta test/data/sars_mock2.fasta", code);
    // Expect sequence IDs present
    assert_contains(out, "sp|P11111|TEST1_SAMPLE1", "Task3 summary id1");
    assert_contains(out, "sp|X00001|ALPHA_SAMPLE2", "Task3 summary id2");

    // Missing file warning
    out = run_capture("./FastaParser3 --summary test/data/NO_SUCH.fasta", code);
    assert_contains(out, "Warning", "Task3 missing file warning");

    std::cout << "Task3 tests passed.\n";
    return 0;
}
