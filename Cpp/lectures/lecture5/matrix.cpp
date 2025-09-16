/*
#include <iostream>
#include <vector>

int main() {
  typedef std::vector<std::vector<int>> IntMatrix;
  typedef std::vector<int> row;

  IntMatrix mt(5, row(4, 2)); // initialize 5x4 matrix with all elements = 2
  mt.push_back(row(4, 3));    // 6th row with all elements = 3
  mt[0][0] = 1;               // only first element of first row is changed to 1

  for (auto r : mt) {
    for (auto x : r) {
      std::cout << x << " , ";
    }
    std::cout << std::endl;
  }
  std::cout << " nrow: " << mt.size() << " ncol: " << mt[0].size() << std::endl;
  return 0;
}
*/

/*
// The Matrix
#include <iostream>
#include <vector>

int main() {
  // vector within vector = Matrix
  std::vector<std::vector<int>> vec2D(5, std::vector<int>(4, 1));

  for (auto vec : vec2D) {
    for (auto x : vec) {
      std::cout << x << ",";
    }
    std::cout << std::endl;
  }
}
*/
