#include <iostream>

/*
- should be only used if speed is really a problem
- body of iinline functions is copied into the caller scope
- speed improvements as number of jumps between functions is reduced
- but size increase as same code is copied into multiple places
- useful for short functions
- inline is a proposal for the complier
- don't use #define macros, better use inline
*/
/*
inline int Double(int x);

int main()
{
    int x = 5;
    std::cout << "Hello double of x is " << Double(x) << std::endl;
    return (0);
}

inline int Double(int x)
{
    return (x * 2);
}
// Non-essential feature - I do not recommended to use it!
*/

// Recursive function
// small elegant, but often inefficient
/*
int fib(int x)
{
    if (x < 3)
        return (1);

    return (fib(x - 2) + (fib(x - 1)));
}
int main()
{
    for (int x = 1; x < 8; x++)
    {
        std::cout << "Fibonacci of " << x
                  << "=" << fib(x) << std::endl;
    }
    return (0);
}
*/

// Iterative function
// often more efficient but less elegant
/*
int fib(int x)
{
    int y = 1;
    int z = 1;
    int tmp = 0;
    if (x < 3)
    {
        return (1);
    }
    for (int i = 3; i <= x; i++)
    {
        tmp = y;
        y = z;
        z = tmp + y;
    }
    return (y + z);
}

int main()
{
    for (int x = 1; x < 8; x++)
    {
        std::cout << "Fibonacci of " << x << "=" << fib(x) << std::endl;
    }
    return (0);
}
*/
/*
Pointers and References
• Questions: how to return more than one value/item?
• Answer 1: in principle you can’t – but you can write
functions which are called with references as arguments.
• Answer 2: you can return a data structure which keeps more
than one value, such as array, vector, list, map etc.
• Calls:
– call by value - argument variable can’t be changed
– call by reference - argument variable can be changed
∗ using pointers
∗ using references (C++ only)
• Call by references:
– to change the input variable (not so recommended)
– for eﬃciency reasons (no copy of large data structures)
*/

// Call by references using pointers
/*
void swap(int *n, int *m);

int main()
{
    int x = 4;
    int y = 5;
    std::cout << "Before swap x = " << x << " - y = "
              << y << std::endl;
    swap(&x, &y);
    std::cout << "After swap x " << x << " - y = "
              << y << std::endl;
    return 0;
}

void swap(int *n, int *m)
{
    int temp = 0;
    temp = *m;
    *m = *n;
    *n = temp;

    return;
}
*/

/*
Remember if x is a pointer:
* x is the value and x is the memory address
* but assignment is like this *x = value

Remember if x is a normal variable
* x is the value and &x is the memory address(pointer)
*/

// Call by references using references
/*
A reference variable is an alias, that is, another name
for an already existing value.
Once a reference is initialized with a variable, either the
variable name or the reference name may be used to refer to the
variable.
*/
/*
void swap(int &rn, int &rm); // only here special symbols

int main()
{
    int x = 6;
    int y = 7;
    std::cout << "Before swap x " << x << " - y = "
              << y << std::endl;
    swap(x, y); // easier to read
    std::cout << "After swap x = " << x << " -y = "
              << y << std::endl;
    return 0;
}

void swap(int &rn, int &rm)
{
    int temp = 0;
    temp = rm;
    rm = rn;
    rn = temp;
    return;
}
*/
/*
// References can't be reassigned
int main()
{
    int x = 6;
    int y = 7;
    int &rx = x; // rx is a reference on x
    std::cout << "x = " << x << " rx = " << rx << " y = " << y << std::endl;

    rx = y; // oops don't do this, x is changed ...
    std::cout << "x = " << x << " rx = " << rx << " y = " << y << std::endl;
    return 0;
}
*/

// const fucntion argument
/*
- sometimes you call functions per reference for efficiency reasons
(large data structures normally) but not to modify the arguments
- to make clear that a variable even given as reference did not
change its values you can use the const keyword
- any change to the variable will produce a compile error
-- use const if you don't want to chnge the variable just like to
copy the address of the object into the function
*/
/*
void foo (const int &x) // x is a const reference
{
    x = 6; // compile error: a const reference canno
    // have its value changed
}
*/

