#include <stdio.h>

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