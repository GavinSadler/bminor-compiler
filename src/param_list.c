
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "param_list.h"
#include "type.h"

struct param_list *param_list_create(const char *name, struct type *type, struct param_list *next)
{
    struct param_list *p = malloc(sizeof(struct param_list));

    p->name = name;
    p->next = next;
    p->type = type;

    return p;
}

struct param_list *param_list_copy(struct param_list *p)
{
    if (!p)
        return NULL;

    struct param_list *pl = (struct param_list *)malloc(sizeof(struct param_list));

    pl->name = p->name;
    pl->symbol = p->symbol;
    pl->type = type_copy(p->type);

    pl->next = param_list_copy(p->next);

    return pl;
}

bool param_list_equals(struct param_list *a, struct param_list *b)
{
    // Symbols don't have to match, just the types

    // Both NULL means they're equal
    if (!a && !b)
        return true;

    // One or the other null? Not equal
    if (!a || !b)
        return false;

    // Make sure types match
    if (!type_equals(a->type, b->type))
        return false;

    // And recurse to the next parameter
    return param_list_equals(a->next, b->next);
}

void param_list_delete(struct param_list *p)
{
    if (!p)
        return;

    p->name = NULL;
    p->symbol = NULL;
    p->type = NULL;

    param_list_delete(p->next);
}

void param_list_print(struct param_list *a)
{
    if (!a)
        return;

    printf("%s: ", a->name);
    type_print(a->type);

    if (a->next)
    {
        printf(", ");
        param_list_print(a->next);
    }
}
