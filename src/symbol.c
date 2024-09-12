
#include <stdlib.h>

#include "symbol.h"

const char* symbol_t_strings[] = {
    #define X(s) #s,
    SYMBOLS
    #undef X
};

struct symbol *symbol_create(symbol_t kind, struct type *type, const char *name)
{
    struct symbol *s = malloc(sizeof(struct symbol));

    s->kind = kind;
    s->type = type;
    s->name = name;

    return s;
}
