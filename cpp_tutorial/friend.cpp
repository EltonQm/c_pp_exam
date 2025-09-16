
// Write a program in C++ to convert a decimal number into binary without using an array with a friend function.

#include <iostream>
using namespace std;

class T4Tutorials_Decimal_Number
{
protected:
	int i = 1, j, n, binaryn = 0;

public:
	T4Tutorials_Decimal_Number()
	{
		cout << "Enter Decimal Number: ";
		cin >> n;

		// Convert decimal to binary
		for (j = n; j > 0; j = j / 2)
		{
			binaryn = binaryn + (n % 2) * i;
			i = i * 10;
			n = n / 2;
		}
	}

	friend int show(T4Tutorials_Decimal_Number A);
};

int show(T4Tutorials_Decimal_Number A)
{
	cout << "Binary number = " << A.binaryn << endl;
	return 0;
}

int main()
{
	T4Tutorials_Decimal_Number A;
	show(A); // Call the show function to display the binary number
	return 0;
}