// Some recommendations
/*
-- use references over pointers
-- use references in parameter lists
-- only use references for large data structures or if you
   like to modify the variable
-- when passing structs or classes (use const if read-only)
-- when passing an argument by reference, always use a const
   reference unless you need to change the value of the argument.

--** When not to pass by reference
-- When passing fundamental types that don't need to be modified. (use passs by value)
*/

/* chatgpt explanantion
## Use references instead of pointers if:

You do not need to reassign the address (i.e., you don't need to point to something else later).

You want to modify the original variable.

You are dealing with large data structures and want to avoid copying.

## Use pointers instead of references if:

You need to represent a nullable or optional object.

You need to reassign to point to different data.

You want to use dynamic memory management.

*/

// Function overloading
/*
Function overloading or method overloading is the ability to create multiple
functions of the same name with different implemtation.
Calls to the overloaded functions will run a specific implementaion of the
function call to perform different tasks depending on context.
*/

// evil C-way
/*
#define dadd(x, y) ((x + y))
// tedious polymorphic way
int add(int x, int y) { return (x + y); }
float add(float x, float y) { return (x + y); }
double add(double x, double y) { return (x + y); }
double add(int x, float y) { return (x + y); }
// ... and more
int main()
{
    int ix = 6;
    int iy = 7;
    float fx = 16.2;
    float fy = 17.2;
    double dx = 26.2;
    double dy = 27.2;
    std::cout << "Using tedious three functions ...  " << std::endl;
    std::cout << "ix+iy = " << add(ix, iy) << " - fx+fy = " << add(fx, fy) << " - dx+dy = " << add(dx, dy) << std::endl;
    std::cout << "Using a C #define ... " << std::endl;
    std::cout << "ix+iy = " << dadd(ix, iy) << " - fx+fy = " << add(fx, fy) << " - dx+dy = " << add(dx, dy) << std::endl;
    return 0;
}
*/

/*
#define dadd(x, y) ((x + y))
Pros: It's a preprocessor macro, so it's simple, quick, and works with any type (as long as + is valid).

Cons:

It doesn’t do type checking.

Debugging can be painful—it’s just a text substitution.

Can lead to subtle bugs due to precedence if you're not careful.
*/

// using auto as a return type
/*
auto add(auto x, auto y)
{
    return (x + y);
}

auto afib(auto x)
{
    if (x < 3)
        return 1;
    return afib(x - 1) + afib(x - 2);
}

int main()
{
    for (int x = 1; x < 8; ++x)
    {
        std::cout << "Fibonacci of " << x << " = " << afib(x) << std::endl;
    }

    std::cout << "add(12, 13) = " << add(12, 13) << std::endl;
    std::cout << "add(12.5, 13.3) = " << add(12.5, 13.3) << std::endl;

    return 0;
}
*/

// Polymorphic functions with templates
/*
template <typename T>
T add(T a, T b)
{
    T result;
    result = a + b;
    return (result);
}

int main()
{
    int ix = 6;
    int iy = 7;
    float fx = 16.2;
    float fy = 17.2;
    double dx = 26.2;
    double dy = 27.2;
    std::cout << "Template version:" << std::endl;
    std::cout << "ix+iy= " << add(ix, iy) << " - fx+fy= " << add(fx, fy)
              << " - dx+dy= " << add(dx, dy) << std::endl;
    return 0;
}
*/
/*
template <typename T, typename S>
T add(T a, S b)
{ // two types
    T result;
    result = a + b;
    return (result);
}

int main()
{
    int ix = 6;
    int iy = 7;
    float fx = 16.2;
    float fy = 17.2;
    double dx = 26.2;
    double dy = 27.2;
    std::cout << "Template version:" << std::endl;
    std::cout << "ix+iy= " << add(ix, iy) << " - fx+fy= " << add(fx, fy)
              << " - dx+dy= " << add(dx, dy) << " - ix+fy+dy = " << add(add(ix, fy), dy) << std::endl;
    return 0;
}
*/

