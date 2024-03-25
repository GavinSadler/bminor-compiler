
#include <stdio.h>

#include "param_list.h"
#include "type.h"

struct param_list *param_list_create(char *name, struct type *type, struct param_list *next)
{
    struct param_list *p = malloc(sizeof(struct param_list));

    p->name = name;
    p->next = next;
    p->type = type;

    // TODO: p->symbol = ???

    return p;
}
