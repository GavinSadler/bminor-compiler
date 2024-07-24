
#include <stdio.h>
#include <stdlib.h>

#include "decl.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

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

void decl_print(struct decl *d, int indent)
{
    if (!d)
        return;

    printf("%s: ", d->name);
    type_print(d->type);

    if (d->value)
    {
        printf(" = ");
        expr_print(d->value);
        printf(";");
    }
    else if (d->code)
    {
        printf(" =\n{\n");
        stmt_print(d->code, indent + 4);
        printf("}");
    }
    else
    {
        printf(";");
    }

    // This is the special case of a toplevel declaration
    // Lets add two newlines to make things look neat
    if (indent == 0)
        printf("\n\n");

    decl_print(d->next, indent);
}

extern int graph_node_id_counter;

int decl_graph(struct decl *d)
{
    if (!d)
        return -1;

    int node_id = graph_node_id_counter;
    graph_node_id_counter++;

    // The definition of the node
    printf("\"decl_%06d\"[\n", node_id);
    printf(
        "\tlabel = \"{ decl: %s | { <type> type | <value> value | <code> code | <symbol> symbol | <next> next }}\"\n",
        d->name);
    printf("\tfillcolor = \"lightgreen\"\n");
    printf("\tshape = \"record\"\n");
    printf("];\n\n");

    // Graph children nodes
    int type_node_id = type_graph(d->type);
    int value_node_id = expr_graph(d->value);
    int code_node_id = stmt_graph(d->code);
    int symbol_node_id = symbol_graph(d->symbol);
    int next_node_id = decl_graph(d->next);

    // Only print edges if a corresponding node exists
    if (type_node_id != -1)
        printf("\"decl_%06d\":type -> \"type_%06d\";\n", node_id, type_node_id);

    if (value_node_id != -1)
        printf("\"decl_%06d\":value -> \"expr_%06d\";\n", node_id, value_node_id);

    if (code_node_id != -1)
        printf("\"decl_%06d\":code -> \"stmt_%06d\";\n", node_id, code_node_id);

    if (symbol_node_id != -1)
        printf("\"decl_%06d\":symbol -> \"symbol_%06d\" [style=\"dashed\"];\n", node_id, symbol_node_id);

    if (next_node_id != -1)
        printf("\"decl_%06d\":next -> \"decl_%06d\";\n", node_id, next_node_id);

    return node_id;
}
