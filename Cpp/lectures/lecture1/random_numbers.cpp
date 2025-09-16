#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main()
{

    int i, j, j2;

    // set the seed

    srand((unsigned)time(NULL));

    /* generate 3 random numbers. */

    for (i = 0; i < 2; i++)
    {

        // generate actual random number

        j = rand();
        cout << " Random Number : " << j << endl;
        j2 = rand() % 100 + 1; // range 1 to 100

        cout << " Random Number 1-100: " << j2 << endl;
    }

    return 0;
}

/*
• we have a rand function but this is pseudo random
• need to call srand first
*/

/*
rand(): This function generates a random integer, typically between 0 and RAND_MAX
(a constant value that depends on the system, but is usually quite large).
rand() % 100: The modulo operator (%) gives the remainder when dividing the
random number by 100. This results in a number from 0 to 99
(i.e., a range of 100 possible values).
rand() % 100 + 1: Adding 1 shifts the range of values to be from 1 to 100 instead of
 0 to 99.

Explanation:
The rand() function generates a random number.
rand() % 100 limits this number to the range 0-99.
Adding 1 ensures that the final result is within the range 1-100.

*/