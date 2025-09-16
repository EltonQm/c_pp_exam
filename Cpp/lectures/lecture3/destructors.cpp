

/*
A destructor is a special member function of a class that is executed whenever an object of it's class
goes out of scope or whenever the delete expression is applied to a pointer to the object of that class.
A destructor will have exact same name as the class prefixed with a tilde (~) and it can neither
return a value nor can it take any parameters.
Destructor can be very useful for releasing resources before coming out of the program like closing files,
releasing memories.

Static members;
We can define class members static using static keyword. When we declare a member of a class as static
it means no matter how many objects of the class are created, there is only one copy of the static
member.
A static member is shared by all objects of the class. All static data is initialized to zero when the
first object is created, if no other initialization is present.
We can't put it in the class definition but it can be initialized outside the class as done in the
following example by redeclaring the static variable, using the scope resolution operator :: to identify
which class it belongs to.
*/
/*
#include <iostream>
using namespace std;

class Animal
{
public:
    Animal(int age); // parameterized constructor
    Animal();        // default constructor
    ~Animal();       // destructor

    int getAge();
    static int animalCount;           // declared static (initialized outside)
    const static int MAXANIMALS = 10; // const static can be initialized inline

protected:
    int itsAge = 0;
};

// Initialize static member outside the class
int Animal::animalCount = 0;

// Return age of the animal
int Animal::getAge()
{
    return itsAge;
}

// Default constructor
Animal::Animal()
{
    animalCount += 1;
    cout << "Simple animal creation..." << endl;
}

// Parameterized constructor
Animal::Animal(int age)
{
    animalCount += 1;
    itsAge = age;
    cout << "Complex animal creation..." << endl;
}

// Destructor (called when object goes out of scope)
Animal::~Animal()
{
    cout << "Destroying an animal (painless!)" << endl;
    animalCount -= 1;
}

// Function that creates and destroys a local Animal object
void func()
{
    Animal animal;
    cout << "Inside func we have " << Animal::animalCount << " animal(s)" << endl;
}

// Main function
int main()
{
    Animal animal1;
    cout << "Animal 1's age: " << animal1.getAge() << endl;

    Animal animal2(12);
    cout << "Animal 2's age: " << animal2.getAge() << endl;

    func(); // animal inside func gets destroyed after func ends

    cout << "Inside main we have " << Animal::animalCount << " animal(s)" << endl;
    cout << "We leave now..." << endl;

    return 0;
}
*/

// Static memeber functions:
/*
By declaring a functon member as static, you make it independent of any particular object of the class.
A static member function can be called even if no objects of the class exist and the static functions
are accessed using only the class name and the scope resolution operator ::.
A static member function can only access static data member, other static member functions and any
other functions from outside the class.
*/

// Animal::animalCount => Animal::getAnimalCount()
/*
#include <iostream>
using namespace std;
class Animal
{
public:
    Animal(int age);
    Animal();
    ~Animal();
    int getAge();
    // static method with access to static vars only
    static int getAnimalCount()
    {
        return animalCount;
    }

protected:
    int itsAge = 0;
    static int animalCount;
};

// initialize outside
int Animal::animalCount = 0;
int Animal::getAge()
{
    return (itsAge);
}

// no argument constructor
Animal::Animal()
{
    animalCount += 1;
    cout << "simple animal creation..." << endl;
}

// parameterized constructor
Animal::Animal(int age)
{
    animalCount += 1;
    cout << "complex animal creation ..." << endl;
    itsAge = age;
}

Animal::~Animal()
{
    cout << "destroy an animal(painless!) " << endl;
    animalCount -= 1;
}

void func()
{
    Animal animal;
    cout << "inside func we have " << Animal::getAnimalCount() << " animals " << endl;
}

int main()
{
    Animal animal;
    cout << "Animal 1's age: " << animal.getAge() << endl;
    Animal animal2(12);
    cout << "Animal 2's age: " << animal2.getAge() << endl;
    func();
    cout << "inside main we have " << Animal::getAnimalCount() << " animals " << endl;
}
*/

