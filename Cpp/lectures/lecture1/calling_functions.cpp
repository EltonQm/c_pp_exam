#include <iostream>
using namespace std;

int sum(int a, int b = 20)
{
    int result;
    result = a + b;
    return (result);
}

int main()
{

    // local variable declaration:

    int a = 100;

    int b = 200;

    int result;

    // calling a function to add the values.

    result = sum(a, b);

    cout << "Total value is : " << result;

    // calling a function again as follows.

    result = sum(a);

    cout << " - Total value is : " << result << endl;

    return 0;
}

/*

• call by value (default, makes copy, can’t change argument)
• call by pointer (address of argument, can change argument)
• call by reference, can change argument - C++ only
• function arguments can have default values - C++ only

*/