
#include <stdio.h>

#include "symbol.h"
#include "type.h"

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name)
{
    struct symbol *s = malloc(sizeof(struct symbol));

    s->kind = kind;
    s->type = type;
    s->name = name;

    // TODO: Figure out what the which field is for

    return s;
}
