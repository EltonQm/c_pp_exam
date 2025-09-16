/*
#include <iostream>
using namespace std;

// Base class A
class A
{
protected:
   int x; // Protected member, accessible to derived classes and friends
public:
    A() { x = 0; } // Constructor initializes x to 0
    friend void show();
};

// Derived class B inherits publicly from A
class B: public A
{
public:
    B() : y(0) {} // Constructor initializes y to 0
private:
    int y; // Private member, only accessible within class B or its friends
};
};

void show()
{
    A a; // Create object of class A
    B b; // Create object of class B
    // Accessing protected member x of A via friendship
    cout << "The default value of A::x = " << a.x << " ";

    // Accessing private member y of B directly — this is actually illegal and
    will cause a compile-time error
    cout << "The default value of B::y = " << b.y;
}
*/
/*
Explanation :
Please note that show() is a friend of class A, so there should not be any
compiler error in accessing any member of A in show(). Class B is inherited from
A, the important point to note here is friendship is not inherited. So show()
doesn't become a friend of B and therefore can't access private members of B.
*/
/*
#include <iostream>
using namespace std;

// Forward declaration of class B (needed because A refers to B)
class B;

// Class A definition
class A {
    int a; // Private data member

public:
    A() : a(0) {} // Constructor initializes 'a' to 0

    // Member function that takes references to objects of A and B
    void show(A &x, B &y);
};

// Class B definition
class B {

private:
    int b; // Private data member

public:
    B() : b(0) {} // Constructor initializes 'b' to 0
    // Granting friend access to A's member function 'show'
    // This allows 'A::show' to access B's private members
    friend void A::show(A &x, B &y);
};

// Definition of A's member function 'show'
// Because it's a friend of B, it can access B's private member 'b'
void A::show(A &x, B &y) {
  x.a = 10; // Modify A's private member
  cout << "A::a=" << x.a << " B::b=" << y.b; // Access B's private member
}

// Main function
int main() {
  A a;
  B b;
  a.show(a, b); // Call 'show' with references to a and b
  return 0;
}
*/
/*
Explanation
This is simple program where a function of class A is declared as friend of
class B. Since show() is friend, it can access private data members of B.
*/
// Friend of the class can be member of some other class but Friend functions
// are not the members of a particular class.
/*
#include <iostream>
using namespace std;

class Test {
private:
  int x;
  int y;

public:
  Test(int x = 0, int y = 0) {
    this->x = x;
    this->y = y;
  }
  Test setX(int a) {
    x = a;
    return *this;
  }
  Test setY(int b) {
    y = b;
    return *this;
  }
  void print() { cout << "x = " << x << " y = " << y << endl; }
};

int main() {
  Test obj1;
  obj1.setX(10).setY(20);
  obj1.print();
  return 0;
}
*/

// Initialization
// 1) For initialization of Non-Static const Data Members (must)
/*
#include<iostream>
using namespace std;

class Test {
    const int t;
public:
  //Initializer list must be used
    Test(int t):t(t) {}
    int getT() { return t; }
};

int main() {
    Test t1(10);
    cout<<t1.getT();
    return 0;
}
*/
// 2) for initialization of Reference members (must)
/*
#include <iostream>
using namespace std;

class Test {
  int &t;

public:
  Test(int &t) : t(t) {} // Initializer list must be used
  int getT() { return t; }
};

int main() {
  int x = 20;
  Test t1(x);
  cout << t1.getT() << endl;
  x = 30;
  cout << t1.getT() << endl;
  return 0;
}
*/
// 3) For initialization of Member Objects that do not have a
// Default Constructor
/*
#include <iostream>
using namespace std;

class A {
  int i;

public:
  A(int);
};

A::A(int arg) {
  i = arg;
  cout << "A's Constructor called: Value of i: " << i << endl;
}

// Class B contains object of A
class B {
  A a;

public:
  B(int);
};

B::B(int x) : a(x) { // Initializer list must be used
  cout << "B's Constructor called";
}

int main() {
  B obj(10);
  return 0;
}
*/
// 4) For initializatoin of Base Class Members
/*
#include <iostream>
using namespace std;

class A {
    int i;
public:
    A(int );
};

A::A(int arg) {
    i = arg;
    cout << "A's Constructor called: Value of i: " << i << endl;
}

// Class B is derived from A
class B: A {
public:
    B(int );
};

B::B(int x):A(x) { //Initializer list must be used
    cout << "B's Constructor called";
}

int main() {
    B obj(10);
    return 0;
}
*/
// 5) When the Constructor's Parameter Name is the Same as Data Member
/*
If the constructor's parameter name is the same as the data member name
then the data member must be initialized either using -- this pointer or
initializer list.
*/

#include <iostream>
using namespace std;

class A {
  int i;

public:
  A(int);
  int getI() const { return i; }
};

A::A(int i) : i(i) {} // Either Initializer list or this pointer must be used
// The above constructor can also be written as
// A::A(int i) {
//   this->i = i;
//}
//

int main() {
  A a(10);
  cout << a.getI();
  return 0;
}
