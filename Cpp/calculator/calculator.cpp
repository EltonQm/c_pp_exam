#include <iostream>
using namespace std;

// Function to generate nth Fibonacci number
int fibonacci(int n)
{
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Function to run a Fibonacci exercise
int runFibonacci(int tries)
{
    int correct = 0;
    for (int i = 0; i < tries; i++)
    {
        int n = rand() % 10 + 1; // Random number from 1 to 10
        int answer;
        cout << "What is Fibonacci(" << n << ")? ";
        cin >> answer;
        if (answer == fibonacci(n))
        {
            cout << "Correct!\n";
            correct++;
        }
        else
        {
            cout << "Wrong. The correct answer is " << fibonacci(n) << endl;
        }
    }
    return correct;
}

// Function to run a square exercise
int runSquare(int tries)
{
    int correct = 0;
    for (int i = 0; i < tries; i++)
    {
        int n = rand() % 20 + 1; // Random number from 1 to 20
        int answer;
        cout << "What is " << n << "^2? ";
        cin >> answer;
        if (answer == n * n)
        {
            cout << "Correct!\n";
            correct++;
        }
        else
        {
            cout << "Wrong. The correct answer is " << n * n << endl;
        }
    }
    return correct;
}

int main()
{
    char choice;
    int tries = 10; // Default number of tries

    cout << "=== Welcome to the Exercise Calculator ===" << endl;

    while (true)
    {
        cout << "\nChoose an option:\n";
        cout << "(f) Fibonacci practice\n";
        cout << "(r) Square practice\n";
        cout << "(o) Options (set number of tries)\n";
        cout << "(q) Quit\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice == 'q')
        {
            cout << "Goodbye!\n";
            break;
        }
        else if (choice == 'o')
        {
            cout << "Enter number of tries you want per round: ";
            cin >> tries;
            if (tries <= 0)
            {
                cout << "Invalid number. Keeping previous value: " << tries << endl;
            }
            else
            {
                cout << "Number of tries set to " << tries << endl;
            }
        }
        else if (choice == 'f')
        {
            cout << "\n--- Fibonacci Exercise ---\n";
            int score = runFibonacci(tries);
            cout << "You got " << score << " out of " << tries << " correct.\n";
        }
        else if (choice == 'r')
        {
            cout << "\n--- Square Exercise ---\n";
            int score = runSquare(tries);
            cout << "You got " << score << " out of " << tries << " correct.\n";
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
