#include <iostream>

using namespace std;

auto add(auto a, auto b)
{
    return (a + b);
}

int main()
{
    cout << add(1, 3) << " " << add(1.0, 2.5) << " " << add(1, 2.5) << endl;
    return 0;
}

// C++ 20 instead of C++17

/*
C++17 does not support auto in function parameters, which is why you get the error.

C++20 introduces template parameter deduction using auto in function signatures,
allowing you to write more generic functions that can accept different types for the
parameters.
*/