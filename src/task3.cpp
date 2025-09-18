// task3.cpp — Task 3: stats + optional --output FILE.tab
#include <iostream>
#include "task_utils.hpp"
#include "task3_utils.hpp"

static std::vector<std::vector<std::string>>
stats_to_rows(const std::map<std::string, NamespaceStats> &m)
{
    std::vector<std::vector<std::string>> rows;
    rows.reserve(m.size() + 1);
    rows.push_back({"namespace", "obsolete_total", "with_alternatives"});
    for (const auto &[ns, st] : m)
    {
        rows.push_back({ns, std::to_string(st.obsolete_total), std::to_string(st.with_alternatives)});
    }
    return rows;
}

int main(int argc, char **argv)
{
    auto opts = parse_task1_cli(argc, argv);

    // Task 3 uses obsolete-stats mode (and optional --output FILE.tab)
    if (!opts.obsolete_stats)
    {
        std::cerr << "Error: Task 3 expects --obsolete-stats mode.\n";
        return 1;
    }

    const std::regex *pat = opts.name_pattern ? &*opts.name_pattern : nullptr;
    const auto stats = compute_obsolete_stats(opts.obo_files, opts.namespaces, pat);
    const auto rows = stats_to_rows(stats);

    if (opts.output_tab)
    {
        if (!write_tab_file(*opts.output_tab, rows))
            return 1;
    }
    else
    {
        // Print to terminal
        for (const auto &r : rows)
        {
            for (size_t i = 0; i < r.size(); ++i)
            {
                if (i)
                    std::cout << '\t';
                std::cout << r[i];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
