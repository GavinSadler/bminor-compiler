
#ifndef DAG_H
#define DAG_H

#include "decl.h"
#include "expr.h"
#include "stmt.h"

typedef enum
{
    DAG_ASSIGN,
    DAG_DEREF,
    DAG_IADD,
    DAG_IMUL,
    // ...
    DAG_NAME,
    DAG_FLOAT_VALUE,
    DAG_INTEGER_VALUE
} dag_kind_t;

struct dag_node
{
    dag_kind_t kind;

    struct _dag_node *left;
    struct _dag_node *right;

    union {
        const char *name;
        double float_value;
        int integer_value;
    } value;
};

struct dag_node *decl_to_dag(struct decl *d);

struct dag_node *stmt_to_dag(struct stmt *s);

struct dag_node *expr_to_dag(struct expr *e);

#endif
