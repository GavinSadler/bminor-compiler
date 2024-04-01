
#include <stdlib.h>

#include "decl.h"

struct decl *decl_create(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next)
{
    struct decl *d = malloc(sizeof(struct decl));

    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;

    return d;
}

void decl_print(struct decl *d, int indent)
{
    if (!d)
        return;

    printf("%s: ", d->name);
    type_print(d->type);

    if (d->value)
    {
        printf(" = ");

        if (d->type->kind == TYPE_ARRAY)
        {

        }
        
        expr_print(d->value);
        printf(";");
    }
    else if (d->code)
    {
        printf(" =\n{\n");
        stmt_print(d->code, indent + 4);
        printf("}");
    }
    else
    {
        printf(";");
    }

    // This is the special case of a toplevel declaration
    // Lets add two newlines to make things look neat
    if (indent == 0)
        printf("\n\n");

    decl_print(d->next, indent);
}
