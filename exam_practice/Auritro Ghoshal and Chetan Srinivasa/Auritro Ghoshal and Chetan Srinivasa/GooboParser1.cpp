//This code was submitted together by Auritro and Chetan.

#include <iostream>
#include <string>
#include <vector>

// Function to display the help information
void displayHelp() {
    std::cout << "Usage: myapp <filename> <taskname> [<id>]\n";
    std::cout << "Arguments:\n";
    std::cout << "  <filename>   : Path to the GO-BO obo input file\n";
    std::cout << "  <taskname>   : Task name like --get--metacyc or --tab-metacyc\n";
    std::cout << "  <id>         : Optional MetaCyc identifier for --get-metacyc\n";
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
    }

    return 0; // Exit the application successfully
}
