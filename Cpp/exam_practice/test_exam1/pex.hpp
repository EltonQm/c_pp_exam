// -!- C++ -!- ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#ifndef __PEX_HPP
#define __PEX_HPP
#include <string>
#include <map>
#include <fstream>
//#include <filesystem>
#include "filesystem/include/ghc/filesystem.hpp"
namespace fs = ghc::filesystem;
namespace pex {
void copy_to_clipboard(const std::string& text);
std::string get_home_directory();
std::string get_os();
std::string get_config_dir(std::string appname);
std::string get_session_type();
std::map<std::string,std::string> read_directory(const fs::path &dir_path);
bool has_extension(const std::string& filename,const std::string pattern=".txt");
} // namespace pex
#endif // __PEX_HPP
