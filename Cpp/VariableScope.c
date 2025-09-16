//Global and local variable

#include <stdio.h>
void print_xy(void);
int x=23, y=51;

int main()
{
    print_xy();
    return(0);
}

void print_xy(void)
{
    printf("Value of x is %d, value of y is %d.\n", x,y);
}

//Global variable

#include <stdio.h>
void print_xy(void);
int x=23, y=51;

int main()
{
    print_xy();
    return(0);
}

void print_xy(void)
{
    int x=5;
    printf("Value of x is %d, value of y is %d.\n", x,y);
}

//Global variable
#include <stdio.h>
void print_xy(void);
int x=23, y=51;

int main()
{
    print_xy();
    return(0);
}

void print_xy(void)
{
    long x = 5;
    float y = 1.7;
    printf("Value of x is %ld, value of y is %f.\n", x,y);
}

//Global Variable
#include <stdio.h> 

void print_xy(void);
    
int main()
{
 int x = 23, y = 51;    
 print_xy();
 return(0);
}
void print_xy(void)
{
 int x = 1, y = 2;
 printf("Value of x is %d, value of y is %d.\n", x, y);   
}

//Static vs Automatic local variables
//Automatic can be written with auto keyword

#include <stdio.h>
int main()
{
    auto int x=23;
    printf("Value of x is %d.\n",x);
    return(0);
}

#include <stdio.h>
void print_integer(void)
int main()
{
    print_integer();
    print_integer();
    print_integer();
    return(0);
}

void print_integer(void)
{
    static int x=1;
    printf("x=%d\n", x++)
}

#include <stdio.h>s
void print_integer(void)
int main()
{
    print_integer();
    print_integer();
    print_integer();
    return(0);
}

void print_integer(void)
{
    static int x=1;
    printf("x=%d\n", x++)
}
//x++ = 1,2,3; x-- = 1,0,-1; --x = 0,-1,-2