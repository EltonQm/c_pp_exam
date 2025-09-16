// GooboParser1.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

void usage(const char *progname)
{
    cout << "Usage: " << progname << " FILENAME (--get-metacyc METACYC-ID | --tab-metacyc)" << endl;
}

void help(const char *progname)
{
    cout << "GooboParser - Author: YourName\n";
    usage(progname);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        usage(argv[0]);
        return 1;
    }

    string filename = argv[1];
    string task = argv[2];

    if (task == "--help" || task == "-h")
    {
        help(argv[0]);
        return 0;
    }

    if (!fs::exists(filename))
    {
        cout << "Error: File '" << filename << "' does not exist!" << endl;
        return 1;
    }

    if (task != "--get-metacyc" && task != "--tab-metacyc")
    {
        cout << "Error: Unknown task '" << task << "'!" << endl;
        usage(argv[0]);
        return 1;
    }

    if (task == "--get-metacyc" && argc < 4)
    {
        cout << "Error: --get-metacyc requires a MetaCyc ID (containing 'RXN' or 'PWY')." << endl;
        return 1;
    }

    cout << "Fine! (Arguments are OK)" << endl;

    return 0;
}
