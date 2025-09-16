#include <iostream>
using namespace std;

// Abstract base class
class Shape
{
public:
    virtual double Area() const = 0; // pure virtual function
    virtual ~Shape() {}              // virtual destructor (good practice)
};

// Derived class: Rectangle
class Rectangle : public Shape
{
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double Area() const override
    {
        return width * height;
    }

private:
    double width, height;
};

// Derived class: Triangle
class Triangle : public Shape
{
public:
    Triangle(double b, double h) : base(b), height(h) {}
    double Area() const override
    {
        return 0.5 * base * height;
    }

private:
    double base, height;
};

int main()
{
    // Array of base class pointers pointing to different derived types
    Shape *shapes[2];
    shapes[0] = new Rectangle(5.0, 3.0); // base class pointer → Rectangle
    shapes[1] = new Triangle(4.0, 6.0);  // base class pointer → Triangle

    // Polymorphic behavior — correct Area() called for each shape
    for (int i = 0; i < 2; ++i)
    {
        cout << "Shape " << i + 1 << " area: " << shapes[i]->Area() << endl;
    }

    // Cleanup
    for (int i = 0; i < 2; ++i)
        delete shapes[i];

    return 0;
}

/*
What is polymorphism?
Polymorphism allows you to treat different derived objects through a common base
class pointer or reference, and call their overridden functions correctly at runtime.

✅ Example: Using polymorphism with virtual functions
🎯 Goal:
We'll create a Shape base class and two derived classes: Rectangle and Triangle.
We'll store them in an array of Shape* and call Area() on each — this only works
correctly because Area() is virtual.

We used a base class pointer (Shape*) to point to different derived types (Rectangle, Triangle)

We called Area() without knowing the exact type at compile time

Because Area() is declared virtual, the correct derived version was called at runtime

*/

/*
while you cannot instantiate an abstract class, you can declare pointers and references to it,
and that's exactly what enables polymorphism in C++.
Concept	Shown in code
Abstract class	Shape with virtual double Area() = 0;
Pointer to abstract class	Shape* shapePtr = &rect;
Reference to abstract class	Shape& shapeRef = rect;
Runtime dispatch	shapePtr->Area() calls Rectangle::Area()
Even though you can't do:

Shape s; // ❌ ERROR: abstract class
you can do:

Shape* s = new Rectangle(); // ✅ OK
✅ Summary

Can You Do This?	Example
❌ Shape s;	No — Shape is abstract
✅ Shape* s = new Rectangle();	Yes — pointer to abstract class
✅ Shape& s = rect;	Yes — reference to abstract class
*/
/*
#include <iostream>
using namespace std;

// Abstract class with a pure virtual function
class Shape {
public:
    virtual double Area() const = 0; // pure virtual
    virtual ~Shape() {}              // always good to have virtual destructor
};

// Derived class
class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double Area() const override {
        return width * height;
    }

private:
    double width, height;
};

int main() {
    // Create a Rectangle object
    Rectangle rect(5.0, 3.0);

    // ✅ Declare a pointer to the abstract class, point it to rect
    Shape* shapePtr = &rect;

    // ✅ Declare a reference to the abstract class, bind it to rect
    Shape& shapeRef = rect;

    // ✅ Polymorphic behavior — calls Rectangle::Area()
    cout << "Area via pointer: " << shapePtr->Area() << endl;
    cout << "Area via reference: " << shapeRef.Area() << endl;

    return 0;
}


*/