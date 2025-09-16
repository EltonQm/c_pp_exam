#include <iostream>
// import all methods from iostream std namespace
// not recommended for header files!!
using namespace std;
// first name space
namespace mapp
{
    int x = 20;
    void func1()
    {
        cout << "Inside mapp::func1 x is: " << x << endl;
    }

    int func2()
    {
        cout << "Inside mapp::func2 x is: " << x << endl;
        func1();
        return (1);
    }
}
int main()
{
    int x = 1;
    // Calls function from first name space.
    mapp::func2();
    cout << "x is: " << x << endl;
    // Good place to locally import a namespace
    using namespace mapp;
    // Calls function again after import.
    func2();
    // variable x in main namespace is not overwritten

    cout << "x is: " << x << endl;
    return 0;
}

/*
• can be used to isolate code and variables
• image a global variable x or a function test
• better to have your variables and functions in their own namespace
*/