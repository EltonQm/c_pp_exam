// -!- C++ -!- ///////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 Detlef Groth.
//
// All rights reserved.
//
// Synopsis:
// Authors:     Detlef Groth
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <regex>
#include <map>
#include <vector>
#include "filesystem/include/ghc/filesystem.hpp"
namespace fs = ghc::filesystem;

static const char HELP[] =
    R"(Example application, Max Musterman, University of Potsdam
  Usage:
    __APPNAME__ (-h | --help)  OBOFILE
    __APPNAME__ --tab-metacyc  METACY-ID OBOFILE
    __APPNAME__ --get-metacyc  OBOFILE

)";
static const char USAGE[] =
    R"(Usage: __APPNAME__ [-h,--help | --get-metacyc METACY-ID | --tab-metacyc] OBOFILE
)";

void usage(std::string appname, bool help = false)
{
    if (help)
    {
        std::cout << std::regex_replace(HELP, std::regex("__APPNAME__"), appname) << std::endl;
    }
    else
    {
        std::cout << std::regex_replace(USAGE, std::regex("__APPNAME__"), appname) << std::endl;
    }
}
int main(int argc, char **argv)
{
    if (argc == 1)
    {
        usage(argv[0]);
    }
    else if (strcmp(argv[1], "--help") == 0 ||
             strcmp(argv[1], "-h") == 0)
    {
        usage(argv[0], true);
    }
    else if (strcmp(argv[1], "--get-metacyc") == 0)
    {
        if (argc != 4)
        {
            std::cout << "Error: Not the right number of arguments for --get-metacyc!\n"
                      << std::endl;
            usage(argv[0]);
        }
        else if (std::regex_match(argv[2], std::regex("^.+(RXN|PWY)$")) == 0)
        {
            std::cout << "Error: Not a MetaCycID given for --get-metacyc! (should end with PWY or RXN\n"
                      << std::endl;
            usage(argv[0]);
        }
        else if (!fs::exists(argv[3]))
        {
            std::cout << "Error: File '" << argv[3] << "' does not exists!\n"
                      << std::endl;
            usage(argv[0]);
        }
        else
        {
            std::cout << "Looks fine for --get-metacyc!" << std::endl;
        }
    }
    else if (strcmp(argv[1], "--tab-metacyc") == 0)
    {
        if (argc != 3)
        {
            std::cout << "Error: Not the right number of arguments for --tab-metacyc!\n"
                      << std::endl;
            usage(argv[0]);
        }
        else if (!fs::exists(argv[2]))
        {
            std::cout << "Error: File '" << argv[2] << "' does not exists!\n"
                      << std::endl;
            usage(argv[0]);
        }
        else
        {
            std::cout << "Looks fine for --tab-meta-cyc!" << std::endl;
        }
    }
    else
    {
        std::cout << "Error: Unkown option '" << argv[1] << "'!\n"
                  << std::endl;
        usage(argv[0]);
    }
    return (0);
}