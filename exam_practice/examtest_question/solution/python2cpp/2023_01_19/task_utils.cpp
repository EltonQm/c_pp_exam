#include "task_utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

void usage()
{
    std::cout << R"(
Usage: GOfastaparser [--get-n | --get-seq ID | --grep-seq PATTERN] FILE

FASTA-Parser by Elton Ugbogu, University of Potsdam, 2025
Extract information from FASTA sequence files
-------------------------------------------
Mandatory arguments:
    FILE               one or more FASTA files (.fasta or .fasta.gz)
Optional arguments (choose one mode only):
    --help             display this help page
    --get-n            show sequence lengths for all sequence IDs
    --get-seq ID       show the sequence for a given sequence ID
    --grep-seq PATTERN search sequences for a given regex pattern
)";
}

void print_command_error(const argparse::ArgumentParser &program, const std::vector<std::string> &args, const std::string &error_message)
{
    std::cerr << "Error: " << error_message << "\n";
    std::cerr << "You entered: ";
    for (const auto &arg : args)
        std::cerr << arg << " ";
    std::cerr << "\n\n";
    usage();
}

void print_command_usage(const std::vector<std::string> &args, const std::string &error_message)
{
    std::cerr << "Error: " << error_message << "\n";
    std::cerr << "You entered: ";
    for (const auto &arg : args)
        std::cerr << arg << " ";
    std::cerr << "\n\n";
    usage();
}

bool has_fasta_ext(const std::string &path)
{
    static const std::regex pattern(R"(.*\.fasta(\.gz)?$)", std::regex_constants::icase);
    return std::regex_match(path, pattern);
}

std::vector<std::string> validate_files(const argparse::ArgumentParser &, const std::vector<std::string> &files)
{
    std::vector<std::string> valid_files;
    for (const auto &filepath : files)
    {
        if (has_fasta_ext(filepath) && fs::exists(filepath))
        {
            valid_files.push_back(filepath);
        }
    }
    return valid_files;
}

void print_invalid_filenames(const argparse::ArgumentParser &, const std::vector<std::string> &args, const std::vector<std::string> &invalid_items)
{
    static const std::regex dot_checker_regex(R"(\.)");
    for (const auto &item : invalid_items)
    {
        if (std::regex_search(item, dot_checker_regex) && !has_fasta_ext(item) && !fs::exists(item))
        {
            auto pos = item.rfind('.');
            std::string ext = item.substr(pos + 1);
            std::ostringstream oss;
            oss << "Invalid file: " << item << " — extension '" << ext << "' is not supported. Expected '.fasta' or '.fasta.gz'.";
            print_command_usage(args, oss.str());
        }
    }
}

std::vector<std::string> validate_patterns(const argparse::ArgumentParser &, const std::vector<std::string> &items)
{
    static const std::regex dot_checker_regex(R"(\.)");
    std::vector<std::string> patterns;
    for (const auto &item : items)
    {
        if (!std::regex_search(item, dot_checker_regex))
        {
            patterns.push_back(item);
        }
    }
    return patterns;
}

void print_invalid_pattern(const argparse::ArgumentParser &, const std::vector<std::string> &args, const std::vector<std::string> &invalid_items)
{
    static const std::regex dot_checker_regex(R"(\.)");
    for (const auto &item : invalid_items)
    {
        if (!std::regex_search(item, dot_checker_regex))
        {
            std::ostringstream oss;
            oss << "Invalid pattern or sequence ID: " << item;
            print_command_usage(args, oss.str());
        }
    }
}

std::vector<std::string> read_fasta_lines(const std::string &filename)
{
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    return lines;
}

void process_get_length(const std::vector<std::string> &files)
{
    for (const auto &file : files)
    {
        auto lines = read_fasta_lines(file);
        std::string id;
        size_t length = 0;
        for (const auto &line : lines)
        {
            if (line.starts_with(">"))
            {
                if (!id.empty())
                {
                    std::cout << id << "\t" << length << "\n";
                    length = 0;
                }
                id = line.substr(1);
            }
            else
            {
                length += line.size();
            }
        }
        if (!id.empty())
        {
            std::cout << id << "\t" << length << "\n";
        }
    }
}

void process_get_sequence(const std::vector<std::string> &files)
{
    for (const auto &file : files)
    {
        auto lines = read_fasta_lines(file);
        for (const auto &line : lines)
        {
            std::cout << line << "\n";
        }
    }
}

void process_grep_sequence(const std::vector<std::string> &files, const std::vector<std::string> &patterns)
{
    std::regex pattern(patterns[0]);
    for (const auto &file : files)
    {
        auto lines = read_fasta_lines(file);
        std::string id;
        std::string seq;
        for (const auto &line : lines)
        {
            if (line.starts_with(">"))
            {
                if (!id.empty() && std::regex_search(seq, pattern))
                {
                    std::cout << ">" << id << "\n"
                              << seq << "\n";
                }
                id = line.substr(1);
                seq.clear();
            }
            else
            {
                seq += line;
            }
        }
        if (!id.empty() && std::regex_search(seq, pattern))
        {
            std::cout << ">" << id << "\n"
                      << seq << "\n";
        }
    }
}
