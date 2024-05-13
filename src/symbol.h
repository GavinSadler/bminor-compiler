
#ifndef SYMBOL_H
#define SYMBOL_H

#include "decl.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

typedef enum
{
    SYMBOL_LOCAL,
    SYMBOL_PARAM,
    SYMBOL_GLOBAL
} symbol_t;

struct symbol
{
    symbol_t kind;
    struct type *type;
    char *name;
    int which;
};

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name);

int symbol_graph(struct symbol* s);

void scope_initialize();

void scope_enter();

void scope_exit();

int scope_level();

void scope_bind(const char *name, struct symbol *sym);

struct symbol *scope_lookup(const char *name);

struct symbol *scope_lookup_current(const char *name);

void decl_resolve(struct decl *d);

void expr_resolve(struct expr *e);

void param_list_resolve(struct param_list *p);

void stmt_resolve(struct stmt *s);

void type_resolve(struct type *t);

#endif
