#include <iostream>
using namespace std;

template <typename T>

T add(T a, T b)
{
    return (a + b);
}

int main()
{
    cout << add(1, 2) << endl;
    cout << add(1.0, 2.5) << endl;
    // the next line did not compile as add(int,float)
    // cout << add(1,2.5) << endl;
    return 0;
}

/*
<T>emplates
Generic programming independent of type
*/