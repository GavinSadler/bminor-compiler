
#include "stack.h"

#include <stdlib.h>

typedef struct stack_node
{
    void *data;
    struct stack_node *next;
} stack_node;

typedef struct stack_
{
    stack_node *top;
} stack_;

stack stack_create()
{
    stack_ *stk = malloc(sizeof(stack_));

    stk->top = NULL;

    return stk;
}

void stack_destroy(stack s)
{
    stack_ *stk = (stack_ *)s;

    stack_node *next_node = stk->top;

    while (next_node != NULL)
    {
        stack_node *next = next_node->next;

        next_node->data = NULL;
        next_node->next = NULL;

        free(next_node);

        next_node = next;
    }

    stk->top = NULL;

    free(stk);
}

void stack_push(stack s, void *item)
{
    stack_ *stk = (stack_ *)s;

    stack_node *new_node = malloc(sizeof(stack_node));

    new_node->data = item;
    new_node->next = stk->top;

    stk->top = new_node;
}

void *stack_pop(stack s)
{
    stack_ *stk = (stack_ *)s;

    if (stk->top == NULL)
        return NULL;

    void *item = stack_peek(s);

    stack_node *new_top = stk->top->next;

    stk->top->data = NULL;
    stk->top->next = NULL;

    free(stk->top);

    stk->top = new_top;

    return item;
}

void *stack_peek(stack s)
{
    stack_ *stk = (stack_ *)s;

    if (stk->top == NULL)
        return NULL;

    return stk->top->data;
}

void *stack_item(stack s, int index)
{
    stack_ *stk = (stack_ *)s;

    stack_node *next_node = stk->top;

    for (int i = stack_size(s) - 1; i > index; i--)
    {
        if (next_node == NULL)
            return NULL;

        next_node = next_node->next;
    }

    return next_node->data;
}

int stack_size(stack s)
{
    stack_ *stk = (stack_ *)s;

    int size = 0;

    stack_node *next_node = stk->top;

    while (next_node != NULL)
    {
        size++;
        next_node = next_node->next;
    }

    return size;
}
