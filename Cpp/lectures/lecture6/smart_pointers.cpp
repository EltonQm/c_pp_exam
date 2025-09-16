 /*
// Pointers on the heap
// With a modern cpp complier there is no reason anymore to use:
Dog *fido = new Dog();
delete fido;

// Instead you can use:
#include <memory>
// complete - but see auto
std::unique_ptr<Dog> fido = std::make_unique<Dog>("Fido");
// shorter - snippet: mku=std::make_unique<%cursor%>();
auto elsa = std::make_unique<Dog>("Elsa");
*/
/*
Because shared_ptr already has an analogous std::make_shared, for consistency
we wil call this one make_unique.
*/

/*
std::unique_ptr<int> p1(new int(5));
std::unique_ptr<int> p2 = p1;            // complie error
std::unique_ptr<int> p3 = std::move(p1); // Transfer ownership.p3

p3.reset(); // Deletes memory
p1.reset(); // Does nothing
*/

/*
#include <iostream> // For input/output
#include <memory>   // For smart pointers (std::unique_ptr)
#include <string>   // For std::string

using namespace std;

// Define a simple Dog class
class Dog {
public:
  // Constructor takes a name and stores it
  Dog(std::string name) { itsName = name; }

  // Destructor announces when a Dog object is destroyed
  ~Dog() { cout << itsName << " is destroyed!\n"; }

  // bark() prints a simple message
  void bark() { cout << itsName << " says wuff!\n"; }

private:
  std::string itsName = ""; // Name of the dog
};

int main() {
  // ---------- C++98 style (manual memory management) ----------

  // Allocate a Dog object on the heap using new
  Dog *fido = new Dog("Fido I.");
  fido->bark(); // Call method
  delete fido;  // Manually free memory (good practice)

  // Another Dog object allocated with new
  Dog *fido2 = new Dog("Fido II.");
  fido2->bark();
  // ❗ Memory leak: fido2 is never deleted, so destructor won't be called

  // ---------- Modern C++11/14 style using smart pointers ----------

  // Create a unique_ptr to a Dog object (memory managed automatically)
  auto elsa = make_unique<Dog>("Elsa"); // Best practice in modern C++
  elsa->bark();                         // Use like a regular pointer

  // No need to delete `elsa`
  // ❱ When `elsa` goes out of scope, its destructor is called,
  // ❱ and the Dog object is deleted automatically

  return 0; // Elsa will be destroyed here
}
*/
/*
Concept             Explanation
new and delete	    Manual memory allocation and deallocation (C++98 style)
delete missing	    Causes a memory leak (e.g., fido2)
std::unique_ptr	    Smart pointer that owns and deletes the object automatically
make_unique<T>()	Safest and cleanest way to create unique pointers in
C++14+
*/
/*
#include <iostream> // For std::cout
#include <memory>   // For std::shared_ptr
#include <string>   // For std::string

using namespace std;

// Dog class definition
class Dog {
public:
  Dog(const std::string &name) : itsName(name) {
    cout << itsName << " is created.\n";
  }

  ~Dog() { cout << itsName << " is destroyed!\n"; }

  void bark() { cout << itsName << " says wuff!\n"; }

private:
  std::string itsName;
};

int main() {
  // ---------- Using shared_ptr (C++11 and later) ----------

  // Create a shared_ptr to manage a Dog object
  shared_ptr<Dog> bella = make_shared<Dog>("Bella");
  bella->bark(); // Call a method

  {
    // Create another shared_ptr sharing ownership of the same Dog
    shared_ptr<Dog> bellaAlias = bella;
    bellaAlias->bark();

    // At this point, bella and bellaAlias both share the same Dog object
    cout << "Use count (shared ownership): " << bella.use_count() << '\n';
  }

  // bellaAlias goes out of scope here, but bella still owns the Dog
  cout << "Use count after alias is gone: " << bella.use_count() << '\n';

  // Once bella goes out of scope (end of main), Dog is automatically destroyed

  return 0;
}
*/
/*
#include <memory>
auto uPtr = std::make_unique<int>(100);
// set a value as usually
uPtr[0] = 1;
*/
/*
The uPtr will have automatic storage duration and
will call the destructor when it goes out of scope.The int will have
dynamic storage duration(heap) and will be deleted by the smart pointer.
*/
/*
#include <array>    // For std::array
#include <iostream> // For std::cout and std::endl
#include <memory>   // For std::unique_ptr and std::shared_ptr
#include <string>   // For std::string

int main() {
  // Create a unique_ptr to a std::string
  auto pUP = std::make_unique<std::string>("Hello, world!");

  // Create a unique_ptr to a dynamically allocated array of 5 integers
  std::unique_ptr<int[]> ptr1{new int[5]{1, 2, 3, 4, 5}};

  // Create a unique_ptr to a std::array<int, 5> using make_unique
  auto ptr2 =
      std::make_unique<std::array<int, 5>>(std::array<int, 5>{1, 2, 3, 4, 5});

  // Create a shared_ptr to a std::string
  auto pSP1 = std::make_shared<std::string>("Hello, world!");

  // Create another shared_ptr that shares ownership with pSP1
  std::shared_ptr<std::string> pSP2 = pSP1;

  // Print the string pointed to by the unique_ptr
  std::cout << *pUP << std::endl;

  // Optionally print contents of ptr1 (raw array managed by unique_ptr)
  std::cout << "ptr1 contents: ";
  for (int i = 0; i < 5; ++i) {
    std::cout << ptr1[i] << " ";
  }
  std::cout << std::endl;

  // Print contents of ptr2 (std::array inside a unique_ptr)
  std::cout << "ptr2 contents: ";
  for (const auto &val : *ptr2) {
    std::cout << val << " ";
  }
  std::cout << std::endl;

  // Print the use count of the shared_ptr
  std::cout << "Shared pointer use count: " << pSP1.use_count() << std::endl;

  return 0;
}
*/
