// https://www.perplexity.ai:
// Using the C++ filesystem library: How can I loop over a directory and
// store the content of that directory in a map where the keys are the
// filenames without the directory prefix and the values are the contents of
// these files?

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>

namespace fs = std::filesystem;

std::map<std::string, std::string> load_directory_contents(const fs::path &dir_path)
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

        contents[filename] = std::string(
            (std::istreambuf_iterator<char>(file_stream)),
            std::istreambuf_iterator<char>());
    }

    return contents;
}

// Can you add as well an example main function which gets the directory to
// read from as an argument? And then print out line by line the key and the
// first line or chars with ... of the content.

int main(int argc, char *argv[])
{
    // Check for correct usage
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <directory>" << std::endl;
        return 1;
    }

    fs::path dir_path = argv[1];
    if (!fs::exists(dir_path) || !fs::is_directory(dir_path))
    {
        std::cerr << "Error: '" << dir_path << "' is not a valid directory." << std::endl;
        return 1;
    }

    auto files = load_directory_contents(dir_path);

    for (const auto &[filename, content] : files)
    {
        std::cout << filename << ": ";
        if (content.empty())
        {
            std::cout << "(empty)" << std::endl;
            continue;
        }
        // Print up to the first line or first 40 chars, then "..."
        auto newline_pos = content.find('\n');
        size_t preview_len = (newline_pos != std::string::npos) ? newline_pos : 40;
        if (preview_len > 40)
            preview_len = 40;
        std::cout << content.substr(0, preview_len);
        if (content.size() > preview_len)
            std::cout << "...";
        std::cout << std::endl;
    }
    return 0;
}