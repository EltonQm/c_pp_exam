#include "task3_utils.hpp"
#include "task2_utils.hpp" // for read_obo_lines(file)
#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("task3",
                                     "GO OBO analytics: xref search, obsolete stats, hierarchy extraction");

    // Mutually exclusive primary modes (enforce manually)
    program.add_argument("--xrefsearch")
        .help("List GO IDs that have Reactome xrefs plus associated Reactome and EC IDs.")
        .nargs(0);
    program.add_argument("--obsolete-stats")
        .help("Show counts of obsolete terms (consider / replaced_by / none).")
        .nargs(0);
    program.add_argument("--hierarchy")
        .help("List is_a and part_of hierarchical relationships.")
        .nargs(0);

    program.add_argument("--limit")
        .help("Limit number of lines printed in hierarchy mode.")
        .scan<'i', int>()
        .default_value(-1);

    program.add_argument("file")
        .help("Input ontology .obo or .obo.gz")
        .nargs(1);

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n\n"
                  << program;
        return 1;
    }

    bool mode_xref = program.is_used("--xrefsearch");
    bool mode_obsolete = program.is_used("--obsolete-stats");
    bool mode_hierarchy = program.is_used("--hierarchy");

    int modes = (int)mode_xref + (int)mode_obsolete + (int)mode_hierarchy;
    if (modes == 0)
    {
        std::cerr << "Error: You must specify exactly one of --xrefsearch, --obsolete-stats, or --hierarchy.\n";
        return 1;
    }
    if (modes > 1)
    {
        std::cerr << "Error: Options are mutually exclusive.\n";
        return 1;
    }

    std::string file = program.get<std::vector<std::string>>("file").front();

    if (!fs::exists(file))
    {
        std::cerr << "Error: File not found: " << file << "\n";
        return 1;
    }
    if (!(file.ends_with(".obo") || file.ends_with(".obo.gz")))
    {
        std::cerr << "Error: File must end with .obo or .obo.gz\n";
        return 1;
    }

    std::vector<std::string> lines;
    try
    {
        lines = read_obo_lines(file);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Failed to read OBO: " << ex.what() << "\n";
        return 1;
    }

    GOStats stats(lines);

    if (mode_xref)
    {
        auto recs = stats.xrefsearch();
        std::cout << "GO_ID\tReactome_IDs\tEC_IDs\n";
        for (const auto &r : recs)
        {
            auto join = [](const std::vector<std::string> &v)
            {
                std::string s;
                for (size_t i = 0; i < v.size(); ++i)
                {
                    s += v[i];
                    if (i + 1 < v.size())
                        s += ",";
                }
                return s;
            };
            std::cout << r.go_id << "\t"
                      << join(r.reactome_ids) << "\t"
                      << join(r.ec_ids) << "\n";
        }
        return 0;
    }

    if (mode_obsolete)
    {
        auto obs = stats.obsoleteStats();
        std::cout << "Total: " << obs.total_obsolete << "\n"
                  << "With consider: " << obs.with_consider << "\n"
                  << "With replaced_by: " << obs.with_replaced_by << "\n"
                  << "Without alternatives: " << obs.no_alternative << "\n";
        return 0;
    }

    if (mode_hierarchy)
    {
        int limit = program.get<int>("--limit");
        auto rels = stats.hierarchical_relationships();
        if (limit >= 0 && (int)rels.size() > limit)
        {
            rels.resize((size_t)limit);
        }
        for (const auto &r : rels)
        {
            std::cout << r << "\n";
        }
        return 0;
    }

    // Should not reach here
    std::cerr << "Internal error: no mode executed.\n";
    return 1;
}
