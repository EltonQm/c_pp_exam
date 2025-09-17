/*
 * Catch2 Test Framework Header
 *
 * This is a stub file. In a real project, you would download the actual
 * catch.hpp file from: https://github.com/catchorg/Catch2
 *
 * For the full single-header version, use:
 * https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp
 *
 * This stub provides the basic macros needed for the test compilation.
 */

#ifndef CATCH_HPP_INCLUDED
#define CATCH_HPP_INCLUDED

#include <exception>
#include <iostream>
#include <string>
#include <vector>

// Basic test macros - simplified version
#define TEST_CASE(name, tags)                                                  \
  void test_##__LINE__();                                                      \
  static TestRegistrar reg_##__LINE__(#name, test_##__LINE__);                 \
  void test_##__LINE__()

#define SECTION(name) if (true)

#define REQUIRE(expr)                                                          \
  do {                                                                         \
    if (!(expr)) {                                                             \
      std::cerr << "FAILED: " << #expr << " at line " << __LINE__              \
                << std::endl;                                                  \
      throw std::runtime_error("Test failed");                                 \
    } else {                                                                   \
      std::cout << "PASSED: " << #expr << std::endl;                           \
    }                                                                          \
  } while (0)

#define REQUIRE_FALSE(expr) REQUIRE(!(expr))

// Simple test registration system
class TestRegistrar {
public:
  TestRegistrar(const std::string &name, void (*test_func)()) {
    std::cout << "Running test: " << name << std::endl;
    try {
      test_func();
      std::cout << "✓ Test passed: " << name << std::endl;
    } catch (const std::exception &e) {
      std::cout << "✗ Test failed: " << name << " - " << e.what() << std::endl;
    }
  }
};

#ifdef CATCH_CONFIG_MAIN
int main() {
  std::cout << "All tests completed." << std::endl;
  return 0;
}
#endif

#endif // CATCH_HPP_INCLUDED