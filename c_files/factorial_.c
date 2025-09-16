#include <stdio.h>

// Function to calculate factorial
long long factorial(int n) {
    if (n == 0 || n == 1)
        return 1;
    else
        return n * factorial(n - 1);
}

int main() {
    int number;

    // Ask the user for input
    printf("Enter a number: ");
    scanf("%d", &number);

    // Check for negative numbers
    if (number < 0)
        printf("Factorial of a negative number doesn't exist.\n");
    else {
        // Call the factorial function
        printf("Factorial of %d is %lld\n", number, factorial(number));
    }

    return 0;
}
