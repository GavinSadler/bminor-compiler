
#include <stdio.h>
#include <stdlib.h>

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "resolve.h"
#include "scope.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

void decl_resolve(struct decl *d)
{
    if (!d)
        return;

    symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;

    d->symbol = symbol_create(kind, d->type, d->name);

    expr_resolve(d->value);
    scope_bind(d->name, d->symbol);

    if (d->code)
    {
        scope_enter();
        param_list_resolve(d->type->params);
        stmt_resolve(d->code);
        scope_exit();
    }

    decl_resolve(d->next);
}

void expr_resolve(struct expr *e)
{
    if (!e)
        return;

    if (e->kind == EXPR_NAME)
    {
        struct symbol *s = scope_lookup(e->name);

        if (!s)
        {
            printf("ERROR: Symbol '%s' referenced, but not yet declared\n", e->name);
            // We need to exit here since typechecking depends on a resolvable program
            exit(1);
        }

        e->symbol = s;
    }
    else
    {
        expr_resolve(e->left);
        expr_resolve(e->right);
    }
}

void param_list_resolve(struct param_list *p)
{
    if (!p)
        return;

    p->symbol = symbol_create(SYMBOL_PARAM, p->type, p->name);
    scope_bind(p->symbol->name, p->symbol);

    param_list_resolve(p->next);
}

void stmt_resolve(struct stmt *s)
{
    if (!s)
        return;

    switch (s->kind)
    {
    case STMT_DECL:
        decl_resolve(s->decl);
        break;
    case STMT_EXPR:
        expr_resolve(s->expr);
        break;
    case STMT_IF:
        scope_enter();
        expr_resolve(s->expr);
        stmt_resolve(s->body);
        stmt_resolve(s->else_body);
        scope_exit();
        break;
    case STMT_FOR:
        scope_enter();
        expr_resolve(s->init_expr);
        expr_resolve(s->expr);
        expr_resolve(s->next_expr);
        stmt_resolve(s->body);
        scope_exit();
        break;
    case STMT_PRINT:
        expr_resolve(s->expr);
        break;
    case STMT_RETURN:
        expr_resolve(s->expr);
        break;
    case STMT_BLOCKSTART:
        scope_enter();
        break;
    case STMT_BLOCKEND:
        scope_exit();
        break;
    default:
        printf("ERROR: Unkown statement type %d when trying to resolve statement\n", s->kind);
        exit(1);
        break;
    }

    stmt_resolve(s->next);
}

void type_resolve(struct type *t)
{
    if (!t)
        return;

    param_list_resolve(t->params);
    type_resolve(t->subtype);
}
