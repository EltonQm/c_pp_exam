#include <stdio.h>

int main(){

    int i;

    for(i=0; i <= 10; i=i+1)
    {
        printf("%d\n", i);
    }

    // Single Statement
    int it = 0;
    while(it < 10)
        printf("i = %d\n", it++);
    
    // Block of statement
    int j = 0;
    while (j < 10) {
        printf("j = %d", ++j);
        printf("\n");
    } 

    if (4 > 2 == 3 > 1) // Gives warning, but works
    {
        printf("Both are true \n");
        printf("4 > 2 gives %d \n", 4 > 2);
        printf("3 > 1 gives %d \n", 4 > 2);
        printf("4 > 2 == 3 > 1 gives %d. \n", 4>2 == 3>1);
    }

    int c;
    c = getchar();
    while (c != EOF) {
        putchar(c);
        c = getchar();
    }

    while ((c=getchar()) != EOF) {
        putchar(c);
    }
    return (0);
    /*
    int ii = 0, jj = 0;
    while(ii<10 && jj < 10){
        i = ii + 1;
        j = jj + 2;

        printf("i=%d/t j = %d\n", i, j);
    }
    return (0);

    // do-while loop
    int i = 0;
    do {
        printf("i = %d\n", ++i)
    } while (i < 10);

    int j = 0;
    do{
        printf("j=%d\n", j++);
    } while(j < 10);

    return (0)
    */

}


/*
The getchar function does not take any parameters. It is invoked without arguments.
The getchar function returns the next character from the standard input as an unsigned 
char cast to an int. If the end-of-file (EOF) is reached or if an error occurs, getchar 
returns EOF. The EOF is typically defined as -1 in the standard library.

The C library putchar function is a part of the standard C library and is used to
write a single character to the standard output, which is typically the console or 
terminal. 

On success, putchar returns the character written as an unsigned char cast to an int. 
If an error occurs, it returns EOF (End Of File)
*/