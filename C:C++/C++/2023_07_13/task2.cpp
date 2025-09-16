// task2.cpp (extends task1)
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>

// Function to extract [Term] entries with MetaCyc xrefs and print GO_ID, NAME, NAMESPACE, METACYC_ID
void parse_terms_with_metacyc(const std::string &filename, const std::unordered_set<std::string> &metacyc_ids)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string go_id, name, ns, metacyc_id;
    bool in_term = false;

    while (std::getline(file, line))
    {
        if (line == "[Term]")
        {
            in_term = true;
            go_id.clear();
            name.clear();
            ns.clear();
            metacyc_id.clear();
        }
        else if (line.empty())
        {
            if (in_term && !go_id.empty() && !metacyc_id.empty())
            {
                std::cout << go_id << "\t" << name << "\t" << ns << "\t" << metacyc_id << "\n";
            }
            in_term = false;
        }
        else if (in_term)
        {
            if (line.rfind("id: GO:", 0) == 0)
            {
                go_id = line.substr(4);
            }
            else if (line.rfind("name:", 0) == 0)
            {
                name = line.substr(6);
            }
            else if (line.rfind("namespace:", 0) == 0)
            {
                ns = line.substr(10);
            }
            else if (line.rfind("xref:", 0) == 0)
            {
                std::size_t pos = line.find("MetaCyc:");
                if (pos != std::string::npos)
                {
                    std::string raw_id = line.substr(pos + 8);
                    raw_id.erase(std::remove(raw_id.begin(), raw_id.end(), '\n'), raw_id.end());
                    if (metacyc_ids.empty() || metacyc_ids.count(raw_id))
                    {
                        metacyc_id = raw_id;
                    }
                }
            }
        }
    }

    if (in_term && !go_id.empty() && !metacyc_id.empty())
    {
        std::cout << go_id << "\t" << name << "\t" << ns << "\t" << metacyc_id << "\n";
    }

    file.close();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./task2 <file.obo> [MetaCyc_ID1 MetaCyc_ID2 ...]" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::unordered_set<std::string> metacyc_ids;
    for (int i = 2; i < argc; ++i)
    {
        metacyc_ids.insert(argv[i]);
    }

    parse_terms_with_metacyc(filename, metacyc_ids);
    return 0;
}
