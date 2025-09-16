#include <iostream>
#include <vector>
using namespace std;

int main()
{
    // auto x ; // not possible as type is unclear
    auto some_variable = 5;                  // type guess ok, type is integer
    std::vector<int> numbers = {1, 2, 3, 5}; // explicit
    auto numbers2 = {3, 1, 3, 5, 8, 9};      // shorter, guess a vector

    for (auto it : numbers)

        printf("%d ", it);

    printf("\n");

    for (auto it : numbers2)

        printf("%d ", it);

    printf("\n");
}