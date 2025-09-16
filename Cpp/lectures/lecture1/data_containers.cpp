#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <set>
#include <map>

using namespace std;

int main()
{

    std::string hw = "Hello World!\n";

    std::cout << hw;

    std::cout << "Length of String: " << hw.length() << std::endl;

    std::vector<int> v = {7, 5, 16, 8};

    std::cout << "element at 0: " << v.at(0) << std::endl;

    v[0]++;

    std::cout << "element at 0: " << v[0] << std::endl;

    std::cout << "length: of vector: " << v.size() << std::endl;

    // add new element (not available for array)

    v.push_back(20);

    std::cout << "last element: " << v.at(v.size() - 1) << std::endl;

    std::map<std::string, char> aaa = {{"Leu", 'L'}};

    // Inserting data in std::map

    aaa.insert(std::make_pair("Cys", 'C')); // make_pair

    aaa.insert(std::make_pair("Asp", 'D'));

    // aaa.insert(std::pair<std::string, char>("Cys", 'C'));  // Using std::pair
    // aaa.insert(std::pair<std::string, char>("Asp", 'D'));  // Using std::pair

    // automatic pair creation

    aaa.insert({"Ala", 'A'});

    // even more reduced insert

    aaa["Asn"] = 'N';

    aaa["His"] = 'H';

    std::cout << "map size: " << aaa.size() << std::endl;

    std::cout << "Proline? " << aaa.count("Pro") << std::endl;

    aaa["Pro"] = 'P';

    std::cout << "Proline? " << aaa.count("Pro") << std::endl;

    return 0;
}

/*
std::string
• std::map (like dictionary (Python), list R)
• std::array (not extensible)
• std::vector (like array but can be extended)
• std::set (like map, but no values)

Example some basic methods for a map:
• begin() - Returns an iterator to the first element in the map
• end() - Returns an iterator to the theoretical element that follows last element in the map
• size() - Returns the number of elements in the map
• empty() - Returns whether the map is empty
• insert(keyvalue, mapvalue) - Adds a new element to the map
• erase(const g)- Removes the key value “g” from the map
• clear() - Removes all the elements from the map
*/

/*
std::pair is a class template that provides a way to store two heterogeneous objects
in a single unit
std::make_pair-creates a pair object of type, defined by the argument type.

The difference is that with std::pair you need to specify the types of both elements,
whereas std::make_pair will create a pair with the type of the elements that are
passed to it, without you needing to tell it.
*/