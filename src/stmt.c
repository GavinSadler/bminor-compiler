
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

extern int graph_node_id_counter;

int stmt_graph(struct stmt *s)
{
    if (!s)
        return -1;

    int node_id = graph_node_id_counter;
    graph_node_id_counter++;

    // The definition of the node
    printf("\"stmt_%06d\"[\n", node_id);
    printf("\tlabel = \"{ stmt: ");

    switch (s->kind)
    {
    case STMT_DECL:
        printf("decl");
        break;
    case STMT_EXPR:
        printf("expr");
        break;
    case STMT_IF:
        printf("if");
        break;
    case STMT_FOR:
        printf("for");
        break;
    case STMT_PRINT:
        printf("print");
        break;
    case STMT_RETURN:
        printf("return");
        break;
    case STMT_BLOCKSTART:
        printf("blockstart");
        break;
    case STMT_BLOCKEND:
        printf("blockend");
        break;
    default:
        printf("ERROR: Unknown enum stmt_t encountered when trying to graph stmt node: %d\n", s->kind);
        exit(1);
        break;
    }

    printf(
        "| { <decl> decl | <init_expr> init_expr | <expr> expr | <next_expr> next_expr | <body> body | else_body <else_body> | <next> next }}\"\n");
    printf("\tfillcolor = \"lightslateblue\"\n");
    printf("\tshape = \"record\"\n");
    printf("];\n\n");

    // Graph children nodes
    int decl_node_id = decl_graph(s->decl);
    int init_expr_node_id = expr_graph(s->init_expr);
    int expr_node_id = expr_graph(s->expr);
    int next_expr_node_id = expr_graph(s->next_expr);
    int body_node_id = stmt_graph(s->body);
    int else_body_node_id = stmt_graph(s->else_body);
    int next_node_id = stmt_graph(s->next);

    // Only print edges if a corresponding node exists
    if (decl_node_id != -1)
        printf("\"stmt_%06d\":decl -> \"decl_%06d\";\n", node_id, decl_node_id);

    if (init_expr_node_id != -1)
        printf("\"stmt_%06d\":init_expr -> \"expr_%06d\";\n", node_id, init_expr_node_id);

    if (expr_node_id != -1)
        printf("\"stmt_%06d\":expr -> \"expr_%06d\";\n", node_id, expr_node_id);

    if (next_expr_node_id != -1)
        printf("\"stmt_%06d\":next_expr -> \"expr_%06d\";\n", node_id, next_expr_node_id);

    if (body_node_id != -1)
        printf("\"stmt_%06d\":body -> \"stmt_%06d\";\n", node_id, body_node_id);

    if (else_body_node_id != -1)
        printf("\"stmt_%06d\":else_body -> \"stmt_%06d\";\n", node_id, else_body_node_id);

    if (next_node_id != -1)
        printf("\"stmt_%06d\":next -> \"stmt_%06d\";\n", node_id, next_node_id);

    return node_id;
}
