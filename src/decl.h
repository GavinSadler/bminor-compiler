
#ifndef DECL_H
#define DECL_H

#include <stdio.h>

#include "expr.h"
#include "stmt.h"
#include "type.h"

struct decl
{
    char *name;
    struct type *type;
    struct expr *value;
    struct stmt *code;
    struct symbol *symbol;
    struct decl *next;
};

struct decl *decl_create(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next);

void decl_print(struct decl *d, int indent);

int decl_graph(struct decl *d);

#endif
