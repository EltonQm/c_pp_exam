
/*
A friend function of a class is defined outside that class' scope but it has the
right to access all private and protected members of that class. Even though the
prototype for friend functions appear in the class definition, friends are not
members functions.
A friend can be a function, function template or member function, or a class or
class template, in which case the entire class and all of its members are
friends.

General Consideraton
1. Friends should be used only for limited purposes.
2. Too many functions or external classes are declared as friends of a class
with protected or private data.
3. Friends lessens the value of encapsulation of seperate classes in oop
4. Friendship is not mutual. If a class A is friend of B, then B doesn't become
friend of A automatically
5. Friendship is not inherited
6. The concept of friends is not there in Java
*/

/*
#include <iostream>

using namespace std;

class Box {
private:
  double width;

public:
  friend void printWidth(Box box);
  void setWidth(double wid) { width = wid; };
};

// Note: printWidth() is just a normal function
// is not a member function of any class

void printWidth(Box box) {

  //Because printWidth() is a friend of Box, it can directly access any member
of this class

  cout << "Width of box : " << box.width << endl;
}

// Main function for the program
int main() {
  Box box;
  // set bix width with memeber function
  box.setWidth(10.0);
  // Use friend function to print the width
  printWidth(box);
  return 0;
}
*/
/*
Copy constructor is a member function which initializes an object using another
object of the same class. A copy constructor has the following general function
prototype.

ClassName (const Classname &old_obj);
If we don't define our own copy constructor, C++ complier creates a default copy
constructor for each class which does a member-wise copy between objects. The
complier created copy constructore works fine in general.

We need to define our own copy constructor only if an object has pointers or any
runtime allocation of the resource like file handle, a network connection...etc

If we don't do this then, we have a memory leak! Memory for those pointers is
not released if an object is destroyed
*/
/*
#include <cstring>
#include <iostream>

using namespace std;

class String {
private:
  char *s;
  int size;

public:
  String(const char *str = NULL); // constructor
  ~String() { delete[] s; }       // destructor
  String(const String &oldstr);   // copy constructor

  void print() { cout << s << endl; }
  void change(const char *);
};

String::String(const char *str) {
  size = strlen(str);
  s = new char[size + 1];
  strcpy(s, str);
}

void String::change(const char *str) {
  delete[] s;
  size = strlen(str);
  s = new char[size + 1];
  strcpy(s, str);
}

// copy constructor, argument is the class object itself
String::String(const String &oldstr) {
  size = oldstr.size;
  s = new char[size + 1]; // create a mutable block of memory,
  strcpy(s, oldstr.s);
}

int main() {
  String str1("GeeksQuiz");
  String str2 = str1; // copy constructor in action
  str1.print();       // what is printed?
  str2.print();
  str2.change("GeeksforGeeks");
  str1.print(); // what is printed now?
  str2.print();

  return 0;
}
*/
/*
// Removed copy constructor
#include <cstring> // for strlen and strcpy
#include <iostream>

using namespace std;

class String {
private:
  char *s;
  int size;

public:
  String(const char *str = NULL); // constructor
  ~String() { delete[] s; }       // destructor
  void print() { cout << s << endl; }
  void change(const char *);
};

String::String(const char *str) {
  size = strlen(str);
  s = new char[size + 1]; // +1 for null terminator
  strcpy(s, str);         // copy string contents into s
}

void String::change(const char *str) {
  delete[] s; // Free the previously allocated memory
  size = strlen(str);
  s = new char[size + 1];
  strcpy(s, str); // Copy new string into s
}

int main() {
  String str1("GeeksQuiz");
  String str2 = str1;
  str1.print(); // what is printed?
  str2.print();
  str2.change("GeeksforGeeks");
  str1.print(); // what is printed now?
  str2.print();

  return 0;
}
*/
/*
TL;DR
void change(const char *); // declaration
Legal and means: “A function that takes a const char pointer, but we’re
notnaming it here.”

void change(const char *str); // definition
Also legal and clearer if you want to name the parameter in the declaration.

 | **Feature**           | **`const char*`**                                   | **`const std::string*`**                               |
 | --------------------- | --------------------------------------------------- | ------------------------------------------------------ |
 | **Points to**         | A C-style null-terminated string                    | A C++ `std::string` object                             |
 | **Memory Management** | Manual (you manage memory, allocation/deallocation) | Managed automatically by `std::string`                 |
 | **Ease of Use**       | ❌ No — must use `strcpy`, `strlen`, etc.            | ✅ Yes — use `.length()`, `.c_str()`, `.substr()`, etc. |
 | **Access Syntax**     | Requires raw pointer dereferencing (`->`)           | Yes, use `->` if pointer; or `.` if it's not a pointer |
 | **Safety**            | ❌ Error-prone (risks of buffer overflows, leaks)    | ✅ Much safer, modern, and preferred in C++             |


Alternative::
void String::change(const std::string &str) {
    size = str.length();
    s = new char[size + 1];
    strcpy(s, str.c_str());
}
- const std::string& avoids copying and doesn’t risk nullptr, unlike const
std::string*.
- Use const std::string& (not even *) in modern C++ for safer, cleaner code:
- Use const char* if you're working with raw strings (as in legacy C or
interfacing with C APIs).

This is called a C-style string or "raw string" because:
#include <stdio.h>
#include <string.h>

int main() {
    const char* s = "world";
    printf("%lu\n", strlen(s));  // prints 5
}

It's literally a pointer to an array of characters.

It ends with a null terminator (\0) to mark where the string stops.

You use functions like strlen(), strcpy(), and strcmp() to work with it.

In contrast, C++ std::string is:
#include <iostream>
#include <string>

int main() {
    std::string s = "world";
    std::cout << s.length();  // prints 5
}
A class (from the C++ Standard Library),

Safer (knows its own length, no need for \0),

Easier to manipulate (you can do s1 + s2, .length(), etc.),

Automatically handles memory for you.
*/

