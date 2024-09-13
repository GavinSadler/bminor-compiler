
#include <stdbool.h>
#include <stdlib.h>

#include "ast.h"

// ======================
// Enum String Generation
// ======================

const char *exrp_t_strings[] = {
#define X(e) #e,
    EXPRS
#undef X
};

const char *stmt_t_strings[] = {
#define X(s) #s,
    STMTS
#undef X
};

const char *type_t_strings[] = {
#define X(t) #t,
    TYPES
#undef X
};

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

    e->kind = EXPR_NAME;
    e->name = n;

    return e;
}

struct expr *expr_create_integer_literal(int c)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = EXPR_INTEGERLITERAL;
    e->literal_value = c;

    return e;
}

struct expr *expr_create_boolean_literal(int c)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = EXPR_BOOLEANLITERAL;
    e->literal_value = c;

    return e;
}

struct expr *expr_create_char_literal(char c)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = EXPR_CHARLITERAL;
    e->literal_value = c;

    return e;
}

struct expr *expr_create_string_literal(const char *str)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = EXPR_STRINGLITERAL;
    e->string_literal = str;

    return e;
}

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

struct stmt *stmt_create(stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr,
                         struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next)
{
    struct stmt *s = malloc(sizeof(struct stmt));

    s->body = body;
    s->decl = decl;
    s->else_body = else_body;
    s->expr = expr;
    s->init_expr = init_expr;
    s->kind = kind;
    s->next = next;
    s->next_expr = next_expr;

    return s;
}

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
