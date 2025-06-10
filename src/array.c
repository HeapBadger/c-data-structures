/**
 * @file array.c
 * @brief Implementation of the dynamic array (vector) data structure.
 *
 * @section operations Operations
 * - **Create**: Creates a new dynamic array with an initial capacity.
 * - **Destroy**: Deletes all elements and frees memory used by the array.
 * - **Insert**: Adds a new element to the end or at a specific index in the
 * array.
 * - **Delete**: Removes an element from the array by index.
 * - **Get**: Retrieves an element from the array by index.
 * - **Set**: Updates the value of an element at a specific index.
 * - **Size**: Returns the current number of elements in the array.
 * - **Capacity**: Returns the total capacity of the array.
 * - **Is Empty**: Checks if the array is empty.
 * - **Clear**: Removes all elements from the array without freeing memory.
 * - **Reserve**: Increases the array's capacity to at least a specified amount.
 * - **Shrink To Fit**: Reduces capacity to match current size.
 * - **Find**: Searches for an element in the array by value.
 * - **Binary Search**: Searches for an element in a sorted array.
 * - **Sort**: Sorts the elements in the array.
 *
 * @section complexity Complexity
 * - **Time Complexity**:   O(1) for access, size, and capacity,
 *                          O(1) amortized for insertion and deletion at the
 * end, O(N) for insertion, deletion, find, and clear, O(log N) for binary
 * search on sorted arrays, O(N log N) for sort, where N is the number of
 * elements.
 * - **Space Complexity**:  O(N) for storing elements in the array,
 *                          where N is the current capacity.
 *
 * @author  heapbadger
 * @date    June 09, 2025
 */

#include <stdio.h>

#include "array.h"

// Create
array_t *
array_create (size_t initial_capacity)
{
    // TODO: implement
    printf("cap: %ld\n", initial_capacity);
    return NULL;
}

// Destroy
void
array_destroy (array_t *p_array)
{
    // TODO: implement
    if (NULL == p_array)
    {
        printf("NULL\n");
    }
    return;
}

// // Insert
// int array_push_back(array_t *a, T value) {
//     // TODO: implement
//     return -1;
// }

// int array_insert(array_t *a, size_t index, T value) {
//     // TODO: implement
//     return -1;
// }

// // Delete
// int array_pop_back(array_t *a, T *out) {
//     // TODO: implement
//     return -1;
// }

// int array_remove(array_t *a, size_t index, T *out) {
//     // TODO: implement
//     return -1;
// }

// // Get
// int array_get(const array_t *a, size_t index, T *out) {
//     // TODO: implement
//     return -1;
// }

// // Set
// int array_set(array_t *a, size_t index, T value) {
//     // TODO: implement
//     return -1;
// }

// Size
size_t
array_size (const array_t *p_array)
{
    // TODO: implement
    if (NULL == p_array)
    {
        printf("NULL\n");
    }
    return 0;
}

// Capacity
size_t
array_capacity (const array_t *p_array)
{
    // TODO: implement
    if (NULL == p_array)
    {
        printf("NULL\n");
    }
    return 0;
}

// // Is Empty
// bool array_is_empty(const array_t *a) {
//     // TODO: implement
//     return true;
// }

// // Clear
// void array_clear(array_t *a) {
//     // TODO: implement
// }

// // Reserve
// int array_reserve(array_t *a, size_t new_capacity) {
//     // TODO: implement
//     return -1;
// }

// // Shrink To Fit
// int array_shrink_to_fit(array_t *a) {
//     // TODO: implement
//     return -1;
// }

// // Find (Linear Search)
// ssize_t array_find(const array_t *a, T key) {
//     // TODO: implement
//     return -1;
// }

// // Binary Search (requires sorted array)
// ssize_t array_binary_search(const array_t *a, T key) {
//     // TODO: implement
//     return -1;
// }

// // Sort
// void array_sort(array_t *a, int (*cmp)(const T *, const T *)) {
//     // TODO: implement
// }