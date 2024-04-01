
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

void type_print(struct type *t)
{
    switch (t->kind)
    {
    case TYPE_VOID:
        printf("void");
        break;
    case TYPE_BOOLEAN:
        printf("boolean");
        break;
    case TYPE_CHARACTER:
        printf("char");
        break;
    case TYPE_INTEGER:
        printf("integer");
        break;
    case TYPE_STRING:
        printf("string");
        break;
    case TYPE_ARRAY:
        if (!t->size)
            printf("array []");
        else
            printf("array [%d]", t->size);

        if (t->subtype)
        {
            printf(" ");
            type_print(t->subtype);
        }
        break;
    case TYPE_FUNCTION:
        printf("function ");
        type_print(t->subtype);
        printf(" ( ");
        param_list_print(t->params);
        printf(" )");
        break;
    default:
        printf("\nERROR: No print case for type of kind: %d\n", t->kind);
        exit(1);
        break;
    }
}
