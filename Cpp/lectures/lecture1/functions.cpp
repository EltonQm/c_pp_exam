#include <iostream>
using namespace std;

// function returning the max between two numbers
int max(int num1, int num2); // declaration

// definition
int max(int num1, int num2)
{
    // local variable declaration
    int result;

    if (num1 > num2)

        result = num1;

    else

        result = num2;

    return result;
}

int main()
{

    int res = 0;

    res = max(4, 5);

    cout << "maximum is: " << res << endl;
}