/*
strcpy(s, oldstr.s);

oldstr is an object of type String.
oldstr.s is a raw C-style string (char*) — and that’s what strcpy expects.

strcpy(char* dest, const char* src) requires both arguments to be of type char*
or const char*

String A("hello");
String B = A; // invokes copy constructor
A is the full class object.
A.s is the actual "hello" string (the char* data inside).

To access the string inside A, you use A.s, not A.

Why not strcpy(s, oldstr)?
Because oldstr is an instance of your String class — not a C-style string.
*/

/*
standard way to write a copy constructor in C++.  - by reference
his is the standard way to write a copy constructor in C++.

You pass an existing object by reference.

No pointer dereferencing is needed.

Cleaner, safer, idiomatic.

String str1("Hello");
String str2 = str1; // invokes copy constructor

*/

// problem both variables point to the same memory address
// program crashes are possible with pointers

/*
Every object in C++ has access to its own address through an important pointer
called this pointer. The this pointer is an implicit parameter to all member
functions.

Therefore inside a member function this may be used to refer to the invoking
object. Friend functions do not have a this pointer, because friends are not
members of a class.

Only member functions have a this pointer.
*/

/*
// this pointer example

#include <iostream>
using namespace std;

class Box {
public:
  // Constructor definition
  Box(double l = 2.0, double b = 2.0, double h = 2.0) {
    cout << "Constructor called. " << endl;
    length = l;
    breath = b;
    height = h;
  }

  double Volume() { return length * breath * height; }
  int compare(Box box) { return this->Volume() > box.Volume(); }
  int compare2(Box box) { return Volume() > box.Volume(); }

private:
  double length; // Length of a box
  double breath; // Breath of a box
  double height; // Height of a box
};

int main() {

  Box Box1(3.3, 1.2, 1.5); // Declare box1
  Box Box2(8.5, 6.0, 2.0); // Decalre box2

  if (Box1.compare(Box2)) {
    cout << "Box2 is smaller than Box1" << endl;
  } else {
    cout << "Box2 is equal to or larger than Box1" << endl;
  }
  cout << "compare: " << Box1.compare(Box2) << " compare2"
       << Box1.compare2(Box2) << endl;

  return 0;
}
*/

