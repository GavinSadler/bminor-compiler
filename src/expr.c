
#include <stdlib.h>

#include "expr.h"

struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = kind;
    e->left = left;
    e->right = right;

    return e;
}

struct expr *expr_create_name(const char *n)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->name = n;

    return e;
}

struct expr *expr_create_integer_literal(int c)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->literal_value = c;

    return e;
}

struct expr *expr_create_boolean_literal(int c)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->literal_value = c;

    return e;
}

struct expr *expr_create_char_literal(char c)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->literal_value = c;

    return e;
}

struct expr *expr_create_string_literal(const char *str)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->string_literal = str;

    return e;
}
