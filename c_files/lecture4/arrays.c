#include <stdio.h>

// Arrays are a collection of storage locations having the same data type.

/*
int main() 
{
    int a[10];
    a[0] = 1;

    printf("%d\n", a[0]);

    return(0);
}
*/

/*
int main()
{
    int points[31], count;

    for (count = 0; count < 31; count++)
    {
        printf("Enter smiley points for May %d:", count+1);
        scanf("%d", &points[count]);
    }
    printf("\n");

    for (count =  0; count < 31; count++)
        printf("Smiley points for May %d: %d\n", count+1, points[count]);
    
    return(0);
}
*/

/*
// Initialize when declaring
int main()
{
    int a[10] = {1,2,3,4,5,6,7,8,9,10};
    int count;
    for (count = 0; count < 10; count++)
    {
        printf("a[%d] is %d\n", count, a[count]);
    }
    return (0);
}
*/
/*
// Parially initialize when declaring
int main()
{
    int a[23] = {1,2,3,4,5,6,7,8,9,10};

    int count;
    for (count=0; count < 23; count++)
    {
        // should be 0 for indexed >= 10, suprise otherwise
        printf("a[%d] is %d\n", count, a[count]);
    }
}
*/
/*
int main()
{
    int a[10] = {1,2,3,4,5,6,7,8,9,10};
    int count;
    for(count = 0; count < 23; count++)
    {
        //Unpredictible for count >= 10
        printf("a[%d] is %d\n", count, a[count]);
    }
}
*/
/*
// Multidimensional arrays
// Total array size is limited(complier, operating system, hardware)
// No limit of dimensions within
int main()
{
    int table[3][5];
    table[0][0] = 1;

    printf("%d\n", table[0][0]);

    int table1[2][5] = {1,2,3,4,5,6,7,8,9,10};
    printf("%d\n", table1[1][2]); // gives 8
    
    return (0);
}
*/
/*
int main()
{
    int a[100];
    float b[100];
    int c[1000][1000];

    printf("Size of a is %d bytes\n", sizeof(a));
    printf("Size of b is %d bytes\n", sizeof(b));
    printf("Size if c is %d bytes\n", sizeof(c));

    return (0);
}
*/
/*
// Strings are character arrays, ending with /0
int main()
{
    char a[23] = {'P','o','t','s','d','a','m','\0'};
    printf("%s\n", a); // Potsdam
    printf("%c\n", a[0]); // P
    printf("%d\n", a[0]); // 80
    printf("%d\n", a[7]); // 0

    return (0);
} 
*/

// Pointers
// Pointers store memory locations
// * : indirection operator
// & : address of

// Pointer address
// Each byte in memory has a unique address
// An int is usually 4 bytes long
// A pointer stores the address of the first byte
// Full int is accessed by incrementing the pointer
// So giving the type is essential
/*
int main()
{
    // initialize variable i
    int i = 99;
    // Set pointer to variable i
    int *pointer_i = &i;
    // i and *pointer_i refer to value
    printf("i is %d, *pointer_i is %d\n", i, *pointer_i);
    // &i and pointer_i refer to address
    printf("%p, %p\n", &i, pointer_i);
}
*/
/*
int main()
{
    // Declare the pointer of type 'int *'
    int *pointer_v1;
    int* pointer_v2;
    int * ppointer_v3;

    // Asterix placement can affect interpretation
    int *p, i; // i is a regular int variable
    int* p1, p2; // both are pointers of type 'int *'  
}
*/
/*
// Array name is a pointer to the first byte
int main()
{
    int a[10];
    printf("Size of int : %lu\n", sizeof(int));
    printf("Start address : %d == %p\n", a , &a[0]);
    printf("Address of 2nd element : %p\n", &a[1]);
    printf("Address of 10th element : %p\n", &a[9]);
}
*/

/*
// Relation between arrays and pointers
// Only pointers can be given to functions
int i[10] = {1,2,3,4,5,6,7,8,9,10};
void print_first_element(int array[], int size);
int main()
{
    int *pointer_i;
    pointer_i = i;
    print_first_element(pointer_i , 10);
    return(0);
}
void print_first_element(int array[], int size)
{
    printf("First element: %d\n", array[0]);
}
*/

/*
// Pointer arithmetic
// increments by sizeof(typename)
#define MAX 13

int a[MAX] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
int *pointer_a, count;

int main()
{
    pointer_a = a;
    printf("Index\tValue\tAddress\n");
    for (count=0; count<MAX; count++)
    {
        printf("%d\t%d\t%p\n", count, *pointer_a++, pointer_a);
    }
    return (0);
}
*/
/*

// Arrays are passed as pointers
void fun(int array[]);

int main()
{
    int i, a[5] = {1,2,3,4,5};
    for (i=0; i<sizeof(a)/sizeof(int); i++)
        printf("Entry %d is %d\n", i+1, a[i]);
    fun(a);
    return (0);
}

void fun(int array [])
{   
    printf("sizeof(array) = %lu\n", sizeof(array));
    printf("sizeof(array)/sizeof(int) = %lu\n", sizeof(array)/sizeof(int));
}
*/
/*
void fun(int *array); // equals array[]

int main()
{
    int i, a[5] = {1,2,3,4,5};
    printf("sizeof(a) : %lu\n", sizeof(a));  // sizeof(a) = 20 , 5 integers × 4 bytes
    for (i=0; i<sizeof(a)/sizeof(int); i++)
       
        printf("Entry %d is %d\n", i+1, a[i]);
    fun(a);
    return (0);
}

void fun(int *array)
{   
    printf("sizeof(array) = %lu\n", sizeof(array)); 
    printf("sizeof(array)/sizeof(int) = %lu\n", sizeof(array)/sizeof(int));
}
*/
/*
When you pass a to fun(), you're passing a pointer to the first element — 
not the whole array. Inside fun():

array is just an int * (a pointer), not the actual array.

sizeof(array) returns the size of the pointer, which is typically:

8 bytes on 64-bit systems (like your Mac with arm64)

4 bytes on 32-bit systems
*/

