#include <cstdlib>
#include <iostream>
#include <string>
#include "filesystem/include/ghc/filesystem.hpp"
//#include <filesystem>
#include <map>
#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <cstdlib>
#include <cstdio>
#else // Assume Linux
#include <cstdlib>
#include <cstdio>
#endif
#include "pex.hpp"

namespace fs = ghc::filesystem;
/*
 * Utility functions for the course Programming Expertise
 *
 */
namespace pex {
/*
 * std::string pex::get_home_directory()
 *
 * returns: The home directory of the current user
 */
std::string get_home_directory() {
  // get home directory Unix
  const char *homeDir = getenv("HOME");
  // Get home directory on Windows (untested)
  const char *userProfile = getenv("USERPROFILE");
  std::string result = "";
  if (homeDir != nullptr) {
    result = homeDir;
  } else if (userProfile != nullptr) {
    result = userProfile;
  }
  return (result);
}
/*
 * std::string pex::get_os()
 *
 * returns: current operating system, should work on
   Linux (linux), Windows (windows), MacOS (macos) and
   BSD (freebsd, openbsd, netbsd) or 'other'
 *
 */

std::string get_os() {
#ifdef _WIN32
  return ("windows");
#elif __APPLE__
  return ("macos");
#elif __linux__
  return ("linux");
#elif __FreeBSD__
  return ("freebsd");
#elif __OpenBSD__
  return ("openbsd");
#elif __NetBSD__
  return ("netbsd");
#else
  return ("other");
#endif
}

std::string get_session_type() {
#ifdef _WIN32
    return "windows"
#elif __APPLE__
    // Not needed on Mac, but keep for interface consistency
    return "macos";
#else
    const char* session_type = std::getenv("XDG_SESSION_TYPE");
    return session_type ? session_type : "";
#endif
}

void copy_to_clipboard(const std::string& text) {
    std::string session_type = get_session_type();
    const char* cmd = nullptr;
#ifdef _WIN32
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        size_t size = (text.size() + 1) * sizeof(wchar_t);
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
        if (hMem) {
            wchar_t* wstr = (wchar_t*)GlobalLock(hMem);
            MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, wstr, text.size() + 1);
            GlobalUnlock(hMem);
            SetClipboardData(CF_UNICODETEXT, hMem);
        }
        CloseClipboard();
    }
    return;
#endif        
    if (session_type == "apple") {
        cmd = "pbcopy";
    } else if (session_type == "wayland") {
        cmd = "wl-copy";
    } else if (session_type == "x11") {
        cmd = "xclip -selection clipboard";
    } else {
        // Unknown or unsupported session type
        return;
    }
    FILE* pipe = popen(cmd, "w");
    if (pipe) {
        fwrite(text.c_str(), 1, text.size(), pipe);
        pclose(pipe);
    }
}
/*
 * std::string pex::get_config_dir(std::string appname)
 *
 * returns: folder path or empty string if the folder does not exists
   and can't be created.
 * 
 * description: the function returns the usual place where applications
   place their configuration files. The folder is created if it
   does not yet exists. For instance on Linux a folder
 * ~/.config/appname will be created if it does not yet exists.
 *
 */

std::string get_config_dir(std::string appname) {
  // we use std::filesystem to check for the necessary directories
  std::string configdir; // create foldername based on the OS
  std::string HOME = pex::get_home_directory();
  std::string OS = pex::get_os();

  if (OS == "linux") {
    configdir = HOME + "/.config/" + appname;
  } else if (OS == "macos") {
    configdir = HOME + "/Library/Application Support/" + appname;
  } else if (OS == "windows") {
    configdir = HOME + "\\AppData\\Roaming\\" + appname;
  } else {
    // probably any other Unix system like Solaris or
    // BSD or any other like  Windows MSYS? or Windows Cygwin
    configdir = HOME + "/.config/"+appname;
  }
  // Check if directory exists
  if (!fs::exists(configdir)) {
    try {
      fs::create_directories(configdir);
    } catch (const fs::filesystem_error &ex) {
      std::cerr << "Failed to create directory: " << ex.what() << std::endl;
      return "";
    }
  } 
  return configdir;
    
}
/*
 * std::map<std::string,std::string> pex::read_directory(std::filesystem::path &dir_path)
 *
 * returns: a map with filenames as keys and content of these files
   as values.
 * 
 */

std::map<std::string,std::string> read_directory(const fs::path &dir_path) {
  std::map<std::string, std::string> contents;
    for (const auto &entry : fs::recursive_directory_iterator(dir_path)) {
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
bool has_extension(const std::string& filename,std::string pattern) {
    return fs::path(filename).extension() == pattern;
}

}; // namespace pex
#ifdef PEX
int main() {
  std::cout << "PEX Home: " << pex::get_home_directory() << std::endl;
    std::cout << "Operating System: " << pex::get_os() << std::endl;
    std::cout << "Session Type:" << pex::get_session_type() << std::endl;
    std::cout << "Creating or accessing configdir: " << pex::get_config_dir("test") << std::endl;
    std::map<std::string,std::string> contents = pex::read_directory("snippets");
    pex::copy_to_clipboard("Hello World!");
    for (const auto & pair : contents) {
        if (pex::has_extension(pair.first,".txt")) {
            std::cout << contents.count(pair.first) << std::endl;
            std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
        }
    }
    
  return (0);
}
#endif
