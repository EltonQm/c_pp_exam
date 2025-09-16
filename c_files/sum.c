#include <stdio.h>

int main()
{
    int a, b, c, sum;

    while (1) {
        printf("Enter three integers: ");
        scanf("%d %d %d", &a, &b, &c);

        if (a == 0 || b == 0 || c == 0) {
            printf("End of summation\n");
            break;
        }

        sum = a + b + c;
        printf("The sum of the numbers is %d\n", sum);
    }

    return 0;
}
