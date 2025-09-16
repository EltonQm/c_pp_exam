// g++ -std=c++17 -I../argparse/include argparse.cpp -o square_program &&
// ./square_program 5

// The square of 5 is 25

#include "argparse.hpp" // Include argparse library
#include <cstdlib>      // For exit()
#include <iostream>     // For std::cout, std::endl
#include <string>       // For std::string

int main(int argc, char *argv[]) {
  // Create argument parser object with program name
  argparse::ArgumentParser program("square_program");

  // Add a positional argument called "number"
  program.add_argument("number")
      .help("Display the square of a given integer")
      .action([](const std::string &value) { return std::stoi(value); });

  // Try to parse the command-line arguments
  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    // Print error and usage help if parsing fails
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }

  // Retrieve the parsed integer value
  int input = program.get<int>("number");

  // Output the square
  std::cout << "The square of " << input << " is " << (input * input)
            << std::endl;

  return 0;
}
