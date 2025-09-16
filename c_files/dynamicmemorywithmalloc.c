#include <stdio.h>
#include <stdlib.h>

// Function to generate an array of a given size
// Returns a pointer to a dynamically allocated array containing values 0 to size-1
int* generateArray(int size) {
    // Allocate memory dynamically for 'size' integers
    int* array = malloc(sizeof(int) * size);

    // Check if memory allocation was successful
    if (array == NULL) {
        printf("Memory allocation failed in generateArray().\n");
        return NULL;  // Return NULL on failure
    }

    // Initialize the array with values from 0 to size-1
    for (int i = 0; i < size; i++) {
        array[i] = i;
    }

    return array;  // Return the pointer to the dynamically allocated array
}

int main() {
    // ---- PART 1: Create and print initial array ----
    int initialSize = 10;
    int* array = malloc(sizeof(int) * initialSize);

    if (array == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Initialize the array
    for (int i = 0; i < initialSize; i++) {
        array[i] = i;
    }

    // Print the array
    printf("Initial array:\n");
    for (int i = 0; i < initialSize; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // ---- PART 2: Reallocate array to size 11 ----
    int count = initialSize;
    count++;  // Increase count to 11

    int* temp = realloc(array, sizeof(int) * count);
    if (temp == NULL) {
        printf("Reallocation failed.\n");
        free(array);  // Free old memory if realloc fails
        return 1;
    }
    array = temp;

    // Fill and print the updated array
    for (int i = 0; i < count; i++) {
        array[i] = i;
    }

    printf("Reallocated array (size 11):\n");
    for (int i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // ---- PART 3: Double the size of the array to 20 ----
    int size = count;  // current size = 11
    size *= 2;         // new size = 22

    temp = realloc(array, sizeof(int) * size);
    if (temp == NULL) {
        printf("Memory reallocation (doubling size) failed.\n");
        free(array);
        return 1;
    }
    array = temp;

    // Add more elements
    for (int i = count; i < size; i++) {
        array[i] = i;
    }

    count = size;  // Update count to reflect actual elements now

    printf("Doubled array (size %d):\n", size);
    for (int i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // ---- PART 4: Demonstrate returning dynamically allocated array from a function ----
    int* newArray = generateArray(size);
    if (newArray == NULL) {
        free(array);
        return 1;
    }

    printf("Generated array from function:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", newArray[i]);
    }
    printf("\n");

    // ---- Cleanup: Free all dynamically allocated memory ----
    free(array);
    free(newArray);

    return 0;
}

/*

The function realloc() is used to resize a previously allocated block of memory.

🧠 What it does (in plain terms)
realloc takes a pointer to memory (from malloc, calloc, or even a previous realloc) and resizes
it to a new size.

It tries to expand or shrink the memory in-place if possible.

If it can't do it in-place, it:

Allocates a new block of the desired size.

Copies the existing data into the new block.

Frees the old block (automatically).

Returns a pointer to the new block.

📦 Syntax
void* realloc(void* ptr, size_t new_size);
ptr: Pointer to previously allocated memory

new_size: The new size you want (in bytes)

Returns:

New memory address (pointer)

NULL if reallocation fails (original memory is not freed in that case)

📌 Example

int* array = malloc(5 * sizeof(int));  // allocate space for 5 integers

// Now want to resize it to hold 10 integers
int* new_array = realloc(array, 10 * sizeof(int));

if (new_array == NULL) {
    // realloc failed: original 'array' still valid, must be freed manually
    free(array);
    printf("Reallocation failed.\n");
} else {
    array = new_array;  // Success: use the resized memory
}
⚠️ Important Notes
If realloc fails, it returns NULL, and the original memory is not freed, so you must still free() it 
manually.

If you assign realloc() directly to the original pointer like this:

array = realloc(array, new_size);
and it fails, you’ve lost the pointer to the original memory, which causes a memory leak.
💡 Always use a temporary pointer like:

int* temp = realloc(array, new_size);
if (temp != NULL) array = temp;
If you pass NULL as the pointer, realloc(NULL, size) behaves like malloc(size).

If new_size == 0, behavior is implementation-defined. Some treat it like free(ptr), others return a
non-NULL pointer. 
malloc

✅ In Summary
realloc is a convenient way to grow or shrink dynamic memory buffers, like resizing an array as more
data comes in. It helps avoid the cost of manual copying and freeing, but you must use it carefully 
to avoid leaks or undefined behavior.

malloc returns a void pointer (void *), which indicates that it is a pointer to a region of unknown data type. 
The use of casting is required in C++ due to the strong type system, whereas this is not the case in C.
One may "cast" (see type conversion) this pointer to a specific type:

int *ptr, *ptr2;
ptr = malloc(10 * sizeof(*ptr)); // without a cast 
ptr2 = (int *)malloc(10 * sizeof(*ptr)); // with a cast 

*/

/*
Common errors
- Not checking for allocation failures:
+ Memory allocation is not guaranteed to succeed, and may instead return a null pointer.
+ Using the returned value, without checking if the allocation is successful, invokes undefined behavior.
+ This usually leads to crash (due to the resulting segmentation fault on the null pointer dereference),
  but there is no guarantee that a crash will happen so relying on that can also lead to problems.

- Memory leaks
+ Failure to deallocate memory using free leads to buildup of non-reusable memory, 
  which is no longer used by the program. 
+ This wastes memory resources and can lead to allocation failures when these resources are exhausted.

- Logical errors
+ All allocations must follow the same pattern: allocation using malloc, usage to store data, 
  deallocation using free. 
+ Failures to adhere to this pattern, such as memory usage after a call to free (dangling pointer) or 
  before a call to malloc (wild pointer), calling free twice ("double free"), etc., usually 
  causes a segmentation fault and results in a crash of the program. These errors can be transient 
  and hard to debug – for example, freed memory is usually not immediately reclaimed by the OS, and 
  thus dangling pointers may persist for a while and appear to work.
*/