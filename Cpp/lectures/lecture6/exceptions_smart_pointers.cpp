#include <exception> // For std::exception base class
#include <iostream>  // For standard input/output

using namespace std;

// Define a custom exception class that inherits from std::exception
struct MyException : public exception {

  // Override the what() method from std::exception
  const char *what() const throw() {
    return "C++ Exception"; // Custom error message
  }
};

int main() {
  try {
    // Explicitly throw an instance of MyException
    throw MyException();
  } catch (MyException &e) {
    // This block handles exceptions of type MyException
    std::cout << "MyException caught" << std::endl;
    std::cout << e.what() << std::endl; // Call overridden what() method
  } catch (std::exception &e) {
    // Fallback: handles other standard exceptions
  }
}

/* You can define your own exception by inheriting and overriding exception
class functionality.
*/

/*
struct MyException : public exception
This means MyException inherits from the standard base class std::exception.

This makes MyException a polymorphic exception type, meaning it can be caught as
either:

MyException, or its base class: std::exception.

Why inherit?
It allows your custom exception to behave like standard exceptions and integrate
with existing exception handling mechanisms.
*/

/*
const char* what() const throw() {
    return "C++ Exception";
}
The method what() is inherited from std::exception:

virtual const char* what() const noexcept;
In MyException, you override this method to return your own custom error
message.

Overriding means providing a new implementation of a virtual function that
exists in a base class.
*/