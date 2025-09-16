

/*
A Linked List is, as the name implies, a list where nodes are linked together.
Each node contains some data and a pointer to the next node.

One major benefit of using linked lists is that nodes can be stored anywhere in memory.
They don't need to be placed contiguously like elements in an array.
This flexibility allows easier memory allocation.

Another key advantage is that adding or removing nodes is efficient,
as there’s no need to shift other elements (unlike in arrays).
*/

// Linked lists do not have indices like arrays do.
// An ArrayList (or an array) stores elements in continuous memory blocks.

// Nodes in a linked list are made up of two parts:
// 1. Data
// 2. A pointer to the next node

// Nodes are usually stored in non-contiguous memory locations.
// They are connected through pointers.

// Types of linked lists:
// - Singly Linked List
// - Doubly Linked List

// Example: Singly Linked List
//                     |
//         Node            Node              Node
//    [data | next] -> [data | next] -> [data | next]

// Example: Doubly Linked List
//                          |
//            Node                  Node                   Node
// [prev | data | next] <-> [prev | data | next] <-> [prev | data | next]

// Advantages:
// 1. Dynamic data structure (grows/shrinks as needed).
// 2. No need for contiguous memory.
// 3. Insertion and deletion are efficient — O(1) if you have the pointer to the node.
// 4. Low memory waste compared to static arrays.

// Disadvantages:
// 1. Higher memory usage (extra pointer per node).
// 2. No random access — you can't use an index like arr[i].
// 3. Searching/accessing elements takes longer — O(n) in worst case.

// Use Cases:
// - Implementation of stacks and queues.
// - GPS route navigation systems.
// - Music playlist navigation.
// - Undo/Redo functionality in editors.
