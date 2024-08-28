

#include "scope.h"
#include "hash_table.h"
#include "stack.h"
#include <stdlib.h>

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

    // printf("Binding variable '%s' of type ", sym->name);
    // type_print(sym->type);
    // printf(" to scope level %d\n", scope_level());

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
