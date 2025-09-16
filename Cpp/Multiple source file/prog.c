#include <stdio.h>
#include "func.h"

int main()
{
    int x_cube;
    float x_halfv;
    int x=3;

    x_cube = cube(x);
    x_halfv =halfv(x);

    printf("x=%d, x_cube=%d, x_halfv=%f\n", x, x_cube, x_halfv);
    return(0);
}


