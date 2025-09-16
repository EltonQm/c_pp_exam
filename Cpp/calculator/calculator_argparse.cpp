#include <argparse/argparse.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>

using namespace std;

// === Function to generate nth Fibonacci number ===
int fibonacci(int n)
{
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// === Function to run a Fibonacci exercise ===
int runFibonacci(int tries)
{
    int correct = 0;
    for (int i = 0; i < tries; i++)
    {
        int n = rand() % 10 + 1;
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

// === Function to run a square exercise ===
int runSquare(int tries)
{
    int correct = 0;
    for (int i = 0; i < tries; i++)
    {
        int n = rand() % 20 + 1;
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

// === Function to run amino acid quiz ===
int runAminoQuiz(int tries)
{
    map<char, string> aminoMap = {
        {'A', "Ala"}, {'R', "Arg"}, {'N', "Asn"}, {'D', "Asp"}, {'C', "Cys"}, {'E', "Glu"}, {'Q', "Gln"}, {'G', "Gly"}, {'H', "His"}, {'I', "Ile"}, {'L', "Leu"}, {'K', "Lys"}, {'M', "Met"}, {'F', "Phe"}, {'P', "Pro"}, {'S', "Ser"}, {'T', "Thr"}, {'W', "Trp"}, {'Y', "Tyr"}, {'V', "Val"}};

    vector<char> toGuess;
    for (const auto &pair : aminoMap)
        toGuess.push_back(pair.first);

    int totalTries = 0;
    // const int maxTries = 30;
    int correct = 0;

    static std::random_device rd;
    static std::mt19937 g(rd());

    while (!toGuess.empty() && totalTries < tries)
    {
        std::shuffle(toGuess.begin(), toGuess.end(), g);
        char code = toGuess.back();
        string answer;
        cout << "What is the 3-letter code for '" << code << "'? ";
        cin >> answer;

        // Normalize user input to capitalize first letter only
        if (!answer.empty())
        {
            answer[0] = toupper(answer[0]);
            for (size_t i = 1; i < answer.size(); ++i)
                answer[i] = tolower(answer[i]);
        }

        if (answer == aminoMap[code])
        {
            cout << "Correct!\n";
            toGuess.pop_back();
            correct++;
        }
        else
        {
            cout << "Wrong! The correct answer is: " << aminoMap[code] << endl;
            // code remains in toGuess
        }

        totalTries++;
        cout << "Progress: " << (tries - toGuess.size()) << "/" << tries << " correct. Tries: "
             << totalTries << "/" << tries << "\n\n";
    }

    if (toGuess.empty())
        cout << "🎉 You correctly guessed all amino acids!\n";
    else
        cout << "❌ You ran out of tries. Better luck next time!\n";

    return correct;
}

// === Menu function ===
void menu(int &tries)
{
    char choice;
    while (true)
    {
        cout << "\nChoose an option:\n";
        cout << "(f) Fibonacci practice\n";
        cout << "(r) Square practice\n";
        cout << "(a) Amino acid quiz\n"; // << NEW Option printed
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
        else if (choice == 'a')
        {
            cout << "\n--- Amino Acid Quiz ---\n";
            int score = runAminoQuiz(tries);
            cout << "You got " << score << " out of 20 correct.\n"; // always 20 amino acids
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

// === Main function ===
int main(int argc, char *argv[])
{
    srand(time(nullptr)); // seed random
    int tries = 10;
    char mode = 'x';

    // Setup argparse
    argparse::ArgumentParser program("calculator");

    program.add_argument("-m", "--mode")
        .help("Start mode: f (fibonacci), r (square), a (amino quiz), x (menu only)")
        .default_value(std::string("x"))
        .nargs(1);

    program.add_argument("-t", "--tries")
        .help("Number of tries per round")
        .default_value(10)
        .scan<'i', int>();

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::exception &err)
    {
        std::cerr << err.what() << endl;
        return 1;
    }

    // Read arguments
    string modeStr = program.get<string>("--mode");
    mode = tolower(modeStr[0]);
    tries = program.get<int>("--tries");

    // Handle initial mode run if valid
    if (mode == 'f')
    {
        cout << "--- Starting Fibonacci Exercise ---\n";
        int score = runFibonacci(tries);
        cout << "You got " << score << " out of " << tries << " correct.\n";
    }
    else if (mode == 'r')
    {
        cout << "--- Starting Square Exercise ---\n";
        int score = runSquare(tries);
        cout << "You got " << score << " out of " << tries << " correct.\n";
    }
    else if (mode == 'a')
    {
        cout << "--- Starting Amino Acid Quiz ---\n";
        int score = runAminoQuiz(tries);
        cout << "You got " << score << " out of " << tries << " correct.\n";
    }
    else if (mode != 'x')
    {
        cout << "Invalid mode entered. Entering menu instead.\n";
    }

    // Enter interactive menu mode
    cout << "\n=== Welcome to the Exercise Calculator ===\n";
    menu(tries);
    return 0;
}

/*
int argc
- stands for argument count.
- It tells you how many arguments were passed to the program including the program name itself.

e.g
./my_program foo bar
argc = 3

char *argv[] means the same things as char **argv
- stands for argument vector.
- It's an array of C-strings(char *), each one being a command-line argument.

e.g
./program hello 123
* argv[0] -> "./program"
* argv[1] -> "hello"
* argv[2] -> "123"
*/

/*
int main(int argc, char *argv[])
{
    cout << "Argument count: " << argc << endl;
    for (int i = 0; i < argc; ++i)
    {
        cout << "Arg " << i << " : " << argv[i] << endl;
    }
    return 0
}
*/