// Lambda: an unnamed fucnton object capable of capturing variabels on scope(function closure).
/*
#include <algorithm>
#include <vector>

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 7, 9, 12, 123};
    for (auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::transform(v.begin(), v.end(), v.begin(), [](int i)
                   {
        // this is a comment inside lambda
        if (i > 3){
            return 3;
        }
        else {
            return i;
        } });
    for (auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
*/

/*
std::transform – Transform each element with a lambda

std::transform(v.begin(), v.end(), v.begin(), [](int i) {
    if (i > 3) {
        return 3;
    } else {
        return i;
    }
});

v.begin(), v.end() → input range

v.begin() → destination (overwrites the original values)

The lambda takes each i, and:

If i > 3, it returns 3

Otherwise, it keeps i
*/

// Using arrays
// this style is not recommended
/*
int main()
{
    // an array with 5 rows and 2 columns
    int a[5][2] = {{0, 0}, {1, 2}, {2, 4}};

    // output each array element's value
    for (int i = 0; i < 3; i++)

        for (int j = 0; j < 2; j++)
        {
            std::cout << "a[" << i << "][" << j << "]: ";
            std::cout << a[i][j] << std::endl;
        }
    return 0;
}
*/
/*
#include <vector>
using namespace std;
// c-style with size
double getAverage(int arr[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; ++i)
    {
        sum += arr[i];
    }
    return (double(sum) / size);
}

double getAverage(vector<int> arr)
{
    // C++ style
    double average = 0;
    for (auto i : arr)
    {
        average += i;
    }
    return (average / arr.size());
}

int main()
{
    int balance[5] = {1000, 2, 3, 17, 50};
    double avg;
    // pass pointer to the array as an assignment
    avg = getAverage(balance, 5);
    // output the returned value
    cout << "Average value is : " << avg << endl;
    vector<int> v = {2, 3, 4, 5};
    cout << "Average value vector is : " << getAverage(v) << endl;
    return 0;
}
*/
/*
#include <vector>
using namespace std;

typedef std::vector<std::vector<int>> Matrix;
int main()
{
    Matrix mt = {
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {1, 0, 1, 0}};

    cout << mt[0][0] << endl;
    cout << mt[0][1] << endl;
    return (0);
}
*/

// Structures and Classes
// A structure is as user defined data type combining data items of different kinds
// a C++ class with only public variables but no methods
/*
#include <cstring>
using namespace std;

struct Books
{
    char title[50];
    char author[50];
    char subject[100];
    int book_id;
};

int main()
{
    // struc here is optional
    struct Books Book1; // Declare Book1 of tpe Book
    struct Books Book2; // Declare Book2 of type Book

    // book1 specification
    strcpy(Book1.title, "Learn C++ Programming");
    strcpy(Book1.author, "Chand Miyan");
    strcpy(Book1.subject, "C++ Programming");
    Book1.book_id = 6495407;

    // book2 specification
    strcpy(Book2.title, "Telecom Biling");
    strcpy(Book2.author, "Yakit Singha");
    strcpy(Book2.subject, "Telecom");
    Book2.book_id = 6495700;

    // Print Book1 info
    cout << "Book 1 title : " << Book1.title << endl;
    cout << "Book 1 author : " << Book1.author << endl;
    cout << "Book 1 subject : " << Book1.subject << endl;
    cout << "Book 1 id : " << Book1.book_id << endl;

    printf("\n");

    // Print Book2 info
    cout << "Book 2 title : " << Book2.title << endl;
    cout << "Book 2 author : " << Book2.author << endl;
    cout << "Book 2 subject : " << Book2.subject << endl;
    cout << "Book 2 id : " << Book2.book_id << endl;

    return 0;
}
*/

/*
A class is used to specify the form of an object and it combines data representation and methods for
manipulating that data into one neat package. The data and functions within a class are called members
of the class.
The data and functions within a class are called members of the class.

- a class with just some public properties is like a struct.
- public properties/methods can be accessed directly from any code outside of the class
- protected properties or methods can be accessed from classes/ibjects inheriting from that class
- private properties and methods can be used only inside of this class
- access operator for member prperties and methods is the dot
*/

