#ifndef TASK2_UTILS_HPP
#define TASK2_UTILS_HPP

#include <vector>
#include <string>
#include <unordered_set>
#include "argparse/argparse.hpp"
#include <iostream>

// Reads .obo or .obo.gz lines
std::vector<std::string> read_obo_lines(const std::string &filename);

void usage(const argparse::ArgumentParser &program);

std::vector<std::string> validate_namespaces(const argparse::ArgumentParser &program,
                                             const std::vector<std::string> &namespaces);

std::vector<std::string> validate_files(const argparse::ArgumentParser &program,
                                        const std::vector<std::string> &files);

void print_invalid_arguments(const argparse::ArgumentParser &program,
                             const std::vector<std::string> &args,
                             const std::vector<std::string> &invalid_items);

std::vector<std::vector<std::string>> process_consider_table(const std::vector<std::string> &files,
                                                             const std::vector<std::string> &namespaces);

std::vector<std::vector<std::string>> process_obsolete_stats(const std::vector<std::string> &files,
                            const std::vector<std::string> &namespaces);

void print_command_error(const argparse::ArgumentParser &program,
                         const std::vector<std::string> & /*args*/,
                         const std::string &error_message);

bool has_obo_ext(const std::string &path);

void parse_terms_with_metacyc_lines(const std::vector<std::string> &lines,
                                    const std::unordered_set<std::string> &metacyc_ids);

void count_obsolete_terms(const std::vector<std::string> &lines);

void print_go_id_and_name(const std::vector<std::string> &lines);

#endif // TASK2_UTILS_HPP
