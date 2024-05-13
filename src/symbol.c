
#include <stdlib.h>

#include "decl.h"
#include "hash_table.h"
#include "stack.h"
#include "symbol.h"
#include "type.h"

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name)
{
    struct symbol *s = malloc(sizeof(struct symbol));

    s->kind = kind;
    s->type = type;
    s->name = name;

    return s;
}

extern int graph_node_id_counter;

int symbol_graph(struct symbol *s)
{
    if (!s)
        return -1;

    int node_id = graph_node_id_counter;
    graph_node_id_counter++;

    // The definition of the node
    printf("\"symbol_%06d\"[\n", node_id);
    printf("\tlabel = \"{ symbol: ");

    switch (s->kind)
    {
    case SYMBOL_LOCAL:
        printf("local");
        break;
    case SYMBOL_GLOBAL:
        printf("global");
        break;
    case SYMBOL_PARAM:
        printf("param");
        break;
    default:
        printf("ERROR: Unknown enum symbol_t encountered when trying to graph symbol node: %d\n", s->kind);
        exit(1);
        break;
    }

    printf(" %s | { <type> type }}\"\n", s->name);
    printf("\tshape = \"record\"\n");
    printf("];\n\n");

    // Graph children nodes
    int type_node_id = type_graph(s->type);

    // Only print edges if a corresponding node exists
    if (type_node_id != -1)
        printf("\"symbol_%06d\":type -> \"type_%06d\";\n", node_id, type_node_id);

    return node_id;
}

// Keeps track of what part of the scope we're in
stack scope_stack;

void scope_initialize()
{
    if (!scope_stack)
    {
        scope_stack = stack_create();
        scope_enter();
    }
}

void scope_enter()
{
    struct hash_table *newTab = hash_table_create(0, 0);
    stack_push(scope_stack, newTab);
    // printf("Operating in scope %d\n", scope_level());
}

void scope_exit()
{
    struct hash_table *h = (struct hash_table *)stack_pop(scope_stack);
    hash_table_delete(h);
    // printf("Descending to scope %d\n", scope_level());
}

int scope_level()
{
    return stack_size(scope_stack);
}

void scope_bind(const char *name, struct symbol *sym)
{
    struct hash_table *h = (struct hash_table *)stack_peek(scope_stack);

    printf("Binding variable '%s' of type ", sym->name);
    type_print(sym->type);
    printf(" to scope level %d\n", scope_level());

    hash_table_insert(h, name, sym);
}

struct symbol *scope_lookup(const char *name)
{
    struct symbol *sym = NULL;

    for (int i = (stack_size(scope_stack) - 1); i >= 0; i--)
    {
        struct hash_table *ht = stack_item(scope_stack, i);
        sym = (struct symbol *)hash_table_lookup(ht, name);
        if (sym)
            break;
    }

    return sym;
}

struct symbol *scope_lookup_current(const char *name)
{
    struct hash_table *ht = stack_item(scope_stack, stack_size(scope_stack) - 1);
    struct symbol *sym = (struct symbol *)hash_table_lookup(ht, name);

    return sym;
}

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
