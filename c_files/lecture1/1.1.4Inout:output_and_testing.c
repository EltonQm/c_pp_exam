#include <stdio.h>

int main()
{
    int a,res_;

    printf("Enter an integer ");
    scanf("%d", &a);

    res_= a*a;
    
    
    printf("The square of the input is %d\n", res_);
    
    int num,res;

    for(num=0; num<15; num++)
    {

        printf("Enter a number: ");
        scanf("%d", &num);

        res = num * num;

        printf("The square of the inputed number is %d\n", res);

        if (num > 0){
            printf("num is positive");
        }

        else if(num < 0) {
            printf("num is negative");
        }
        else {
            printf("num is zero");
        }
    } 

    return 0;
}



/*
In C, the int main() function is required because it defines the entry point of the 
program. When you run a C program, the operating system needs to know where to start 
executing the code, and main() is the default function where the execution begins.

Here's why it's structured this way:

Entry Point: The main() function is the standard entry point for a C program. 
The C runtime system calls the main() function when the program starts. 
It’s like telling the operating system, “Here’s where my program begins.”
Return Type: The int before main() signifies that the function will return an integer. 
This return value can communicate the success or failure of the program to the 
operating system. By convention:
return 0; indicates success.

Non-zero values typically indicate an error.
This is especially important in systems programming where the return code can be 
checked by other programs or the operating system itself.
Structured Language: C is a more structured and low-level language compared to Python. 
The explicit use of main() and the return type makes it clear how the program's execution
flows and gives the programmer control over how the program exits and communicates 
its status.
*/ 









