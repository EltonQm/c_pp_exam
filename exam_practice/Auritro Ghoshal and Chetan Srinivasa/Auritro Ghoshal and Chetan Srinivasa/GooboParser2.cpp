//This code was submitted together by Auritro and Chetan.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <tuple>

// Function to display the help information
void displayHelp() {
    std::cout << "Usage: myapp <filename> <taskname> [<id>]\n";
    std::cout << "Arguments:\n";
    std::cout << "  <filename>   : Path to the GO-BO obo input file\n";
    std::cout << "  <taskname>   : Task name like --get-metacyc or --tab-metacyc\n";
    std::cout << "  <id>         : Optional MetaCyc identifier for --get-metacyc\n";
}

std::vector<std::tuple<std::string, std::string, std::string, std::string>> getMetacyc(const std::string& filename, const std::string& metacycId) {
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
    std::regex rxCycId("^xref: (MetaCyc:" + metacycId + ")$");

    std::smatch match;
    std::string go_id, name, name_space, metacyc;

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
        else if (std::regex_search(line, match, rxCycId)) {
            metacyc = metacycId;
            result.emplace_back(go_id, name, name_space, metacyc);
        }
    }

    infile.close();
    return result;

    return result;
}

int main(int argc, char** argv) {
    // Check the number of arguments
    if (argc < 3 || argc > 4) {
        displayHelp();
        return 1; // Exit the application
    }

    // Parse the command line arguments
    std::string filename = argv[1];
    std::string taskname = argv[2];
    std::string id;

    if (argc == 4) {
        id = argv[3];
    }

    // Check if "--get-metacyc" is provided and validate the id
    if (taskname == "--get-metacyc") {
        if (id.empty() || (id.find("RXN") == std::string::npos && id.find("PWY") == std::string::npos)) {
            std::cout << "Invalid MetaCyc identifier. It should contain either RXN or PWY substring.\n";
            return 1; // Exit the application
        }

        std::vector<std::tuple<std::string, std::string, std::string, std::string>> result = getMetacyc(filename, id);

        // Display the retrieved data from the result vector
        for (const auto& tuple : result) {
            std::cout << std::get<0>(tuple) << "\t\t" << std::get<1>(tuple) << "\t\t" << std::get<2>(tuple) << "\t\t" << std::get<3>(tuple) << std::endl;
        }
    }

    return 0; // Exit the application successfully
}
