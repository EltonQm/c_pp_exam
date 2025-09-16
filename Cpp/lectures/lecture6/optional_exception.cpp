#include <iostream> // For std::cout
#include <optional> // For std::optional
#include <string>   // For std::string

// std::optional is used as a return type to represent a value that might be
// absent

// Function that returns an optional string
std::optional<std::string> create(bool b) {
  if (b)
    return "Godzilla"; // Return a string if 'b' is true
  return {};           // Return empty optional if 'b' is false
}

int main() {
  // Call create(false): returns empty optional, so value_or() provides fallback
  // "empty"
  std::cout << "create(false) returned " << create(false).value_or("empty")
            << '\n';

  // Call create(true): returns "Godzilla", so value_or() gives actual value
  std::cout << "create(true) returned " << create(true).value_or("empty")
            << '\n';

  // Use the optional in a boolean context to check if a value is present
  if (create(false)) {
    std::cout << "it returns something\n"; // Will not be executed
  } else {
    std::cout << "it returns nothing\n"; // Will be printed
  }
}

/* Assuming in a GUI we don't want to crash the program with an exception of
the function. It might be feasible sometimes in this case just return nothing
from the function, if the function was not successful.*/

/*
std::optional<T> allows a function to return either a value of type T or no
value (empty).

value_or("fallback") safely accesses the value or returns a default.

if (optional) checks whether the optional contains a value.
*/