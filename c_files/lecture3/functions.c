// #include <stdio.h>

/*
// integer overflow
int main() {

    char a = 2, b = 7, c;

    c = a + b;

    printf("c = %d + %d = %d\n", a ,b, c);

    c = a  + 255;
    printf("c = %d + 255 = %d\n", a ,c);

    c = a + 127;
    printf("c = %d + 127 = %d\n", a, c);

}
*/

// structure of a function
// function prototype
// main function
// function definition

// Modular programming
// Break down a problem in multiple smaller parts.
// Modular independence, contains all the code it needs.
// Easier to maintain and to identify problems/errors.
// Reusable

// Local variables
// Accessible within the function only.
// Shorter lifetime (storage allocation and delloaction)
/*
#include <stdio.h>

void print_xy(void);

int main(){
    int x = 23, y = 51;
    print_xy();
    return(0)
}

void print_xy(void){
    int x = 1, y = 2;
    printf("Value of x is %d, value of y is %d.\n", x, y);
}
*/

// Global variable.
// Accessible for the whole program, below definition
// Initialized to 0 if no value is  given

/*
include <stdio.h>

void print_xy(void);
int x =23, y = 51;

int main(){
    print_xy();
    return(0);
}

void print_xy(void){
    printf("Value of x is %d, value of y is %d.\n", x, y)
}
*/

/*
void print_xy(void);
int x = 23, y = 51;

int main()
{
    print_xy();
    return(0);
}
void print_xy(void)
{
    x = 5;
    printf("Value of x is %d, value of y is %d.\n", x, y);
}
*/

/*
void print_xy(void);
int x = 23, y = 51;

int main()
{
    print_xy();
    return(0);
}
void print_xy(void)
{
    long x = 5;
    float y = 1.7;
    printf("Value of x is %ld, value of y is %f.\n", x, y);
}
*/
/*
#include <stdio.h>

void print_integer(void);
void print_integerneg(void);

int main()
{
    print_integer();
    print_integer();
    print_integer();
    print_integerneg();
    print_integerneg();
    print_integerneg();
    return(0);
}

void print_integer(void)
{

    static int x = 1;
    printf("x=%d\n", x++);

}

void print_integerneg(void)
{

    static int x = 1;
    printf("x=%d\n", --x);
}
*/

/*
// Modular programming - multiple source files
// Main module (prog.c)
#include <stdio.h>
#include "funcs.h"

int main()
{
    int x_cube;
    float x_half;
    int x = 3;


    x_cube = cube(x);
    x_half = half(x);
    printf("x=%d, x_cube=%d, x_half=%f\n", x, x_cube, x_half);
    return(0);
}
*/

/*
// Global variable scope with multiple source files
#include <stdio.h>
#include "funcs.h"

static int x = 23, y = 51;

int main()
{
    extern int x, y; // good practice
    int x_cube = cube(x);
    printf("x=%d, x_cube=%d\n", x, x_cube);
    return(0);
}
// variable linkage and visibility across multiple source files.
// static for Global Variables
// You're declaring global variables with internal linkage, meaning:

// They are visible only within this .c file

// They cannot be accessed from other files — they're private to this
translation unit

// Think of static as saying: “this global variable is local to this file.”

// extern for Global Variables
// You're declaring global variables that exist somewhere else, typically in
another .c file.

// This line doesn't allocate memory; it just tells the compiler:

// “Hey, these variables exist — trust me — you'll find them elsewhere during
linking.”

// BUT... in your case, x and y were declared as static, so they aren’t visible
outside functions.c.

// So this extern line is kind of misleading and unnecessary here — it only
makes sense if x and y were defined in another file without static.
*/

// Recursion is a function that calls itself
#include <stdio.h>

unsigned int factorial(unsigned int i);

int main() {
  unsigned int f;
  unsigned int x = 8;
  f = factorial(x);
  printf("Factorial of %d is %d\n", x, f);
  return (0);
}

unsigned int factorial(unsigned int i) {
  if (i == 1)
    return (1);
  else
    i *= factorial(i - 1);
  return (i);
}

/*
// printf formatting
specifier        Description
%c               single character
%d               signed integer to decimal
%ld              signed long integer to decimal
%u               unsigned integer to decimal
%f               floating-point number to decimal
%e               floating point number to decimal exponent

// scanf formatting
specifier        Description
%c               character or sequence of character
%s               string(sequence of non-whitespace character)
%d               decimal integer
%u               unsigned decimal integer
%e, %f           floating-point number

*/

/*
Global variables are accessible within the source code file where they are
declared. They are accessible within the whole program if declared and preceeded
by extern in other source code files.

Static global variables are only accessible within the source code file they are
declared
*/