// WHne to use the this pointer
/*
When local variable's name is same as member'S name. this->x = x;
To return reference to the calling object: return *this;

The argparse header library uses return(*this) to allow nested method execution
Here the implementation of add_Argument and help return *this and allow this
method chaining via obj.method1().method2().method3()
*/

/*
Operator Overloading
YOu can redefine or overload most of the built-in-operators available in C++
Overloaded operators are functions with special names;
the keyword "operator" followed by the symbol for the operator beinh defined.
Like any other function, an overloaded operator has a return type and a
parameter list.

Box operator+(const Box&);

declares the addition operator that can be used to add two Box objects and
returns final Box object. Most overloaded operators may be defined as
non-members functions or as class member functions.

In case we define above functions as non-member functions of a class thern we
would have to pass two arguments for each operand as follows:

Box operator+(const Box&, const Box&);
*/
/*
// Operator overloading example
#include <iostream>
using namespace std;

class Box {
public:
  double getVolume(void) { return length * breadth * height; }
  void setLength(double len) { length = len; }
  void setBreadth(double bre) { breadth = bre; }
  void setHeight(double hei) { height = hei; }

  // Overload+ operator to add two Box objects
  Box operator+(const Box &b) {
    Box box;
    box.length = this->length + b.length;
    box.breadth = this->breadth + b.breadth;
    box.height = this->height + b.height;

    return box;
  }

private:
  double length;  // Length of a box
  double breadth; // Breadth od a box
  double height;  // Height of a box
};

// Main fucniton for the program
int main() {
  Box Box1;            // Declare Box1 of type Box
  Box Box2;            // Declare Box2 of type Box
  Box Box3;            // Declare Box3 of type Box
  double volume = 0.0; // Store volume of box here

  // box1 specification
  Box1.setLength(6.0);
  Box1.setBreadth(7.0);
  Box1.setHeight(5.0);

  // box2 specification
  Box2.setLength(12.0);
  Box2.setBreadth(13.0);
  Box2.setHeight(10.0);

  // volume of box 1
  volume = Box1.getVolume();
  cout << "Volume of Box1: " << volume << endl;

  // volume of box 2
  volume = Box2.getVolume();
  cout << "Volume of Box2 : " << volume << endl;

  // Add two object as follows
  Box3 = Box1 + Box2;

  // volume of box 3
  volume = Box3.getVolume();

  cout << "Volume of Box3 : " << volume << endl;

  return 0;
}
*/

// Namespace
/*
The namespace keyword allows you to create a new scope.
The name is optional, and can be omitted to create an unnamed namespace.

Once you create a namespace, you'll have to refer to it explicitly or use the
using keyword. A namespace is defined with a namespace block.
*/
/*
#include <iostream>
using namespace std;
// first name space
namespace first_space {
void func() { cout << "Inside first_space" << endl; }
} // namespace first_space

// second namespace
namespace second_space {
void func() { cout << "Inside second_space" << endl; }
} // namespace second_space

int main() {
  // Calls function from first name space
  first_space::func();
  // Calls function from second name space
  second_space::func();

  return 0;
}
*/

// Using dirsctive
/*
- Using directive you can also avoid prepending of namespaces with the using
namespace directive.
- The directive tells the complier that the subsequent code is making use of
names in the specified namespace.
- The namespace is thus implied for the following code
*/
/*
#include <iostream>
using namespace std;
namespace first_space { // firstname space
void func() { cout << "Inside first_space" << endl; }
} // namespace first_space

namespace second_space { // second name space
void func() { cout << "INside second_space" << endl; }
} // namespace second_space

using namespace first_space;
int main() {
  // This calls function from first name space
  // No use of “using” in header files!
  func();

  return 0;
}
*/
/*
// Nested Namespaces

#include <iostream>
using namespace std;

// first name space
namespace first_space {
void func() { cout << "Insdie first_space" << endl; }
// second name space
namespace second_space {
void func() { cout << "Inside second_space" << endl; }
} // namespace second_space
} // namespace first_space

using namespace first_space::second_space; // Inside second_space
// using namespace first_space;  // Insdie first_space
int main() {
  // This calls functon from second name space
  func();
  return 0;
}
*/