// Const member functions
/*
You have used the const keyword to declare variables that would not change.
You can also use the const keyword with member functions within a class.
If you declare a class method const, you are promising that the method won't change the value of any
of the members of the class.
To decalre a class method constant, put the keyword const after the parentheses enclosing any parameters,
but before the semicolon ending the method declaration.
*/
/*
#include <iostream>
using namespace std;

class Animal
{
public:
    Animal(int age);
    Animal();
    ~Animal();
    int getAge() const; // no change on object
    void setAge(int age);
    static int animalCount;

protected:
    int itsAge = 0;
};

// initialize outside;
int Animal::animalCount = 0;

int Animal::getAge() const
{
    return (itsAge);
}

void Animal::setAge(int age)
{
    itsAge = age;
}

// no argument constructor
Animal::Animal()
{
    animalCount += 1;
    cout << "simple animal creation..." << endl;
}

// parameterized constructor
Animal::Animal(int age)
{
    animalCount += 1;
    cout << "complex animal creation ..." << endl;
    itsAge = age;
}

Animal::~Animal()
{
    cout << " destroy an animal (painless!)" << endl;
    animalCount -= 1;
}

int main()
{
    Animal animal;
    cout << "Animal 1's age: " << animal.getAge() << endl;
    animal.setAge(animal.getAge() + 1);
    cout << "Animal 1's age: " << animal.getAge() << endl;
    cout << "inside main we have " << Animal::animalCount << " animal " << endl;
}
*/
// Inheritance
/*
In oop, inheritance is the mechanism of basing an object or class upon another object
(prototype-based inheritance) or class (class-based inheritance), retaining similar implementation.
In most class-bsed oop, an object created through inheritance (a "child object") acquires all the
properties and behavoirs of the parent object (except: constructors, destructor, overloaded operators
and friend functions of the base class).
*/
/*
#include <iostream>

class Animal
{
public:
    void run() { std::cout << "Run,run,run!\n"; }
    void eat() { std::cout << "Eating generic food!\n"; }
};

class Cat : public Animal
{
public:
    void meow()
    {
        std::cout << "Meow, meow!\n";
    }
    // we have run automatically as we used: public Animal
};
// private inheritance is the default
// which is different to python where public is the default
class Dog : private Animal
{
public:
    void wuff()
    {
        std::cout << "Wuff, wuff!\n";
    }
    // as it is private inheritance
    // we must reimplement run and call Animal::run()
    // by the Dog::run() method
    void run()
    {
        Animal::run();
    }
};

int main()
{
    Cat mike = Cat();
    Dog fido = Dog();
    mike.run();
    mike.eat();
    mike.meow();
    fido.run(); // possible because we reimplemented it
    // fido.eat() not possible due to private inheritance :(
    fido.wuff();
}
*/
/*
Composition: Object composition is a way to combine objects or data types into more complex ones.
Compositions relate to, but are not the same as data structures, and common ones are the tagged union,
set, sequence and various graph structures, as well as the object used in oop.

inheritance - class B is-a class-A. A dog is-a animal.
composition - Class-B has-a Class-A. A dog has-a leg.

=> prefer compostion over inheritance if possible
inheritance:
 - both classes are in the same logical domain
 - subclass is a proper subtype of superclass
 - superclass implementaton is necessary or appropriate for the subclass
 - enhancements made by the subclass are primarily additive.

 composition:
 - there is truly a part of relationship
 - you only need very few methods of the base class
 - you like to hide methods from the base class
 - you like to delegate methods to different components from an object
*/
/*
#include <iostream>
#include <string>

class Tail
{
public:
    void wag() { std::cout << "Wag, wag!" << std::endl; }
};

class Dog
{
public:
    Dog(std::string name, std::string breed);
    int getConfidence() const { return (confidence); }
    void chase(std::string thing);

protected:
    int confidence = 0;
    std::string itsName;
    std::string itsBreed;
    Tail itsTail;
};

Dog::Dog(std::string name, std::string breed)
{
    itsName = name;
    itsBreed = breed;
    itsTail = Tail();
}

void Dog::chase(std::string thing)
{
    std::cout << itsName << " chases " << thing << "!" << std::endl;
    itsTail.wag();
    confidence += 1;
}

int main()
{
    Dog fido("Fido PEX XXII", "Australian Shepherd");
    fido.chase("Kathi PEX XXII");
    std::cout << "Fido confidence: " << fido.getConfidence() << std::endl;
}
*/