// struct Books ==> class Book I
/*
#include <cstring>
using namespace std;

class Books
{
public:
    char title[50];
    char author[50];
    char subject[100];
    int book_id;
};

void printBook(Books book);

int main()
{

    Books Book1; // Declare Book1 of tpe Book
    Books Book2; // Declare Book2 of type Book

    // book1 specification
    strcpy(Book1.title, "Learn C++ Programming");
    strcpy(Book1.author, "Chand Miyan");
    strcpy(Book1.subject, "C++ Programming");
    Book1.book_id = 6495407;

    // book2 specification
    strcpy(Book2.title, "Telecom Biling");
    strcpy(Book2.author, "Yakit Singha");
    strcpy(Book2.subject, "Telecom");
    Book2.book_id = 6495700;

    printBook(Book1);
    printBook(Book2);

    return 0;
}

void printBook(Books book)
{
    // Print Book1 info
    cout << "Book title : " << book.title << endl;
    cout << "Book author : " << book.author << endl;
    cout << "Book subject : " << book.subject << endl;
    cout << "Book id : " << book.book_id << endl;
}
*/
/*
// struct Books ==> class Book II
#include <cstring>
using namespace std;

class Books
{
public:
    string title = " ";
    string author = " ";
    string subject = " ";
    int book_id = 0;

    void printBook()
    {
        cout << "Book title : " << title << endl;
        cout << "Book author : " << author << endl;
        cout << "Book subject : " << subject << endl;
        cout << "Book id : " << book_id << endl;
    }
};

int main()
{

    Books Book1; // Declare Book1 of type Book
    Books Book2; // Declare Book2 of type Book

    // book1 specification
    Book1.title = "Learn C++ Programming";
    Book1.author = "Chand Miyan";
    Book1.subject = "C++ Programming";
    Book1.book_id = 6495407;

    // book2 specification
    Book2.title = "Telecom Biling";
    Book2.author = "Yakit Singha";
    Book2.subject = "Telecom";
    Book2.book_id = 6495700;

    cout << "Class Example 2 ..\n";

    Book1.printBook();
    Book2.printBook();

    return 0;
}
*/

// adding functions to a class

/*
A member function of a class is a function that has its definition or its prototype within the class
definition like any other variable.
It operates on any object of the class of which it is a member and has access to all the members of a
class of that object.
*/
/*
using namespace std;
class Animal
{
public:
    int age = 0;
    // declare ans define in class definition
    void eat()
    {
        cout << "I'm eating generic food." << endl;
    }
    // declare here only -define later
    void run(float km);

protected:
    float km = 0.0;
    string name = "Max Musteranimal";
};

// define here
void Animal::run(float akm)
{
    cout << "I'm " << name << ",running now : " << akm << " km " << endl;
    km = km + akm;
    cout << "Total runnings: " << km << "km" << endl;
}
int main()
{
    Animal animal;
    animal.eat();
    animal.run(3);
    animal.run(2.1);
}
*/

// Getter and setter functions:
/*
Public variables are usually discouraged, and the better form is to make all variables private
and access them with getters and setters.
*/

/*
class Animal
{
public:
    void setAge(int age);
    int getAge() const;
    // const means does not chnage anything
    // within the object, here it is optional
protected:
    int itsAge = 0;
};
*/
/*
#include <string>
using namespace std;

class Animal
{
public:
    // declare and define in class definition
    void eat()
    {
        itsWeight += 0.5;
        cout << "I'm eating generic food." << endl;
    }

    // getter and setter methods
    int getAge() const { return (itsAge); }
    void setAge(int age)
    {
        itsAge = age;
        return;
    }
    float getWeight() const { return (itsWeight); }
    // declare here only -define later
    void run(float km);

protected:
    float itsKm = 0.0;
    string itsName = "Maxi Musteranimalin";
    int itsAge = 0;
    float itsWeight = 0.0;
};

// define here
void Animal::run(float km)
{
    cout << "I'm running now: " << km << "km" << endl;
    itsKm += km;
    itsWeight -= km * 0.1;
    cout << "Total runnings: " << itsKm << "km" << endl;
}

int main()
{
    Animal animal;
    animal.eat();
    animal.run(3);
    animal.eat();
    animal.run(2.1);
    cout << "Current weight: " << animal.getWeight() << endl;
}
*/