// Namespace aliases
/*
Namespace aliases allow the programmer to define an alternate name for a
namespce They are commonly used as a convenient shortcut for long or deeply
nested namespaces
*/
/*
Syntax
namespace alias_name = ns_name; (1)
namespace alias_name = ::ns_name; (2)
namespace alias_name = nested_name::ns_name (3)
The new alias_name provides an alternate method of accessing ns_name.
alias_name must be a name not previosly used.

alias_name is valid for the duration of the scope in which it is introduced.
*/
/*
#include <iostream>
namespace foo {
namespace bar {
namespace baz {
int qux = 42;
}
} // namespace bar
} // namespace foo

// c++17 nesting improvement
namespace foo::bar::baz {
int qix = 43;
}
namespace fbz = foo::bar::baz;

int main() { std::cout << fbz::qux << ' ' << fbz::qix << std::endl; }
*/
/*
// Namespace summary
- namespace allow to structure and organize code
- aviod name clashes for variables, functons and classes
- you can put related functionality in the same namespace
- namespace can be extended over several files
- you can import namespace qualifiers into the current scope
- abbreviate long namespace names using alias
- using namespaces is recommended for not so small projects
- decalre functions inside of the namespace, define them outside
- place namespaces into a folder of the same name if they consist of several
files or in a file of the same name if they span only one file
- c++ namespaces are much more convenient than R ones
 - no automatic imports of all functions
 - easier generation and nesting
*/
// Templates:
/*
Templates are the foundation of generic programming, which involves writing code
in a way that is independent of any particular type. A template is a blueprint
or formuala for creating a generic class or a function. The library containers
like iterators and algorithms are examples of generic programming and have been
developed using template concept.
There is a single definition of each container, such as vector, but we can
define many different kinds of vectors for example, vector<int> or
vector<string>
*/
// template function definition
/*
template<class type>
ret-type func-name(parameter list){
    // body of function
}
// type is a placeholder name for a data type used by the function
*/
/*
// Function template exmaple
#include <iostream>
#include <string>
using namespace std;

// no need for #define macros anymore
// references here as arguments

template <typename T> T Max(T const &a, T const &b) { return a < b ? b : a; }

int main() {
  int i = 39;
  int j = 20;
  cout << "Max(i,j): " << Max(i, j) << endl;
  double f1 = 13.5;
  double f2 = 20.7;
  cout << "Max(f1,f1): " << Max(f1, f2) << endl;

  string s1 = "Hello";
  string s2 = "World";
  cout << "Max(s1,s2): " << Max(s1, s2) << endl;

  return 0;
}
*/
/*
Tenary operator
if (a < b)
    return b;
else
    return a;

Condition: a < b

If a < b is true, return b

Otherwise, return a
*/
/*
// Clas template based on std:vector
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T> class Stack {
private:
  vector<T> elems; // elmements
public:
  void push(T const &elem); // push element
  T pop();                  // pop element
  T top() const;            // return top element
  bool empty() const;       // return true if empty
};

template <class T> bool Stack<T>::empty() const { return (elems.empty()); }

template <class T> void Stack<T>::push(T const &elem) {
  // append copy of passed element
  elems.push_back(elem);
}

template <class T> T Stack<T>::pop() {
  T elem;
  if (elems.empty())
    throw out_of_range("Stack<T>::pop(): empty stack");
  elem = top();
  elems.pop_back(); // remove last element
  return (elem);
}

template <class T> T Stack<T>::top() const {
  if (elems.empty()) {
    throw out_of_range("Stack<T>::top(): empty stack");
  }
  // return copy of last element
  return elems.back();
}

int main() {
  try {
    Stack<int> intStack;       // stack of ints
    Stack<string> stringStack; // stack of strings
    // manipulation int stack
    intStack.push(7);
    intStack.push(9);
    cout << intStack.top() << endl;
    // manuipulation string stack
    stringStack.push("Hello");
    stringStack.push("World!");
    cout << stringStack.top() << std::endl;
    cout << stringStack.pop() << std::endl;
    stringStack.pop();
    stringStack.pop();

  } catch (exception const &ex) {
    cout << "Exception: " << ex.what() << endl; // cerr better
    return -1;
  }
}
*/
/*
// USing the array class
#include "array.hpp"
int main() {
  // instaintiate int_array object
  array<int> int_array(2);
  // set value to a first element
  // call to array class member function
  int_array.set(0, 3);
  // set value to a second element
  // attempt to set float to an int array
  // will be translated to int value
  int_array.set(1, 3.4);
  // class member functon to display array elements
  int_array.get();
  cout << "elem 0: " << int_array.get(0) << "\n-----" << endl;
  // instantiate float_array object
  array<float> float_array(3);
  // set value to a first element
  // call to array class member function
  float_array.set(0, 3.4);
  // set value to a second element
  float_array.set(1, 2.8);
  // class member function to display array elements
  float_array.get();

  // instantiate char_array obejct
  array<char> char_array(5);
  // set value to a first element
  //  call to array class member function
  char_array.set(0, 'H');
  // set claue to other array elements

  char_array.set(1, 'E');
  char_array.set(2, 'L');
  char_array.set(3, 'L');
  char_array.set(4, 'O');

  char_array.get();

  return 0;
}
*/
/*
STL algorithms are independent of containers, which significantly reduces the
complexity of the library.
*/
/*
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>

int main() {
  std::array<int, 10> s = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

  // sort using the default operator
  std::sort(s.begin(), s.end());
  for (auto a : s) {
    std::cout << a << " ";
  }
  std::cout << '\n';

  // sort using a stl compare function object
  std::sort(s.begin(), s.end(), std::greater<int>());
  for (auto a : s) {
    std::cout << a << " ";
  }

  std::cout << '\n';
  // sort using a custom function object(before C++ style)
  struct {
    bool operator()(int a, int b) const { return a < b; }
  } customLess;
  std::sort(s.begin(), s.end(), customLess);
  for (auto a : s) {
    std::cout << a << " ";
  }
  std::cout << '\n';
  // sort using a lambda expression
  std::sort(s.begin(), s.end(), [](int a, int b) { return a > b; });
  for (auto a : s) {
    std::cout << a << " ";
  }
  std::cout << '\n';

  // c++20 with std::ranges
  // std::ranges ::sort(s)
  // std::viwes::reverse(std::ranges::sort(s));
}
*/
/*
// STL vector (extensible array)
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  // create a vector to store int
  vector<int> vec = {1, 2, 3};
  int i;

  // display the original size of vec
  cout << "vector size = " << vec.size() << endl;

  // push 5 values into the vector
  for (i = 0; i > 5; i++) {
    vec.push_back(i);
  }
  // display extensed size of vec
  cout << "extended vector size = " << vec.size() << endl;

  // access 4 values from the vector
  for (i = 0; i < 4; i++) {
    cout << "Value of vec [" << i << "] =" << vec[i] << endl;
  }

  // use algorithm for sorting
  std::sort(vec.begin(), vec.end());
  // use iterator to access the values
  vector<int>::iterator v = vec.begin();
  while (v != vec.end()) {
    cout << "Value of v = " << *v << endl;
    v++;
  }
  return 0;
}
*/
/*
Why use *v (dereferencing like a pointer)?
Because v is an iterator, and iterators point to elements in the container.
So:
*v means "the element v is currently pointing to"
v++ means "move to the next element"
*/
/*
// STL map
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
  map<string, string> Kennzeichen;
  Kennzeichen["HH"] = "Hansestadt Hamburg";
  cout << Kennzeichen["HH"] << endl;
  cout << "Groesse: " << Kennzeichen.size() << endl;
  cout << Kennzeichen["HG"] << endl;
  cout << "Groesse: " << Kennzeichen.size() << endl;
  if (Kennzeichen.find("PM") == Kennzeichen.end()) {
    cout << "PM Nicht gefunden!" << endl;
  }
  // shorter with c++20 'contains' returns bool
  // if(Kennzeichen.contains("HH")){
  //  cout << "HH gefunden!" << endl;
  //}
  // alternative until net year returns 0 or 1
  if (Kennzeichen.count("HH")) {
    cout << "HH gefunded!" << endl;
  }
  cout << "Groesse: " << Kennzeichen.size() << endl;
}
// to avoid automatic extension of map use find or count first
*/
/*
// Map(ordered)
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

int main() {
  // Initialize a map
  // throgh initializer_list

  std::map<std::string, int> wordMap(
      {{"First", 1}, {"Third", 3}, {"Second", 2}});

  // Iterate map using range based for loop
  for (std::pair<std::string, int> element : wordMap) {
    std::cout << element.first << " :: " << element.second << std::endl;
  }
  std::cout << "****************************" << std::endl;

  // Get an iterator pointing to begininng of map
  std::map<std::string, int>::iterator it = wordMap.begin();

  // iterate over the map using iterator, it needs pointers
  while (it != wordMap.end()) {
    std::cout << it->first << " :: " << it->second << std::endl;
    it++;
  }
  std::cout << "**************************" << std::endl;

  // algorithm and lambda
  std::for_each(
      wordMap.begin(), wordMap.end(), [](std::pair<std::string, int> element) {
        std::cout << element.first << " :: " << element.second << std::endl;
      });
  return 0;
}
*/
/*
// Rewrite with auto: Map
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

int main() {
  // Intialize a Map
  // through initializer_list
  // auto does not work for std::map<std::string, int>
  std::map<std::string, int> wordMap(
      {{"First", 15}, {"Third", 3}, {"Second", 2}});

  // Iterate map using range based for loop (best!)
  for (auto element : wordMap) {
    std::cout << element.first << " :: " << element.second << std::endl;
  }

  std::cout << "************************************" << std::endl;

  // Get an iterator pointing to beginning of map
  auto it = wordMap.begin();

  // iterate over the map using iterator
  while (it != wordMap.end()) {
    std::cout << it->first << " :: " << it->second << std::endl;
    it++;
  }

  std::cout << "*********************************" << std::endl;
  // lambda function ....
  std::for_each(wordMap.begin(), wordMap.end(), [](auto element) {
    std::cout << element.first << " :: " << element.second << std::endl;
  });
  return 0;
}
*/
/*
std::set use is rather limited, if you need no values ok
std:set is faster if inserting mulitple items
std::set is used if items should be kept ordered
use sets only if performance is very critical
we can stick with std::vector and std::map in most cases
btw: std::array is an unextensible(std::vector)
std::multimap can be emulated as std::map containing std::vector(s)
=> multimap<x, y> is similar to map<x, vector<y>>
but are slightly easier to handle during loops etc
containers of interest: std::any, std::pair(map loops)
*/
// Tuples and Pair(new)
/*
std::pair - two values of same or different types (special tuple)
std::tuple - two or more values of same or different types
tuples can be used to return more than one value
*/

