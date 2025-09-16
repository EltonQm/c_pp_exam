#include <iostream>

/*
class X
{
private:
    int x;

public:
    void setx(int x)
    {
        this->x = x;
    }
    void printValue()
    {
        std::cout << "x = " << x;
    }
};

int main()
{

    X test;
    int g = 10;
    test.setx(g);
    test.printValue();
}
*/

using namespace std;

class Test
{
private:
    int x;

public:
    Test(int x = 0) { this->x = x; }
    void change(Test *t) { this = t; }
    void print() { cout << "x = " << x << endl; }
};

int main()
{
    Test obj(5);
    Test *ptr = new Test(10);
    obj.change(ptr);
    obj.print();
    return 0;
}