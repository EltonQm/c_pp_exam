#pragma once
#include <cstdio>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <array>
#include <iostream>

inline std::string run_capture(const std::string &cmd, int &exit_code)
{
    std::array<char, 4096> buf{};
    std::string out;

#if defined(_WIN32)
    FILE *pipe = _popen(cmd.c_str(), "r");
#else
    FILE *pipe = popen(cmd.c_str(), "r");
#endif
    if (!pipe)
    {
        throw std::runtime_error("Failed to run command: " + cmd);
    }
    while (fgets(buf.data(), buf.size(), pipe))
    {
        out.append(buf.data());
    }

#if defined(_WIN32)
    exit_code = _pclose(pipe);
#else
    exit_code = pclose(pipe);
#endif
    if (exit_code == -1)
    {
        throw std::runtime_error("pclose failed for: " + cmd);
    }
    // On POSIX pclose returns status << 8 sometimes; keep raw.
    return out;
}

inline void assert_contains(const std::string &haystack,
                            const std::string &needle,
                            const std::string &context)
{
    if (haystack.find(needle) == std::string::npos)
    {
        std::cerr << "ASSERT FAILED (" << context << "): expected substring:\n"
                  << "  \"" << needle << "\"\nBut it was not found.\n";
        std::exit(1);
    }
}

inline void assert_true(bool cond, const std::string &context)
{
    if (!cond)
    {
        std::cerr << "ASSERT FAILED: " << context << "\n";
        std::exit(1);
    }
}

inline void banner(const std::string &name)
{
    std::cout << "[TEST] " << name << "\n";
}
