#pragma once // Ensures the header is included only once per compilation unit

#include <fstream>       // For file input
#include <sstream>       // For string stream parsing
#include <stdexcept>     // For throwing runtime errors
#include <string>        // For using std::string
#include <unordered_map> // For hash map (lookup table)
#include <vector>        // For dynamic array (vector)

// Class to represent a substitution matrix like BLOSUM62
class SubstitutionMatrix {
private:
  // 2D map to hold substitution scores: matrix[residue1][residue2] = score
  std::unordered_map<char, std::unordered_map<char, int>> matrix;

public:
  // Function to load the substitution matrix from a file
  void loadFromFile(const std::string &filename) {
    std::ifstream file(filename); // Open the file
    if (!file.is_open()) {
      // If file cannot be opened, throw an error
      throw std::runtime_error("Failed to open substitution matrix file.");
    }

    std::string line;
    std::vector<char> header; // Holds the header residues (first row)

    // Read lines to find the header (amino acid letters)
    while (std::getline(file, line)) {
      if (line[0] == '#')
        continue; // Skip comment lines starting with '#'
      std::istringstream iss(line);
      char first;
      iss >> first;
      if (std::isalpha(
              first)) { // If the first token is a letter, it's the header line
        iss.clear();
        iss.str(line); // Reset stream to the start of the line
        char residue;
        while (iss >> residue) {
          header.push_back(residue); // Store all residue letters
        }
        break; // Header found, break the loop
      }
    }

    // Read the matrix body (rows of scores)
    while (std::getline(file, line)) {
      if (line.empty() || line[0] == '#')
        continue; // Skip empty or comment lines
      std::istringstream iss(line);
      char rowResidue;
      iss >> rowResidue; // Read the row residue (first column)
      for (size_t i = 0; i < header.size(); ++i) {
        int score;
        iss >> score; // Read the score for header[i]
        // Store the score in the matrix at position [rowResidue][header[i]]
        matrix[rowResidue][header[i]] = score;
      }
    }

    file.close(); // Close the file
  }

  // Get the substitution score between two residues 'a' and 'b'
  int getScore(char a, char b) const {
    // Check if both residues exist in the matrix
    if (matrix.find(a) != matrix.end() &&
        matrix.at(a).find(b) != matrix.at(a).end()) {
      return matrix.at(a).at(b); // Return the score
    } else {
      // If residues are not found, throw an error
      throw std::invalid_argument("Residues not found in substitution matrix.");
    }
  }
};