// Multiple Inheritance
/*
#include <iostream>
using namespace std;

class Bird
{
public:
    void fly()
    {
        cout << "I believe i can fly ..." << endl;
    }
    void beat()
    { // can't have eat in Bird and Horse
        cout << "Mhmm, dendrobena ..." << endl;
    }
};

class Horse
{
public:
    void run()
    {
        cout << "Run, run , run ..." << endl;
    }
    void heat()
    { // cant have eat in Bird and Horse
        cout << "Mhmm, apples ..." << endl;
    }
};

class Pegasus : public Bird, public Horse
{
public:
    void whoami()
    {
        cout << "Pegasus" << endl;
    }
};

int main()
{
    Pegasus pegi;
    pegi.fly();
    pegi.run();
    pegi.beat(); // can't have eat in Bird and Horse
    pegi.heat(); // can't have eat in Bird and Horse
    pegi.whoami();
}
*/
/*

MI is a feature in which an object or class can inherit characteristics and features from more than one
parent object or parent class. It is distinct from single inheritance where an object or class may only
inherit from one particular object or class.

The diamond problem creates an ambiguity as to which parent class implements said features.
This can be addressed in various ways including virtual inheritance.
Alternate methods of object composition not based on inheritance such as mixins and traits have also been
proposed to address the ambiguity.

Abstract class:
is conceptually, a class that cannot be instantiated and is usually implemented as a class that has one
or more pure virtual (abstract) functions.
A pure virtual function is one which must be overridden by any concrete(i.e non-abstract) derived class.
*/

/*
// Example abstract class
class AbstractClass{
    // public:
    // Pure virtual function makes
    // this class Abstract class.
    // virtual void AbstractMemberFunction() = 0;
    // Virtual functon.
    // virtual void NonAbstractMemberFunction1();
    // void NonAbstarctMemberFuncton2();

}
    In general an abstract class is used to define an implementation and is intended to be inherited from
    by concrete classes.
    It's a way of forcing a contract between the class designer and the users of that class.
    If we wish to create a concrete class from an abstract class we must declare and define a matching
    member function for each abstract member function of the base class.
*/
/*
// Virtual functions
#include <iostream>
class Animal
{
public:
    void eat()
    {
        std::cout << "I'm eating generic food.";
    }
};

class Cat : public Animal
{
public:
    void eat() { std::cout << "I'm eating a rat. "; }
};

void gomensa(Animal xyz) { xyz.eat(); }

int main()
{
    Animal animal = Animal();
    Cat cat = Cat();
    animal.eat(); // Output: "I'm eating generic food."
    cat.eat();    // Outputs: "I'm eating a rat."
    std::cout << std::endl
              << "But in the mensa: " << std::endl;
    gomensa(animal);
    gomensa(cat); // Why not a rat?
}
*/
/*
// Two functions
#include <iostream>
class Animal
{
public:
    void eat()
    {
        std::cout << "I'm eating generic food.";
    }
};

class Cat : public Animal
{
public:
    void eat() { std::cout << "I'm eating a rat."; }
};

void gomensa(Animal xyz) { xyz.eat(); }

// we add another func for cats' but that is tedious ...
void gomensa(Cat xyz) { xyz.eat(); }
int main()
{
    auto animal = Animal(); // or Animal animal = Animal ()
    auto cat = Cat();
    animal.eat();
    cat.eat();
    std::cout << "Now in the mensa: " << std::endl;
    gomensa(animal);
    gomensa(cat);
}
*/
/*

Can Instantiate?	Case
✅ Yes	Class has public, accessible constructor
❌ No	Constructor is private or deleted
❌ No	Class is abstract (pure virtual function not implemented)
❌ No	Constructor requires parameters, and none are given

*/