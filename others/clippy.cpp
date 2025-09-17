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

// file pexclippy.cpp
#include <algorithm>
#include <iostream>
#include <map>
#include <ncurses.h>
#include <string>
#include <vector>
// check for older compilers
#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#elif __cpp_lib_experimental_filesystem
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include "filesystem/include/ghc/filesystem.hpp"
namespace fs = ghc::filesystem;
#endif
class PexClippy
{
private:
    std::map<std::string, std::string> items; // files
    bool contains(const std::string &haystack, const std::string &needle)
    {
        auto it = std::search(haystack.begin(), haystack.end(), needle.begin(),
                              needle.end(), [](char ch1, char ch2)
                              { return std::tolower(ch1) == std::tolower(ch2); });
        return (it != haystack.end());
    }

public:
    void menu(const fs::path &dir_path)
    {
        items = readDirectory(dir_path);
        // use readDirectory to fill the map
        std::vector<std::string> files;
        for (const auto &myPair : items)
        {
            files.push_back(myPair.first);
        }

        initscr();
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK); // Grey highlight
        init_pair(2, COLOR_GREEN, COLOR_BLACK); // Grey highlight
        init_pair(3, COLOR_BLUE, COLOR_BLACK);  // Grey highlight
        noecho();
        cbreak();
        keypad(stdscr, TRUE);

        std::string input;
        int ch;
        std::vector<std::string> filtered;

        while (true)
        {
            filtered.clear();
            for (const auto item : files)
            {
                if (input.empty() || contains(item, input))
                {
                    filtered.push_back(item);
                }
            }

            clear();
            attron(COLOR_PAIR(3));
            mvprintw(0, 0, "PEX - Clippy by Detlef Groth, 2025");
            attroff(COLOR_PAIR(3));
            mvprintw(1, 0, "Search: %s", input.c_str());
            mvprintw(2, 0, "Press ENTER to select, ESC to exit!");

            // Highlight first item
            int row = 4;
            for (size_t i = 0; i < filtered.size(); ++i)
            {
                if (i == 0)
                {
                    attron(COLOR_PAIR(2));
                    mvprintw(row, 3, "%s", filtered[i].c_str());
                    attroff(COLOR_PAIR(1));
                }
                else
                {
                    mvprintw(row, 3, "%s", filtered[i].c_str());
                }
                row++;
            }

            move(1, 8 + input.length()); // Move cursor to input position
            refresh();

            ch = getch();
            if (ch == 27)
                break; // ESC
            else if (ch == KEY_ENTER || ch == 10)
            { // Handle both Enter keys
                if (!filtered.empty())
                {
                    // Show selected item (top of list)
                    mvprintw(row + 3, 0, "Selected: %s", filtered[0].c_str());
                    refresh();
                    getch(); // Wait for any key
                }
            }
            else if (ch == KEY_BACKSPACE || ch == 127)
            {
                if (!input.empty())
                    input.pop_back();
            }
            else if (isprint(ch))
            {
                input += static_cast<char>(ch);
            }
        }
        endwin();
    }
    void itemAdd(std::string key, std::string value)
    {
        std::cout << "\nFunction itemAdd is not ready yet!\n";
    }
    std::string itemGet(std::string key)
    {
        std::cout << "\nFunction itemGet is not ready yet!\n";
        // should return the file content
        return ("");
    }
    std::map<std::string, std::string> readDirectory(const fs::path &dir_path)
    {
        std::map<std::string, std::string> contents;

        for (const auto &entry : fs::recursive_directory_iterator(dir_path))
        {
            if (!entry.is_regular_file())
                continue;

            const auto filename = entry.path().filename().string();

            std::ifstream file_stream(entry.path());
            if (!file_stream)
                continue; // Skip unreadable files

            contents[filename] =
                std::string((std::istreambuf_iterator<char>(file_stream)),
                            std::istreambuf_iterator<char>());
        }

        return contents;
    }
};
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <directory>" << std::endl;
        return 1;
    }
    fs::path dir_path = argv[1];
    if (!fs::exists(dir_path) || !fs::is_directory(dir_path))
    {
        std::cerr << "Error: '" << dir_path << "' is not a valid directory."
                  << std::endl;
        return 1;
    }
    auto clippy = PexClippy();
    clippy.menu(dir_path);
    return 0;
}