
#include <stdlib.h>

#include "decl.h"
#include "expr.h"
#include "stmt.h"

// Quick macro for doing indentation
#define INDENT(n)                                                                                                      \
    for (int i = 0; i < n; i++)                                                                                        \
        printf(" ");

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

void stmt_print(struct stmt *s, int indent)
{
    if (!s)
        return;

    switch (s->kind)
    {
    case STMT_DECL:
        INDENT(indent)
        decl_print(s->decl, indent);
        printf("\n");
        break;
    case STMT_EXPR:
        INDENT(indent)
        expr_print(s->expr);
        printf(";\n");
        break;
    case STMT_IF:
        INDENT(indent)
        printf("if( ");
        expr_print(s->expr);
        printf(" )\n");
        INDENT(indent)
        printf("{\n");
        stmt_print(s->body, indent + 4);
        INDENT(indent)
        printf("}\n");

        if (s->else_body)
        {
            INDENT(indent)
            printf("else\n");
            INDENT(indent)
            printf("{\n");
            stmt_print(s->else_body, indent + 4);
            INDENT(indent)
            printf("}\n");
        }

        break;
    case STMT_FOR:
        INDENT(indent)
        printf("for(");
        expr_print(s->init_expr);
        printf(" ; ");
        expr_print(s->expr);
        printf(" ; ");
        expr_print(s->next_expr);
        printf(")\n");
        INDENT(indent)
        printf("{\n");
        stmt_print(s->body, indent + 4);
        INDENT(indent)
        printf("}\n");
        break;
    case STMT_PRINT:
        INDENT(indent)
        printf("print ");
        expr_print(s->expr);
        printf(";\n");
        break;
    case STMT_RETURN:
        INDENT(indent)
        printf("return ");
        expr_print(s->expr);
        printf(";\n");
        break;
    default:
        printf("\nERROR: No print case for stmt of kind: %d\n", s->kind);
        exit(1);
        break;
    }

    if (s->next)
    {
        // Add a newline between statements
        printf("\n");
        stmt_print(s->next, indent);
    }
}
