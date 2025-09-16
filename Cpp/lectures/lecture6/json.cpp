#include "include/json.hpp"
#include <iostream>
using json = nlohmann::json;

int main() {
  // create JSON object
  json object = {{"the good", "il buono"},
                 {"the bad", "il cattivo"},
                 {"the ugly", "il brutto"}};
  // ouput element with the key "the ugly"
  std::cout << object.at("the ugly") << '\n';

  // change element with key "the bad"
  object.at("the bad") = "il cattivo";

  // output changed array
  std::cout << object << '\n';
  for (auto &[key, value] : object.items()) {
    std::cout << "key: " << key << " - value: " << value << std::endl;
  }
  return (0);
}