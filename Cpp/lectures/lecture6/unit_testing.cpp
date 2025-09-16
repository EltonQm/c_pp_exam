#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <vector>

int factorial(int number) {
  return number <= 1 ? number : factorial(number - 1) * number;
}

TEST_CASE("testing the factorial function") {
  CHECK(factorial(1) == 1);
  CHECK(factorial(2) == 2);
  CHECK(factorial(3) == 6);
  CHECK(factorial(10) == 3628800);
  CHECK(factorial(3) == 7); // should fail
}
/*
int main() {
  IMatrix mt(true);
  IMatrix mt2(3, 3, false);
  mt2.set(0, 0, 5);
  mt2.set(1, 0, 4); // expected result ??
  mt2.asText();
  std::vector<int> newrow = {7, 8, 9};
  mt2.addRow(newrow);
  std::vector<int> newcol = {7, 8, 9, 10};
  //...
  // another 300 lines
  //...
  std::cout << "end" << std::endl;
}
*/
/*
- In comparison to other PL, C++ has no default Unit-Test library.
- Unit testing ensures code integrity during development.
*/

/*
Test-driven development (TDD) is a software development process relying on
software requirements being converted to test cases before software is fully
developed, and tracking all software development by repeatedly testing the
software against all test cases. This is opposed to software being developed
first and test cases created later.
*/