#include <iostream>

using namespace std;

// Global variable used from other files
// produces warning

extern int e = 1; // avoid if possible

// Global variable declaration for this file
int g;

int main()
{

    // Local variable declaration:

    int a, b;

    e = 3;

    // actual initialization

    a = 10;

    b = 20;

    g = a + b;

    // e = g -1; // would not work

    cout << g << endl;

    cout << e << endl;

    return 0;
}

/*

1. extern Declaration of e: The extern int e = 1; line is problematic.
The extern keyword is used to declare a variable that is defined in another file,
and it should not have an initialization.

The proper way to use extern is to declare
the variable without initializing it in the header or source file.
Initialization should be done only in one translation unit (typically in a .cpp file).

Correcting the extern usage:

Instead of extern int e = 1;, declare it as:
extern int e;  // Declaration only

You would define it (and initialize it) in another file, e.g., main.cpp or another .cpp
file:
int e = 1;  // Definition and initialization

2. Global Variable g: You declared g as a global variable with int g;. While this works,
it's typically better to avoid global variables unless absolutely necessary,
as they can introduce unexpected side effects.
Local variables (like the ones used in main) are generally preferred because their
scope is limited to the function.

*/