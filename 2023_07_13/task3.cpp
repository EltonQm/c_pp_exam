// task3.cpp (extends task2 with gzip and vector input)
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <zlib.h>
#include <cstring>
#include "task_utils.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./task3 <file.obo[.gz]> [MetaCyc_ID1 MetaCyc_ID2 ...]" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::unordered_set<std::string> metacyc_ids;
    for (int i = 2; i < argc; ++i)
    {
        metacyc_ids.insert(argv[i]);
    }

    const auto lines = read_obo_lines(filename);
    parse_terms_with_metacyc_lines(lines, metacyc_ids);
    return 0;
}