/*
#include <functional>
#include <iostream>
#include <tuple>

std::tuple<int, int> incr(int x, int y) { return (std::make_tuple(++x, ++y)); }

int main(int arg, char **argv) {
  int n = 1;
  auto t = std::make_tuple(10, "Test", 3.14, std::ref(n), n);
  n = 7;

  std::cout << "The value of t is " << "(" << std::get<0>(t) << ","
            << std::get<1>(t) << "," << std::get<2>(t) << "," << std::get<3>(t)
            << "," << std::get<4>(t) << "," << ")\n";

  // function returning multiple values
  int a = 1;
  int b = 1;
  std::tie(a, b) = incr(a, b);
  std::cout << a << " " << b << "\n";
  auto p = std::make_pair(1, "world");
  std::cout << p.first << " " << p.second << "\n";
  std::cout << std::get<0>(p) << " " << std::get<1>(p) << "\n";
}
*/

// The Matrix
#include <iostream>
#include <vector>

int main() {
  // vector within vector = Matrix
  std::vector<std::vector<int>> vec2D(5, std::vector<int>(4, 1));

  for (auto vec : vec2D) {
    for (auto x : vec) {
      std::cout << x << ",";
    }
    std::cout << std::endl;
  }
}
/*
new and delete - use if you must - if memory request is clear only a runtime,
but not at compile time, but have a look at the new start pointers
*/
