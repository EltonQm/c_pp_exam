#include <stdio.h>

int calculateStaticrun(int n)
{
    
    static int num = 0;
    
    num+=n;
    
    return num;
}
int main()
{
    
    printf("1: %d\n", calculateStaticrun(10));
    printf("1: %d\n", calculateStaticrun(10));
    printf("1: %d\n", calculateStaticrun(10));
    printf("1: %d\n", calculateStaticrun(10));
    printf("1: %d\n", calculateStaticrun(10));
    
    int finalSUm = calculateStaticrun(3);
    printf("%d\n", finalSUm);
    return finalSUm;
}
