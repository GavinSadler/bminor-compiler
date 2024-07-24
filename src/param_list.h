
#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include "/home/gavin/bminor-compiler/src/param_list.h"

#include <stdbool.h>
#include <stdio.h>

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "type.h"

struct expr;
struct type;

struct param_list
{
    const char *name;
    struct type *type;
    struct symbol *symbol;
    struct param_list *next;
};

struct param_list *param_list_create(const char *name, struct type *type, struct param_list *next);

struct param_list *param_list_copy(struct param_list *p);

bool param_list_equals(struct param_list *a, struct param_list *b);

void param_list_delete(struct param_list *p);

void param_list_print(struct param_list *a);

int param_list_graph(struct param_list *p);

#endif
