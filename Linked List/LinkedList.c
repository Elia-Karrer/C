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


/// Creates a new list and returns address of first element
node* list_new(unsigned int length)
{
    // Allocate memory for list
    node* list = (node*) malloc(length * sizeof(node));

    // Error check
    if(list == NULL)
        return NULL;

    // Link list elements
    for(unsigned int i = 0; i < length; i++)
        list[i].ptr = (i == length-1) ? NULL : &list[i+1];

    // Return first list element
    return list;
}


/// Sets the value of element in list
void list_set(node* start, unsigned int index, int val)
{
    node* current = start -> ptr;

    // Exception list[0]
    if(index == 0)
        start -> value = val;

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

    // Set value of found list element
    current -> value = val;
}


/// Gets the value of element in list
int list_get(node* start, unsigned int index)
{
    node* current = start -> ptr;

    // Exception list[0]
    if(index == 0)
        return start -> value;

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

    // Return value of found list element
    return current -> value;
}


/// EXAMPLE CODE
int main(void)
{
    unsigned int i;

    // Create list
    node* list = list_new(8);

    // Set every element in list to index number
    for(i = 0; i < 8; i++)
        list_set(list, i, i);

    // Output list with overflow
    for(i = 0; i < 10; i++)
        printf("list[%d] = %d\n", i, list_get(list, i));
}
