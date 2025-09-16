// #include <stdio.h>
// //#include <limit.h>

// int main() 
// {  
//     for (int i = 1; i <= 100; i++) {
//         printf("%d", i);
//     }

//     return 0;
// }  

#include <stdio.h>

int main() {
    for (int i = 1; i <= 100; i++) {
        printf("%d", i);

        if (i != 100) {
            
            //Write a program to print out numbers from 1 to 100 separating them by commas.
            printf(", ");
        }
    }

    return 0;
}