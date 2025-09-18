// task1.cpp — Task 1: CLI with validation and subcommands
#include <iostream>
#include "task_utils.hpp"

int main(int argc, char **argv)
{
    const auto opts = parse_task1_cli(argc, argv);

    std::cout << "Mode: " << (opts.consider_table ? "consider-table" : "obsolete-stats") << "\n";
    std::cout << "Files:\n";
    for (auto &f : opts.obo_files)
        std::cout << "  " << f << "\n";
    if (!opts.namespaces.empty())
    {
        std::cout << "Namespaces:";
        for (auto &ns : opts.namespaces)
            std::cout << " " << ns;
        std::cout << "\n";
    }
    if (opts.name_pattern.has_value())
    {
        std::cout << "Pattern set\n";
    }
    // Actual processing for Task 1 will be in task2/task3 utilities.
    return 0;
}
