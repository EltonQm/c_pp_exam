#include <iostream>
using namespace std;

class Person
{
private:
    int km = 0;

public:
    void eat(const string &food) const
    {
        cout << " I love to eat " << food << endl;
    }

    void getkm()
    {
        cout << "I can run " << km << " kms" << endl;
    }
};

class Cat
{
private:
    int catkm = 4;

public:
    void cateat(const string &catfood)
    {
        cout << " I love to eat " << catfood << endl;
    }
    void getkm()
    {
        cout << "I can run " << catkm << " kms" << endl;
    }
};

class SingingMixin
{
public:
    void sing(int n = 1)
    {

        for (int i = 0; i < n; i++)
        {
            cout << "La lalala lalala" << endl;
        }
    }
};

class SingingPerson : public Person, public SingingMixin
{
};

class SingingCat : public Cat, public SingingMixin
{
public:
    SingingCat() { cout << "CatyCAtyRatty" << endl; }
};

int main()
{
    SingingPerson p;
    SingingCat c;

    p.eat("chicken");
    p.getkm();

    p.sing(5);

    c.getkm();
    c.cateat("rats");

    return 0;
}