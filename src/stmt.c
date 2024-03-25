
#include <stdlib.h>

#include "decl.h"
#include "expr.h"
#include "stmt.h"

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
