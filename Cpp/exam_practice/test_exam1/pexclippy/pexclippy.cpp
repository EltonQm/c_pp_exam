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
// compile with: g++ -std=c++17 clippy.cpp -o clippy -lncurses && strip clippy
// ghc file system
//  git clone https://github.com/gulrak/filesystem.git
//  syntax check: g++ -Wall -x c++ -fsyntax-only clippy.cpp
#include "pexclippy.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <ncurses.h>
#include <string>
#include <vector>
#include "pex.hpp"
namespace fs = ghc::filesystem;
// stativ variable, just for illustration
// it makes only sense to have one instance as you
// can work only on one terminal easily
int PexClippy::instances = 1;
// Constructor
// just for illustrative purposes
PexClippy::PexClippy() {
    // would stay invisible due to ncurses!
    std::cerr << "Object of PexClippy created!\n";
    // so we sleep for reading!
    sleep(3);
    instances += 1;
}
// Destructor:
// just for illustrative purposes
PexClippy::~PexClippy() {
    instances += 1;
    std::cout << "Object of PexClippy destroyed!\n";
}
bool PexClippy::contains(const std::string &haystack,
                         const std::string &needle) {
    auto it = std::search(haystack.begin(), haystack.end(), needle.begin(),
    needle.end(), [](char ch1, char ch2) {
        return std::tolower(ch1) == std::tolower(ch2);
    });
    return (it != haystack.end());
}
void PexClippy::menu(const fs::path &dir_path) {
    items = readDirectory(dir_path);
    // use readDirectory to fill the map
    std::vector<std::string> files;
    for (const auto &myPair : items) {
        files.push_back(myPair.first);
    }
    std::sort(files.begin(),files.end());
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
    int i = 0;
    while (true) {
        filtered.clear();
        i = 0;
        //  g++ -Wall -x c++ -fsyntax-only clippy.cpp
        // item without & gives warning
        for (const auto &item : files) {
            if (input.empty() || contains(item, input)) {
                if (pex::has_extension(item)) {
                    filtered.push_back(item);
                    i = i +1;
                    if (i > 10) {
                        break ;
                    }
                }
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
        for (size_t i = 0; i < filtered.size(); ++i) {
            if (i == 0) {
                attron(COLOR_PAIR(2));
                mvprintw(row, 3, "%s", filtered[i].c_str());
                attroff(COLOR_PAIR(1));
            } else {
                mvprintw(row, 3, "%s", filtered[i].c_str());
            }
            row++;
        }
        if (!filtered.empty()) {
            std::string content = items[filtered[0]];
            std::istringstream stream(content);
            std::string line;
            row++;
            attron(COLOR_PAIR(3));
            mvprintw(row, 0, "File Preview: %d %s", (int)items.count(filtered[0]), filtered[0].c_str());
            row++;
            attroff(COLOR_PAIR(3));
            for (int j = 0; std::getline(stream, line); j++ ) {
                mvprintw(row, 0, line.c_str());
                row++;
                if (j > 2) {
                    mvprintw(row, 0, "...");
                    row++;
                    break;
                }
            }
        }

        move(1, 8 + input.length()); // Move cursor to input position
        refresh();

        ch = getch();
        if (ch == 27) {
            break;  // ESC
        } else if (ch == KEY_ENTER || ch == 10) { // Handle both Enter keys
            if (!filtered.empty()) {
                // Show selected item (top of list)
                pex::copy_to_clipboard(items[filtered[0]]);
                mvprintw(row + 2, 0, "Selected and copied to clipboard content of: %s", filtered[0].c_str());
                refresh();
                getch(); // Wait for any key
            }
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (!input.empty())
                input.pop_back();
        } else if (isprint(ch)) {
            input += static_cast<char>(ch);
        }
    }
    endwin();
}
void PexClippy::itemAdd(std::string key, std::string value) {
    std::cout << "\nFunction itemAdd is not ready yet!\n";
}
std::string PexClippy::itemGet(std::string key) {

    if (items.count("key") > 0)   {
        return(items["key"]);
    } else {
        return("No file selected!");
    }
}
std::map<std::string, std::string>
PexClippy::readDirectory(const fs::path &dir_path) {
    std::map<std::string, std::string> contents;
    // delegating functionality to pex namespace
    // as this might be general useful not only for pexclippy
    contents = pex::read_directory(dir_path);
    return contents;
}

int main(int argc, char **argv) {
    fs::path dir_path;
    if (argc < 2) {
        dir_path = pex::get_config_dir("pexclippy") ;
        //std::cerr << "Usage: " << argv[0] << " <directory>" << std::endl;
        // return 1;
    } else {
        fs::path dir_path = argv[1];
    }

    if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
        std::cerr << "Error: '" << dir_path << "' is not a valid directory."
                  << std::endl;
        return 1;
    }
    auto clippy = PexClippy();
    clippy.menu(dir_path);
    return 0;
}

