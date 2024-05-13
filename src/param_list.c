
#include <stdlib.h>

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "type.h"

struct param_list *param_list_create(char *name, struct type *type, struct param_list *next)
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

void param_list_print(struct param_list *a)
{
    if (!a)
        return;

    printf("%s: ", a->name);
    type_print(a->type);

    if (a->next)
    {
        printf(", ");
        param_list_print(a->next);
    }
}

extern int graph_node_id_counter;

int param_list_graph(struct param_list *p)
{
    if (!p)
        return -1;
    
    int node_id = graph_node_id_counter;
    graph_node_id_counter++;

    // The definition of the node
    printf("\"param_list_%06d\"[\n", node_id);
    printf(
        "\tlabel = \"{ parameter: %s | { <type> type | <symbol> symbol | <next> next }}\"\n",
        p->name);
    printf("\tshape = \"record\"\n");
    printf("];\n\n");

    // Graph children nodes
    int type_node_id = type_graph(p->type);
    int symbol_node_id = symbol_graph(p->symbol);
    int next_node_id = param_list_graph(p->next);

    // Only print edges if a corresponding node exists
    if (type_node_id != -1)
        printf("\"param_list_%06d\":type -> \"type_%06d\";\n", node_id, type_node_id);
        
    if (symbol_node_id != -1)
        printf("\"param_list_%06d\":symbol -> \"symbol_%06d\";\n", node_id, symbol_node_id);

    if (next_node_id != -1)
        printf("\"param_list_%06d\":next -> \"param_list_%06d\";\n", node_id, next_node_id);

    return node_id;
}
