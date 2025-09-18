// task2.cpp — Task 2: run consider-table over inputs
#include <iostream>
#include <regex>
#include "task_utils.hpp"
#include "task2_utils.hpp"

int main(int argc, char **argv)
{
    const auto opts = parse_task1_cli(argc, argv);
    if (!opts.consider_table)
    {
        std::cerr << "Error: Task 2 expects --consider-table mode.\n";
        return 1;
    }

    const std::regex *pat = opts.name_pattern ? &*opts.name_pattern : nullptr;
    const auto rows = build_consider_table(opts.obo_files, opts.namespaces, pat);

    for (const auto &r : rows)
    {
        // GO:obsolete_id <tab> alt_ids_csv <tab> parent_id
        std::cout << r.obsolete_id << '\t' << r.alternatives_csv << '\t' << r.parent_id << '\n';
    }
    return 0;
}
