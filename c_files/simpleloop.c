#include <stdio.h>

/*
int i;

int main() {

    for (i=0; i<10; i++)
    {
        printf("%d\n", i);
    }
    return(0);
}
*/

int print_int(int i)
{
    printf("%d\n", i);
    return 0;
}

int main(){
    print_int(2);
    return 0;
}