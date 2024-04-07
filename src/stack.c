
#include "stack.h"

#include <stdlib.h>

typedef struct stack_node
{
    void *data;
    stack_node *next;
    stack_node *previous;
} stack_node;

typedef struct stack_info
{
    stack_node *base;
    stack_node *head;
    int size;
} stack_info;

stack stack_create()
{
    stack_info *si = (stack_info *)malloc(sizeof(stack_info));

    si->base = NULL;
    si->head = NULL;
    si->size = 0;

    return (stack)si;
}

void stack_destroy(stack s)
{
    stack_info *si = (stack_info *)s;

    stack_node *sn = si->base;

    // Destroy the stack from the bottom-up
    while (sn != NULL)
    {
        stack_node *next = sn->next;

        sn->data = NULL;
        sn->next = NULL;
        sn->previous = NULL;
        free(sn);

        sn = next;
    }

    // Clean up the stack info
    si->base = NULL;
    si->head = NULL;
    si->size = 0;

    free(si);
    // NOTE: We are not responsible, at least in this implementation, of destroying whatever
    // is in the data field. If we need to we can implement this later
}

void stack_push(stack s, void *item)
{
    stack_info *si = (stack_info *)s;

    stack_node *nn = (stack_node *)malloc(sizeof(stack_node));

    nn->data = item;
    nn->next = NULL;
    nn->previous = si->head;

    // If head == NULL (or base == NULL for that matter), then the stack is empty
    // Otherwise, it has something in it we can attach to
    if (si->head != NULL)
        si->head->next = nn;
    else
        si->base = nn;

    si->head = nn;
    si->size += 1;
}

void *stack_pop(stack s)
{
    // TODO: Handle when at the end of the stack or an empty stack
    stack_info *si = (stack_info *)s;

    void *item = si->head->data;
    void *new_head = si->head->previous;

    si->head->data = NULL;
    si->head->previous = NULL;
    free(si->head);

    si->head = new_head;

    return item;
}

void *stack_peek(stack s)
{
    stack_node *sn = (stack_node *)s;

    // Accounting for an empty stack
    if (sn == NULL)
        return NULL;

    // If there is no next item, we are at the top of the stack
    // Return what we have
    if (sn->next == NULL)
        return sn->data;

    return stack_peek(sn->next)
}

void *stack_item(stack s, int index);

int stack_size(stack s)
{
    stack_node *sn = (stack_node *)s;

    if (sn == NULL)
        return 0;

    return stack_size((stack)sn->next) + 1;
}
