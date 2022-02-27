/**
    Linked list
    All rights reserved
    Elia Karrer, 2022
*/


#include <stdio.h>


/// Single node element
typedef struct NODE
{
    int value;
    struct NODE* ptr;
} node;

typedef struct NODE* list;


/// Creates a new list and returns address of first element
node* list_new(unsigned int length)
{
    node* list;
    unsigned int i;
        
    // Allocate memory for list
    list = (node*) malloc(length * sizeof(node));

    // Error check
    if(list == NULL)
        return NULL;

    // Link list elements
    for(i = 0; i < length-1; i++)
        list[i].ptr =  &list[i+1];
    list[length-1].ptr = NULL;
    
    // Return first list element
    return list;
}


/// Returns the pointer to element with wanted index
node* list_addr_index(node* start, int index)
{
    node* current = start -> ptr;

    // Exception list[0]
    if(index == 0)
        return start;

    // Loop trough list until wanted index is found or list ends
    for(unsigned int i = 1; i < index; i++)
    {
        // Move 1 element forward
        current = current -> ptr;

        // Avoid segmentation fault:
        // If index is out of range, highest possible gets returned
        if(current -> ptr == NULL)
            break;
    }

    // Return address of last element
    return current;
}


/// Return pointer to last element
node* list_addr_end(node* start)
{
    node* current = start -> ptr;

    // Loop trough list until list ends
    while(current -> ptr != NULL)
        // Move 1 element forward
        current = current -> ptr;

    // Return address of last element
    return current;
}


/// Appends list b to list a
void list_add(node* a, node* b)
{
    // Gets last element of list
    node* end = list_addr_end(a);

    // Links appended part to main list
    end -> ptr = b;
}


/// Sets the value of element in list
void list_set(node* start, unsigned int index, int val)
{
    node* elem = list_addr_index(start, index);
    elem -> value = val;
}


/// Gets the value of element in list
int list_get(node* start, unsigned int index)
{
    node* elem = list_addr_index(start, index);
    return elem -> value;
}


/// Appends n-elements to list
void list_append(node* start, int length)
{
    // Create new list to append
    node* newlist = list_new(length);

    // Check for errors while creating new list
    if(newlist == NULL)
        return;

    // Append new list to existing list
    list_add(start, newlist);
}


/// Removes specific element from list
void list_remove(node* start, unsigned int index)
{
    // Exception list[0]
    if(index == 0)
    {
        // Change start
        start = start -> ptr;
        return;
    }

    // Exception list[max]
    if(list_addr_index(start, index) -> ptr == NULL)
    {
        // End list 1 before
        node* temp = list_addr_index(start, index-1);
        temp -> ptr = NULL;
        return;
    }

    // Get addresses
    node* element_before_rem = list_addr_index(start, index-1);
    node* element_after_rem = list_addr_index(start, index+1);

    // Set
    element_before_rem -> ptr = element_after_rem;
}



/// Example code
int main(void)
{
    // Create new list with 10 elements
    list x = list_new(10);

    // Append 90 elements to the list
    list_append(x, 90);

    // Enumerate list values with "index"
    for(int i = 0; i < 100; i++)
        list_set(x, i, i);

    // Removes last element
    list_remove(x, 99);

    // Output every element of list with overflow
    for(int i = 0; i < 110; i++)
        printf("x[%d] = %d\n", i, list_get(x, i));
}
