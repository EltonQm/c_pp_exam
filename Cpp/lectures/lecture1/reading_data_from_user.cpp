#include <iostream>
using namespace std;

int main()
{
    char name[50]; // no auto here !!
    int age;       // no auto here!!
    cout << "Please enter your name: ";
    cin >> name;
    cout << "Please enter your age: ";
    cin >> age;
    cout << "Your name is: " << name << ". Your age is: " << age << "." << endl;
}