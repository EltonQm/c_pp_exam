// Include standard input/output library for printf
#include <stdio.h>
// Include standard library for malloc and free
#include <stdlib.h>
// Include string library for strcpy
#include <string.h>

// Define the student structure
struct student {
    char name[23]; // Fixed-size array for student name (up to 22 characters + null terminator)
    struct student* next; // Pointer to the next student in the linked list
};

// Typedef to simplify referencing the student structure
typedef struct student student;

// Main function
int main() {
    // Initialize pointers for the linked list
    student *new, *current, *head = NULL;

    // Adding first element ("Christian")
    new = (student*)malloc(sizeof(student)); // Allocate memory for a new student
    if (new == NULL) { // Check for allocation failure
        printf("Memory allocation failed!\n");
        return 1;
    }
    strcpy(new->name, "Christian"); // Copy the name "Christian" into the student's name field
    new->next = NULL; // Set the next pointer to NULL (end of list)
    head = new; // Set head to point to the first student

    // Adding additional element ("Michael") to the end
    current = head; // Start at the head of the list
    while (current->next != NULL) { // Traverse to the last node
        current = current->next;
    }
    new = (student*)malloc(sizeof(student)); // Allocate memory for another student
    if (new == NULL) { // Check for allocation failure
        printf("Memory allocation failed!\n");
        return 1;
    }
    strcpy(new->name, "Michael"); // Copy the name "Michael" into the student's name field
    new->next = NULL; // Set the next pointer to NULL (new end of list)
    current->next = new; // Link the last node to the new student

    // Print out elements
    current = head; // Start at the head of the list
    while (current != NULL) { // Traverse the list until the end
        printf("%s\n", current->name); // Print the student's name
        current = current->next; // Move to the next student
    }

    // Free memory
    while (head != NULL) { // Traverse the list to free each node
        current = head; // Store the current node
        head = head->next; // Move head to the next node
        free(current); // Free the current node's memory
    }

    return 0; // Return 0 to indicate successful execution
}
