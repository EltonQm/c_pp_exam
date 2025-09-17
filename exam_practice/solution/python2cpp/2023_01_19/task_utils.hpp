// task_utils.hpp
#ifndef TASK_UTILS_HPP
#define TASK_UTILS_HPP

#include <vector>
#include <string>
#include <unordered_set>
#include <argparse.hpp>

std::vector<std::string> read_fasta_lines(const std::string &filename);
void usage();
void print_command_usage(const std::vector<std::string> &args, const std::string &error_message);
std::vector<std::string> validate_files(const argparse::ArgumentParser &program, const std::vector<std::string> &files);
void print_invalid_filenames(const argparse::ArgumentParser &program, const std::vector<std::string> &args, const std::vector<std::string> &invalid_items);
std::vector<std::string> validate_patterns(const argparse::ArgumentParser &program, const std::vector<std::string> &items);
void print_invalid_pattern(const argparse::ArgumentParser &program, const std::vector<std::string> &args, const std::vector<std::string> &invalid_items);
bool has_fasta_ext(const std::string &path);
void process_get_length(const std::vector<std::string> &files);
void process_get_sequence(const std::vector<std::string> &files);
void process_grep_sequence(const std::vector<std::string> &files, const std::vector<std::string> &patterns);
void print_command_error(const argparse::ArgumentParser &program, const std::vector<std::string> &args, const std::string &error_message);

#endif // TASK_UTILS_HPP