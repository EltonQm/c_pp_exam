#include <iostream>
using namespace std;

/* local variable is same as a member's name */
/*
class Test
{
private:
    int x;

public:
    void setX(int x)
    {
        // The 'this' pointer is used to retrieve the object's x
        // hidden by the local variable 'x'
        this->x = x;
    }
    void print() { cout << "x = " << x << endl; }
};

// second example
int main()
{
    Test obj;
    int x = 20;
    obj.setX(x);
    obj.print();
    return 0;
}
*/

/*
class MyClass
{
    int value;

public:
    MyClass(int value)
    {
        // Parameter 'value' shadows member 'value'
        this->value = value; // 'this->value' refers to the member variable
    }

    void printValue()
    {
        std::cout << "Value: " << this->value << std::endl;
    }
};
*/

/*
#include <iostream>
using namespace std;

class Test
{
private:
    int x;
    int y;

public:
    Test(int x = 0, int y = 0)
    {
        this->x = x;
        this->y = y;
    }

    Test &setX(int a)
    {
        x = a;
        return *this;
    }

    Test &setY(int b)
    {
        y = b;
        return *this;
    }

    void print() { cout << "x = " << x << " y = " << y << endl; }
};

int main()
{
    Test obj1(5, 5);

    cout << "obj1 before chaange is : ";
    obj1.print();
    // Chained function calls.  All calls modify the same object
    // as the same object is returned by reference
    obj1.setX(10).setY(20);

    obj1.print();
    return 0;
}
// When a reference to a local object is returned, the returned reference can be used to chain function
// calls on a single object.
*/

class Student
{
private:
    int kg = 0;

public:
    Student(int kg = 0) { this->kg = kg; }
    Student run(int km)
    {
        this->kg = this->kg - km / 2;
        std::cout << "Running " << km << "km!\n";
        return (*this);
    }
    Student sleep(int x = 1) const
    {
        std::cout << "Sleeping " << x << " hours!\n";
        return (*this);
    }
    Student eat(int amount = 1)
    {
        std::cout << "Eating " << amount << " meal(s)!\n";
        return (*this);
    }
    int getWeight() const
    {
        return (this->kg);
    }
};

int main(int argc, char *argv[])
{
    Student fritz = Student(70);
    std::cout << fritz.eat(2).run(10).sleep(8).getWeight() << std::endl;
    return (0);
}