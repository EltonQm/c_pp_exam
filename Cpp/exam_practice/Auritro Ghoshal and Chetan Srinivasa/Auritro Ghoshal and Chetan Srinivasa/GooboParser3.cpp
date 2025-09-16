//This code was submitted together by Auritro and Chetan.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <tuple>

class OboParser {
public:
    OboParser(const std::string& filename) : filename(filename) {}

    std::vector<std::tuple<std::string, std::string, std::string, std::string>> getMetacyc(const std::vector<std::string>& ids) {
        std::vector<std::tuple<std::string, std::string, std::string, std::string>> result;

        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return result;
        }

        std::string line;
        std::regex rxGoId("^id: (GO:[0-9]{7})$");
        std::regex rxName("^name: ([A-Za-z ]+)$");
        std::regex rxNameSpace("^namespace: ([A-Za-z_]+)$");

        std::smatch match;
        std::string go_id, name, name_space;

        while (std::getline(infile, line)) {
            if (std::regex_search(line, match, rxGoId)) {
                go_id = match[1];
            }
            else if (std::regex_search(line, match, rxName)) {
                name = match[1];
            }
            else if (std::regex_search(line, match, rxNameSpace)) {
                name_space = match[1];
            }
            else if (isMetacycIdMatch(line, ids)) {
                std::string metacyc = extractMetacycId(line);
                result.emplace_back(go_id, name, name_space, metacyc);
            }
        }

        infile.close();
        return result;
    }
    std::vector<std::tuple<std::string, std::string, std::string>> generateTabMapping() {
        std::vector<std::tuple<std::string, std::string, std::string>> result;

        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return result;
        }

        std::string line;
        std::regex rxGoId("^id: (GO:[0-9]{7})$");
        std::regex rxEC("^xref: (EC:(.+))$");
        std::regex rxCycId("^xref: (MetaCyc:(.+))$");

        std::string go_id, ec_id, rxn_id;

        while (std::getline(infile, line)) {
            std::smatch match;
            if (std::regex_search(line, match, rxGoId)) {
                go_id = match[1];
            }
            else if (std::regex_search(line, match, rxEC)) {
                ec_id = match[2];
            }
            else if (std::regex_search(line, match, rxCycId)) {
                rxn_id = match[2];
            }

            if (!go_id.empty() && !ec_id.empty() && !rxn_id.empty()) {
                result.emplace_back(go_id, ec_id, rxn_id);
                go_id.clear();
                ec_id.clear();
                rxn_id.clear();
            }
        }

        infile.close();
        return result;
    }

private:
    std::string filename;

    bool isMetacycIdMatch(const std::string& line, const std::vector<std::string>& ids) {
        for (const std::string& id : ids) {
            std::regex rxCycId("^xref: (MetaCyc:" + id + ")$");
            if (std::regex_search(line, rxCycId)) {
                return true;
            }
        }
        return false;
    }

    std::string extractMetacycId(const std::string& line) {
        std::regex rxCycId("^xref: (MetaCyc:(.+))$");
        std::smatch match;
        if (std::regex_search(line, match, rxCycId) && match.size() > 2) {
            return match[2];
        }
        return "";
    }
};

// Usage:
int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <filename> <taskname> [<id> ...]\n";
        return 1;
    }

    std::string filename = argv[1];
    std::string taskname = argv[2];

    std::vector<std::string> ids;
    for (int i = 3; i < argc; ++i) {
        ids.emplace_back(argv[i]);
    }

    OboParser parser(filename);

    if (taskname == "--get-metacyc") {
        std::vector<std::tuple<std::string, std::string, std::string, std::string>> result = parser.getMetacyc(ids);
        for (const auto& tuple : result) {
            std::cout << "GO ID: " << std::get<0>(tuple) << "\t" << std::get<1>(tuple) << "\t\t" << std::get<2>(tuple) << "\t\t" << std::get<3>(tuple) << std::endl << std::endl;
        }
    }
    else if (taskname == "--tab-metacyc") {
		 std::vector<std::tuple<std::string, std::string, std::string>> result = parser.generateTabMapping();
		 for (const auto& tuple : result) {
			 std::cout << std::get<0>(tuple) << "\t" << std::get<1>(tuple) << "\t\t" << std::get<2>(tuple) << std::endl;
		}
	}		
    else {
        std::cout << "Invalid task name. Please provide a valid task." << std::endl;
        return 1;
    }

    return 0;
}
