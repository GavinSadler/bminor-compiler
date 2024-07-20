
#ifndef STACK_H
#define STACK_H

typedef void *stack;

stack stack_create();

void stack_destroy(stack s);

void stack_push(stack s, void *item);

void *stack_pop(stack s);

void *stack_peek(stack s);

void *stack_item(stack s, int index);

int stack_size(stack s);

#endif
