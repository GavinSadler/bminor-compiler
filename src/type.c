
#include <stdlib.h>

#include "type.h"
#include "param_list.h"

struct type *type_create(type_t kind, struct type *subtype, struct param_list *params)
{
    struct type *t = malloc(sizeof(struct type));

    t->kind = kind;
    t->subtype = subtype;
    t->params = params;

    return t;
}
