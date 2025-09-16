#include <iostream>

// Function declaration
void func(void);
static int count = 10; /* Global variable */
auto x = 4;
// not without initialization

// like: auto x ;
int main()
{
    while (count--)
    { // two minus!!
        func();
    }

    std::cout << "And x is " << x << std::endl;
    return 0;
}

// Function definition
void func(void)
{
    static int i = 5; // local static variable
    i++;
    std::cout << "i is " << i;
    std::cout << " and count is " << count << std::endl;
}

/*
auto (automatic type deduction since C++11)
register (quick access but no memory address, deprecated)

static I (C and C++ keep values after function calls)
static II (C++ within classes work on classes not on objects)

extern (other files)
mutable (class objects)
*/