// -!- C++ -!- ///////////////////////////////////////////////////////////////
//
// Copyright (C) 2025 MicroEmacs User.
//
// All rights reserved.
//
// Synopsis:
// Authors:     MicroEmacs User
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __PEXCLIPPY_HPP
#define __PEXCLIPPY_HPP
#include <map>
#include <string>
#include <unistd.h> // to use sleep, works on Windows? Mac?
// check for older compilers before C++17/20
#include "filesystem/include/ghc/filesystem.hpp"
//#include <filesystem>
class PexClippy {
private:
  std::map<std::string, std::string> items; // files
  bool contains(const std::string &haystack, const std::string &needle);

public:
  static int instances;
  PexClippy();
  ~PexClippy();
  void menu(const ghc::filesystem::path &dir_path);
  void itemAdd(std::string key, std::string value);
  std::string itemGet(std::string key);
  std::map<std::string, std::string> readDirectory(const ghc::filesystem::path &dir_path);
};
#endif // __PEXCLIPPY_HPP
