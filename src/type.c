
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "param_list.h"
#include "type.h"

struct type *type_create(type_t kind, struct type *subtype, struct param_list *params)
{
    struct type *t = malloc(sizeof(struct type));

    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    t->size = 0;

    return t;
}

bool type_equals(struct type *a, struct type *b)
{
    // Null check
    if (!a && !b)
        return true;

    // If either or are null...
    if (!a || !b)
        return false;

    if (a->kind == b->kind)
    {
        // Since array and function types are 5 and 6 as integers,
        // we can simply check to see if they are less than 5
        if (a->kind < 5)
        {
            return true;
        }
        else if (a->kind == TYPE_ARRAY)
        {
            return type_equals(a->subtype, b->subtype);
        }
        else if (a->kind == TYPE_FUNCTION)
        {
            // Functions use the subtype field as their returnt type
            return type_equals(a->subtype, b->subtype) && param_list_equals(a->params, b->params);
        }
    }

    return false;
}

struct type *type_copy(struct type *t)
{
    if (!t)
        return NULL;

    struct type *t_copy = malloc(sizeof(struct type));

    t_copy->kind = t->kind;
    t_copy->size = t->size;
    t_copy->params = param_list_copy(t->params);
    t_copy->subtype = type_copy(t->subtype);

    return t_copy;
}

void type_delete(struct type *t)
{
    if (!t)
        return;

    type_delete(t->subtype);
    t->subtype = NULL;

    param_list_delete(t->params);
    t->params = NULL;

    t->kind = 0;
    t->size = 0;

    free(t);
    t = NULL;
}