// Constructor
/*
A class constructor is a special member function of a class that is executed whenever we create new
objects of that class.
A constructor will have exact same name as the class and it does not have any return type at all,
not even void. Constructors can be useful for setting intial values for certain member variables.
*/
/*
using namespace std;
class Animal
{
public:
    // constructor 1
    Animal(int age) { itsAge = age; }

    // constructor 2 (standard)
    Animal() { itsAge = 0; }
    void eat()
    {
        itsWeight += 0.5;
        cout << "I'm eating generic food." << endl;
    }

    // getter and setter methods
    int getAge() { return (itsAge); }
    void setAge(int age)
    {
        itsAge = age;
        return;
    }
    float getWeight() { return (itsWeight); }

    // declare here nly -define later
    void run(float km);

protected:
    float itsKm = 0.0;
    char itsName[50] = "";
    int itsAge = 0;
    float itsWeight = 0.0;
};

// define here
void Animal::run(float km)
{
    cout << "I'm running now: " << km << "km" << endl;
    itsKm += km;
    itsWeight -= km * 0.1;
    cout << "Total runnings; " << itsKm << " km " << endl;
}
int main()
{
    Animal animal;
    animal.eat();
    animal.run(3);
    animal.eat();
    animal.run(2.1);
    cout << "Current weight: " << animal.getWeight() << endl;
    cout << "Animals age: " << animal.getAge() << endl;
    Animal animal2(12);
    cout << "Current weight: " << animal2.getWeight() << endl;
    cout << "Animals age: " << animal2.getAge() << endl;
}
*/

/*
// Outside constructor definitions
using namespace std;
// just declarations within the class body
// public part is the puclished interface for programmers

class Animal
{
public:
    // Constructor 1
    Animal(int age);
    // constructor 2
    Animal();
    int getAge();

protected:
    int itsAge = 0;
};

// the implementation, details
// normally not required to be read by the programmer

int Animal::getAge()
{
    return (itsAge);
}

// no argument constructor
Animal::Animal()
{
    cout << "simple animal creation..." << endl;
}

// parameterized constructor
Animal::Animal(int age)
{
    cout << "complex animal creation..." << endl;
    itsAge = age;
}

// use case
int main()
{
    Animal animal;
    cout << "Animal 1's age: " << animal.getAge() << endl;
    Animal animal2(12);
    cout << "Animal2's age: " << animal2.getAge() << endl;
}
*/

// Using initialization lists
// without list
/*
class Animal
{
public:
    // constructor 1
    Animal(int age) { itsAge = age; }

protected:
    int itsAge = 0;
};

// same but with initialization list:
class Animal
{
public:
    // constructor 1
    Animal(int age) : itsAge(age) {}

protected:
    int itsAge = 0;
};
*/

/*
// When to use initialization lists
1) for initialization of non-static const data members:

const data members must be initalized using initializer lists.
In the following example, 't' is a const data member of class "Test" and is initialized using an
initializer list.
*/
/*
#include <string>
using namespace std;
class Animal
{
public:
    Animal(string name) : name(name) {} // Initializer list
    string getName() const { return name; }

private:
    const string name; // should not change after birth
};
int main()
{
    Animal fido("Fido the Collie");
    cout << fido.getName() << endl;
    return 0;
}
*/
/*
2) For initialization of reference members:
Reference members must be initialized using initializer lists. In the followinf exampld, 't' is a
reference member of class 'Test' and it is initialized using an initializer list.
*/
// Initializaton of reference data members

#include <string>
using namespace std;

class Animal
{
    int &age;

public:
    Animal(int &age) : age(age) {}
    int getAge() const { return age; }
};
int main()
{
    int age = 2;
    Animal fido(age);
    cout << fido.getAge() << endl;
    age = 3;
    cout << fido.getAge() << endl;
    return 0;
}

// This is not recommended because it breaks encapsulation.
// It's only useful if you make references to huge external data structures.

// getter and setter methods c++
