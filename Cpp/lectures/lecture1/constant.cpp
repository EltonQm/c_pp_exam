#include <iostream>

using namespace std;

int main()
{
    // initialisations
    const int LENGTH = 10;
    const int WIDTH = 5;
    const char NEWLINE = '\n';

    int area;

    // code execution

    area = LENGTH * WIDTH;

    cout << area;

    cout << NEWLINE;

    return 0;
}

/*
You can define constants either using #define or the const
keyword.

Constants should be defined with uppercase letters.
*/
