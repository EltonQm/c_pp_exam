#include <stdio.h>
unsigned int factorial(unsigned int i);
int main()
{
    unsigned int f;
    unsigned int x=8;
    f=factorial(x); 
    printf("Factorial of %d is %d\n", x , f);
    return(0);
}

unsigned int factorial (unsigned int i)
{
    if(i==1)
        return(1);
    else
    {
        i *= factorial(i-1);
        return(i);
    }    
}