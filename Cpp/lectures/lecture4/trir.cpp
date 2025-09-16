#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
/*
namespace dutils
{

    std::vector<std::pair<int, std::string>> grep(std::string pattern, std::string vstring, const std::regex::flag_type &flag = std::regex::basic)
    {

        std::regex rx(pattern, flag);

        std::vector<std::pair<int, std::string>> results;

        for (auto it = std::sregex_iterator(vstring.begin(), vstring.end(), rx);
             it != std::sregex_iterator(); ++it)
        {

            results.emplace_back(it->position(), it->str());
        }
        return results;
    }

    std::vector<std::pair<int, std::string>> grep(std::string pattern, std::vector<std::string> vstring, const std::regex::flag_type &flag = std::regex::basic)
    {

        std::regex rx(pattern, flag);
        std::vector<int> index_matches;
        std::vector<std::pair<int, std::string>> results;

        for (int i = 0; i < vstring.size(); ++i)
        {

            if (std::regex_search(vstring[i], rx))
            {

                results.emplace_back(i, vstring[i]);
            }
        }
        return results;
    }
}

template <typename K, typename V>
void print_map(const std::map<K, V> &m)
{
    for (const auto &entry : m)
    {
        std::cout << entry.first << ":" << entry.second << std::endl;
    }
}

int main()
{

    std::vector<std::pair<int, std::string>> res;
    std::vector<std::pair<int, std::string>> ret;

    std::string input1 = "Hello";
    std::vector<std::string> input2 = {"Hello", "Hallo", "Hullo", "World"};

    res = dutils::grep("[Hh][ae]", input1);
    ret = dutils::grep("[Hh][ae]", input2);

    std::cout << "---Vector Results ---\n";
    for (const auto &v : res)
    {
        std::cout << "Match at index: " << v.first << "->" << v.second << std::endl;
    }

    for (const auto &s : ret)
    {
        std::cout << "Match at index: " << s.first << "->" << s.second << std::endl;
    }

    std::cout << "\n--- Using print_map ---\n";
    std::map<int, std::string> m_res;
    std::map<int, std::string> m_ret;

    for (const auto &v : res)
        m_res[v.first] = v.second;

    for (const auto &s : ret)
        m_ret[s.first] = s.second;
    print_map(m_ret);
    print_map(m_res);

    return 0;
}
*/

#include <string>
#include <vector>
#include <regex>
#include <algorithm>

namespace xyutils
{

    // Match a pattern in a single string
    bool grepl(const std::string &pattern, const std::string &text,
               std::regex::flag_type flag = std::regex::ECMAScript)
    {
        return std::regex_search(text, std::regex(pattern, flag));
    }

    // Match a pattern in a vector of string - returns vector of bools
    std::vector<bool> grepl(const std::string &pattern, const std::vector<std::string> &texts,
                            std::regex::flag_type flag = std::regex::ECMAScript)
    {
        std::regex rx(pattern, flag);
        std::vector<bool> results;
        for (const auto &s : texts)
        {
            results.push_back(std::regex_search(s, rx));
        }
        return results;
    }

    // Replace pattern in single string
    std::string gsub(const std::string &pattern, const std::string &replacement,
                     const std::string &text, std::regex::flag_type flag = std::regex::ECMAScript)
    {
        return std::regex_replace(text, std::regex(pattern, flag), replacement);
    }

    // Replace pattern in vector of strings
    std::vector<std::string> gsub(const std::string &pattern, const std::string &replacement,
                                  const std::vector<std::string> &texts, std::regex::flag_type flag = std::regex::ECMAScript)
    {
        std::vector<std::string> results;
        std::regex rx(pattern, flag);
        for (const auto &s : texts)
        {
            results.push_back(std::regex_replace(s, rx, replacement));
        }
        return results;
    }

    bool any(const std::vector<bool> &flags)
    {
        return std::any_of(flags.begin(), flags.end(), [](bool val)
                           { return val; });
    }

    bool all(const std::vector<bool> &flags)
    {
        return std::all_of(flags.begin(), flags.end(), [](bool val)
                           { return val; });
    }

    bool none(const std::vector<bool> &flags)
    {
        return std::none_of(flags.begin(), flags.end(), [](bool val)
                            { return val; });
    }
}

int main()
{
    std::vector<std::string> words = {"Hello", "world", "Hallo", "apple"};

    auto matches = xyutils::grepl("H.", words); // regex match
    auto replaced = xyutils::gsub("H.", "Xx", words);

    for (const auto &word : replaced)
    {
        std::cout << word << "\n";
    }

    bool has_match = xyutils::any(matches);
    std::cout << "Any matches? " << std::boolalpha << has_match << std::endl;

    return 0;
}
