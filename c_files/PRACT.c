#include <stdio.h> //The program first includes the standard input/output library (<stdio.h>).
#include <limits.h>

int main() //The main() function is declared, which returns an integer value.
{
    printf("Size in bytes: %d\n", sizeof (char));
    printf("CHAR_MIN: %d\n", CHAR_MIN);
    printf("SIze in bytes: %d\n", sizeof (unsigned char));
    printf("UCHAR_MAX: %d\n", UCHAR_MAX);
    

    int i = 0;
    while(i < 10) //starts from 0-9
        printf("i=%d\n", i++);

    int j = 0;
    while (j<10) { //1-10
        printf("j=%d\n", ++j);
    }    

// Type casting in the C language is a way to convert a variable from one data type to another.
// It allows you to temporarily change the type of a variable to another type to perform a specific
// operation or assign it to another variable of a different type.

// For example, let's say you have an integer variable called "a" and a floating-point variable called "b",
//  and you want to assign the value of "b" to "a". Since "a" is an integer and "b" is a floating-point number, 
//  you cannot directly assign the value of "b" to "a". In this case, you can use type casting to convert "b" to an integer before assigning it to "a":

// int a;
// float b = 3.14;
// a = (int)b; // type cast b to an integer before assigning it to a
// The syntax for type casting in C is to place the desired data type in parentheses before the variable to be converted. 
// For example, the expression (int)b will convert the floating-point variable "b" to an integer.

// It's important to note that type casting can sometimes result in data loss or unexpected behavior, 
// especially when converting between types of different sizes or precision. 
// Therefore, it's important to use type casting carefully and only when necessary.

// The precedence of these operators determines the order in which they are evaluated in an expression. 
// The relational operators have higher precedence than the assignment operators but lower precedence than arithmetic operators.

// Unary precedes binary operators, so the ++ operator is evaluated before the < operator.

int c; //An integer variable c is declared.

c= getchar();
//The getchar() function is called to read a single character from the standard input and store it in the variable c.

// A while loop is used to repeat the following steps as long as c does not equal EOF:
while (c != EOF) { //end of file (EOF)

//The character stored in c is printed to the standard output using the putchar() function.
    putchar(c);

//Another character is read from the standard input and stored in c using getchar().
    c = getchar();

//When the end of file is reached, the while loop terminates and the program returns 0.

    //Note that getchar() and putchar() are used instead of scanf() and printf() 
    //because they read and write single characters, respectively, which is more appropriate for this program
}

    return(0);

}