/*
// Arrays and pointer arithmetics
int main()
{
    int i, a[5] = {1,2,3,4,5};

    for(i = 0; i<5; i++)
    {
        printf("a[%d] = %d\n", i, *a++); // Not working. Array addresses cannot be changed 
    }
    return(0);
}
*/
/*
int main()
{
    int i, a[5] = {1,2,3,4,5};
    int *pointer_a = a; // pointer that can be modified

    for(i = 0; i<5; i++)
    {
        printf("a[%d] = %d\n", i, *pointer_a++); // Not working. Array addresses cannot be changed 
    }
    return(0);
}
*/
/*
int main()
{
    int i, a[5] = {2,4,6,8,10};
    
    for(i = 0; i<5; i++)
    {
        printf("a[%d] = %d\n", i, *(a+i)); // This works. Array addresses is not be changed 
    }
    return(0);

}
*/
/*
// Passing by reference
void swap(int *a, int *b);

int main()
{
    int a = 1;
    int b = 2;
    printf("a=%d, b=%d\n", a, b);
    swap(&a,&b);
    printf("a=%d, b=%d\n", a, b);
    return(0);
}

void swap(int *a, int *b)
{
    int tmp = *b;
    *b = *a;
    *a = tmp;
}
*/
/*
// Pointers to pointers
int main()
{
    int i = 23;
    int *p1, **p2;

    p1 = &i;
    p2 = &p1;

    printf("i = %d\n", i); // 23
    printf("p1 = %p\n", p1); // addr1
    printf("*p1 = %d\n", *p1); // 23
    printf("p2 = %p\n", p2); //addr2 
    printf("*p2 = %p\n", *p2); //addr1
    printf("**p2 = %d\n", **p2); // 23
    
    return(0);
}
*/

/*
// Arrays of pointers
int main()
{
    char *welcome[4] = {"My", "name", "is", "Christian"};

    int i;
    for(i=0; i<4; i++)
        printf("%s", welcome[i]);
    printf("\n");

    return(0);
}
*/
/*
// Dynamic memory allocation
// malloc() function allows dynamic memory allocation
// looks for free memory and returns address
// Part of stdlib.h

#include <stdlib.h>

int main()
{
    char *string; // Allocate memory for a 95 character string
    string = (char*) malloc(96);

    int count;
    char *p = string;
    for(count=32; count<126; count++)
        *p++ = count;
    *p++ = '\0';

    puts(string);
    printf("%s\n", string);

    free(string); // freeing allocated memory

    return (0);
}
*/

/*
#include <stdlib.h>

int main()
{
    // Allocate memory for a user defined integer array
    int size;
    scanf("%d", &size);

    // int *a;
    //  a = (int*) malloc(size * sizeof(int));
    int *a = (int*) malloc(size * sizeof(int));
    if (a == NULL)
    {
        puts("Requested memory couldn't be allocated");
        exit(1);
    }
   

    free(a);

    return(0);
}
*/
/*
// Trees and linked lists
// Structures

#include <strings.h>
int main()
{
    struct sample{
        int height;
        int width;
    };

    struct sample s1;
    
    s1.height = 150;
    s1.width = 180;

    printf("h = %d, w=%d\n", s1.height, s1.width);

    struct sample2{
        char *name;
        int height;
        int width;
    };

    struct sample2 s2;
    s2.name = "Bacterial";
    s2.height = 150;
    s2.width = 180;

    printf("%s: h = %d, w=%d\n", s2.name, s2.height, s2.width);

    return(0);
}
*/

/*
#include <stdlib.h>
#include <string.h>

int main()
{
    struct student {
        char name[23];
        struct  student *next;    
    };

    typedef struct student student;

    student *new, *current, *head=NULL;
    
    // Adding first element
    new = (student*) malloc(sizeof(student));
    strcpy(new->name, "Christian");
    new -> next = NULL;
    head = new;

    // Adding additional element to the end
    current = head;
    while(current -> next != NULL)
    {
        current = current->next;
    }
    new = (student*) malloc(sizeof(student));
    strcpy(new->name, "Michael");
    new->next = NULL;
    current->next = new;

    // Print out elements
    current = head;
    while(current != NULL)
    {
        printf("%s\n", current->name);
        current = current->next;
    }

    // Free memory
    while(head != NULL)
    {
        current = head;
        head = head->next;
        free(current);
    }
    return(0);

}
*/

// Binary tree
#include <stdlib.h>

struct node
{
    int value;
    struct node *left;
    struct node *right;
};

int main()
{
    struct node* firstNode = (struct node*) malloc(sizeof(struct node));
    firstNode->value = 0;
    firstNode->left = NULL;
    firstNode->right=NULL;

    return(0);
}












