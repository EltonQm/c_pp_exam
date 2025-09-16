// An array Template class
// file array.hpp

#ifndef ARRAY_H_
#define ARRAY_H_

#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;

#include <typeinfo>
// define a class array of type T
// the type is not known yet and will be define by instantiation
// of object if class array<T> from main

template <typename T> class array {
private:
  int asize;
  T *myarray; // we need a pointer here as we don't know
              // how many elements we will need

public:
  // constructor with user pre-defined size
  array(int s) {
    asize = s;
    myarray = new T[asize];
  }

  // class array member function to set
  // element of myarray with type T values
  void set(int elem, T val) { myarray[elem] = val; }
  int get(int elem) { return (myarray[elem]); }
  
  // for loop to display all elements of an array
  void get() {
    for (int j = 0; j < asize; j++) {
      // typeid will retreive a type for each value
      cout << setw(7) << j << setw(13) << myarray[j]
           << " type: " << typeid(myarray[j]).name() << endl;
    }
    cout << "-------------------------------" << endl;
  }
  
  int size() { return (asize); }

};
#endif
