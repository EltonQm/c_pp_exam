#include <iostream>
int main()
{
    enum
    {
        kreuz,
        pik,
        karo,
        herz,
        dummy = 7
    };
    int karte = karo;
    std::cout << "Wert von Pik ist: " << pik << "\n";
    std::cout << "Wert der Karte: " << karte << "\n";
    karte = dummy;
    std::cout << "Wert der Karte: " << karte << "\n";
    return (0);
}

/*

In C, an enum (short for enumeration) is a user-defined data type
that consists of a set of named integer constants.
It allows you to assign names to integral values, making code more
readable and easier to maintain. By default, the first name in an
enum has a value of 0, and each subsequent name's value is
incremented by 1. You can explicitly assign values to enum members

*/