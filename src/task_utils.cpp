// task_utils.cpp — shared utils impl
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "task_utils.hpp"

namespace fs = std::filesystem;

static std::string to_lower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return s;
}

bool has_obo_ext_ci(const std::string &path)
{
    auto lower = to_lower(path);
    if (lower.size() < 4)
        return false;
    return lower.rfind(".obo") == lower.size() - 4;
}

bool is_valid_namespace(const std::string &ns)
{
    static const std::unordered_set<std::string> allowed = {
        "molecular_function", "cellular_component", "biological_process"};
    return allowed.count(ns) > 0;
}

void normalize_and_validate_namespaces(const std::vector<std::string> &raw,
                                       std::unordered_set<std::string> &out)
{
    for (auto &s : raw)
    {
        auto lower = to_lower(s);
        if (is_valid_namespace(lower))
            out.insert(lower);
    }
}

void validate_input_files(const std::vector<std::string> &files,
                          std::vector<std::string> &valid,
                          std::vector<std::string> &invalid_ext,
                          std::vector<std::string> &missing)
{
    for (const auto &f : files)
    {
        if (!has_obo_ext_ci(f))
        {
            invalid_ext.push_back(f);
            continue;
        }
        if (!fs::exists(f) || !fs::is_regular_file(f))
        {
            missing.push_back(f);
            continue;
        }
        valid.push_back(f);
    }
}

void print_usage(const std::string &prog)
{
    std::cout
        << "Usage (quick):\n"
        << "  " << prog << " --consider-table <OBO...> [--namespace NS[,NS...]] [--pattern REGEX]\n"
        << "  " << prog << " --obsolete-stats <OBO...> [--namespace NS[,NS...]] [--pattern REGEX]\n"
        << "  " << prog << " --help\n\n"
        << "Namespaces: molecular_function, cellular_component, biological_process\n"
        << "Notes:\n"
        << "  • Files must have .obo (case-insensitive) and exist\n"
        << "  • --pattern filters GO term names by regex\n"
        << "Examples:\n"
        << "  " << prog << " --consider-table go-2020-01.obo go-2021-01.obo --namespace molecular_function --pattern \".*ribosome.*\"\n"
        << "  " << prog << " --obsolete-stats go-2020-01.obo --namespace cellular_component,biological_process\n";
}

CLIOptions parse_task1_cli(int argc, char **argv)
{
    argparse::ArgumentParser program("GOParser");
    program.add_description("GO OBO parser with subcommands and validation");

    // Mutually exclusive modes
    auto mode = program.add_mutually_exclusive_group(true);
    mode.add_argument("--consider-table")
        .help("Generate consider-table for obsolete GO terms")
        .nargs(argparse::nargs_pattern::at_least_one);
    mode.add_argument("--obsolete-stats")
        .help("Print stats on obsolete GO terms")
        .nargs(argparse::nargs_pattern::at_least_one);

    program.add_argument("--namespace")
        .help("Comma-separated namespaces (mf, cc, bp full names)")
        .default_value(std::string{});

    program.add_argument("--pattern")
        .help("Regex for GO term name filter")
        .default_value(std::string{});

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n\n";
        print_usage(argv[0]);
        std::cout << program; // full help
        std::exit(1);
    }

    CLIOptions opts;
    std::vector<std::string> inputs;

    if (program.is_used("consider-table"))
    {
        opts.consider_table = true;
        inputs = program.get<std::vector<std::string>>("consider-table");
    }
    else if (program.is_used("obsolete-stats"))
    {
        opts.obsolete_stats = true;
        inputs = program.get<std::vector<std::string>>("obsolete-stats");
    }

    if (inputs.empty())
    {
        std::cerr << "Error: at least one .obo file is required.\n\n";
        print_usage(argv[0]);
        std::exit(1);
    }
    opts.obo_files = inputs;

    // namespaces
    const auto ns_csv = program.get<std::string>("--namespace");
    if (!ns_csv.empty())
    {
        std::vector<std::string> raw;
        std::string cur;
        for (char c : ns_csv)
        {
            if (c == ',')
            {
                if (!cur.empty())
                {
                    raw.push_back(cur);
                    cur.clear();
                }
            }
            else
                cur.push_back(c);
        }
        if (!cur.empty())
            raw.push_back(cur);
        normalize_and_validate_namespaces(raw, opts.namespaces);
        if (opts.namespaces.empty())
        {
            std::cerr << "Warning: provided namespaces invalid; ignoring filter.\n";
        }
    }

    // regex pattern
    const auto pat = program.get<std::string>("--pattern");
    if (!pat.empty())
    {
        try
        {
            opts.name_pattern.emplace(pat, std::regex::ECMAScript);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: invalid regex: " << e.what() << "\n\n";
            print_usage(argv[0]);
            std::exit(1);
        }
    }

    // file validation
    std::vector<std::string> valid, invalid_ext, missing;
    validate_input_files(opts.obo_files, valid, invalid_ext, missing);

    if (!invalid_ext.empty())
    {
        for (auto &f : invalid_ext)
        {
            std::cerr << "Error: invalid extension (expected .obo): " << f << "\n";
        }
    }
    if (!missing.empty())
    {
        for (auto &f : missing)
        {
            std::cerr << "Error: file not found: " << f << "\n";
        }
    }
    if (valid.empty())
    {
        std::cerr << "\nNo valid input files.\n\n";
        print_usage(argv[0]);
        std::exit(1);
    }
    opts.obo_files = std::move(valid);
    return opts;
}
