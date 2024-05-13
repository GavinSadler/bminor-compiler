
#include <stdlib.h>

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "type.h"

struct type *type_create(type_t kind, struct type *subtype, struct param_list *params)
{
    struct type *t = malloc(sizeof(struct type));

    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    t->size = 0;

    return t;
}

void type_print(struct type *t)
{
    if (!t)
        return;

    switch (t->kind)
    {
    case TYPE_VOID:
        printf("void");
        break;
    case TYPE_BOOLEAN:
        printf("boolean");
        break;
    case TYPE_CHARACTER:
        printf("char");
        break;
    case TYPE_INTEGER:
        printf("integer");
        break;
    case TYPE_STRING:
        printf("string");
        break;
    case TYPE_ARRAY:
        if (!t->size)
            printf("array []");
        else
            printf("array [%d]", t->size);

        if (t->subtype)
        {
            printf(" ");
            type_print(t->subtype);
        }
        break;
    case TYPE_FUNCTION:
        printf("function ");
        type_print(t->subtype);
        printf(" ( ");
        param_list_print(t->params);
        printf(" )");
        break;
    default:
        printf("\nERROR: No print case for type of kind: %d\n", t->kind);
        exit(1);
        break;
    }
}

extern int graph_node_id_counter;

int type_graph(struct type *t)
{
    if (!t)
        return -1;
    
    int node_id = graph_node_id_counter;
    graph_node_id_counter++;

    // The definition of the node
    printf("\"type_%06d\"[\n", node_id);
    printf("\tlabel = \"{ type: ");

    switch (t->kind)
    {
    case TYPE_VOID:
        printf("void");
        break;
    case TYPE_BOOLEAN:
        printf("boolean");
        break;
    case TYPE_CHARACTER:
        printf("char");
        break;
    case TYPE_INTEGER:
        printf("integer");
        break;
    case TYPE_STRING:
        printf("string");
        break;
    case TYPE_ARRAY:
        printf("array");
        break;
    case TYPE_FUNCTION:
        printf("function");
        break;
    default:
        printf("ERROR: Unknown enum type_t encountered when trying to graph type node: %d\n", t->kind);
        exit(1);
        break;
    }

    printf(" | { <params> params | <subtype> subtype }}\"\n");
    printf("\tshape = \"record\"\n");
    printf("];\n\n");

    // Graph children nodes
    int params_node_id = param_list_graph(t->params);
    int subtype_node_id = type_graph(t->subtype);

    // Only print edges if a corresponding node exists
    if (params_node_id != -1)
        printf("\"type_%06d\":params -> \"param_list_%06d\";\n", node_id, params_node_id);
        
    if (subtype_node_id != -1)
        printf("\"type_%06d\":subtype -> \"type_%06d\";\n", node_id, subtype_node_id);

    return node_id;
}

bool type_equals(struct type *a, struct type *b)
{
    // Null check
    if (!a && !b)
        return true;

    // If either or are null...
    if (!a || !b)
        return false;

    if (a->kind == b->kind)
    {
        // Since array and function types are 5 and 6 as integers,
        // we can simply check to see if they are less than 5
        if (a->kind < 5)
        {
            return true;
        }
        else if (a->kind == TYPE_ARRAY)
        {
            return type_equals(a->subtype, b->subtype);
        }
        else if (a->kind == TYPE_FUNCTION)
        {
            // Functions use the subtype field as their returnt type
            return type_equals(a->subtype, b->subtype) && param_list_equals(a->params, b->params);
        }
    }

    return false;
}

struct type *type_copy(struct type *t)
{
    if (!t)
        return NULL;

    struct type *t_copy = malloc(sizeof(struct type));

    t_copy->kind = t->kind;
    t_copy->size = t->size;
    t_copy->params = param_list_copy(t->params);
    t_copy->subtype = type_copy(t->subtype);

    return t_copy;
}

void type_delete(struct type *t)
{
    if (!t)
        return;

    type_delete(t->subtype);
    t->subtype = NULL;

    param_list_delete(t->params);
    t->params = NULL;

    t->kind = 0;
    t->size = 0;

    free(t);
    t = NULL;
}

// === Typechecking functions ===

// Global typechecking variable that gets set to false if a typechecking error was found
bool typecheck_succeeded = true;

void decl_typecheck(struct decl *d)
{
    if (!d)
        return;

    // For decls with declared values
    if (d->value)
    {
        // Evaluate the expression's type...
        struct type *t = expr_typecheck(d->value);

        // If it doesn't match the declared type
        if (!type_equals(t, d->symbol->type))
        {
            printf("ERROR: Symbol declaration for '%s' does not match expression's evaluated type\n", d->name);
            printf("\tExpected type '");
            type_print(d->type);
            printf("', epression type evaluated as '");
            type_print(t);
            printf("'\n");

            typecheck_succeeded = false;
        }
    }

    // Function parameter verification
    if (d->type->kind == TYPE_FUNCTION)
    {
        struct symbol *s = scope_lookup(d->name);

        // In the case that there exists a function prototype already...
        if (s)
        {
            // Make sure the parameters match
            if (!param_list_equals(d->type->params, s->type->params))
            {
                printf("ERROR: Function declaration for '%s' does not match prototype's parameter list\n", d->name);
                printf("\tExpected parameters '");
                param_list_print(s->type->params);
                printf("', declaration parameters are '");
                param_list_print(d->type->params);
                printf("'\n");

                typecheck_succeeded = false;
            }

            // Make sure return types match
            if (!type_equals(d->type->subtype, s->type->subtype))
            {
                printf("ERROR: Function declaration for '%s' does not match prototype's return type\n", d->name);
                printf("\tExpected return type '");
                type_print(s->type->subtype);
                printf("', declaration return type is '");
                type_print(d->type->subtype);
                printf("'\n");

                typecheck_succeeded = false;
            }
        }
    }

    // Function body verification
    if (d->code)
    {
        struct type *t = stmt_typecheck(d->code);

        // In the event that a stmt_typecheck returns null, this means the function had no return type
        // This is equivalent to a type of void
        if (!t)
            t = type_create(TYPE_VOID, 0, 0);

        if (!type_equals(t, d->type->subtype))
        {
            printf("ERROR: Declaration for function '%s' returns the wrong type of variable\n", d->name);
            printf("\tExpected type '");
            type_print(d->type->subtype);
            printf("', return expression type evaluated as '");
            type_print(t);
            printf("'\n");
            typecheck_succeeded = false;
        }

        type_delete(t);
    }

    decl_typecheck(d->next);
}

// Note: this function will only return a type when a return statement is encountered
struct type *stmt_typecheck(struct stmt *s)
{
    if (!s)
        return NULL;

    struct type *t;
    struct type *result;

    switch (s->kind)
    {
    case STMT_DECL:
        decl_typecheck(s->decl);
        break;
    case STMT_EXPR:
        expr_typecheck(s->expr);
        break;
    case STMT_IF:
        t = expr_typecheck(s->expr);
        if (t->kind != TYPE_BOOLEAN)
        {
            printf("ERROR: if statment expression should be of type boolean, but expression type evaluated to type '");
            type_print(t);
            printf("'\n");
            typecheck_succeeded = false;
        }
        stmt_typecheck(s->body);
        stmt_typecheck(s->else_body);
        break;
    case STMT_FOR:
        expr_typecheck(s->init_expr);
        expr_typecheck(s->expr);
        expr_typecheck(s->next_expr);
        stmt_typecheck(s->body);
        break;
    case STMT_PRINT:
        t = expr_typecheck(s->expr);
        type_delete(t);
        break;
    case STMT_RETURN:
        return expr_typecheck(s->expr);
        break;
    case STMT_BLOCKSTART:
        break;
    case STMT_BLOCKEND:
        break;
    default:
        printf("ERROR: Unknown statement type encountered, %d\n", s->kind);
        break;
    }

    return stmt_typecheck(s->next);
}

// Note: This function will always return a new type struct on the heap
//       The returned struct type will need to be destroyed after use
struct type *expr_typecheck(struct expr *e)
{
    if (!e)
        return 0;

    struct type *lt = expr_typecheck(e->left);
    struct type *rt = expr_typecheck(e->right);
    struct type *result;

    switch (e->kind)
    {
        // Atomic types
    case EXPR_INTEGERLITERAL:
        result = type_create(TYPE_INTEGER, 0, 0);
        break;
    case EXPR_STRINGLITERAL:
        result = type_create(TYPE_STRING, 0, 0);
        break;
    case EXPR_CHARLITERAL:
        result = type_create(TYPE_CHARACTER, 0, 0);
        break;
    case EXPR_BOOLEANLITERAL:
        result = type_create(TYPE_BOOLEAN, 0, 0);
        break;

        // Named symbol
    case EXPR_NAME:
        // See if this symbol is valid in our scope
        // struct symbol *sym = scope_lookup(e->symbol->name);
        // if (!sym)
        // {
        //     printf("ERROR: Symbol '%s' referenced, but does not exist in this scope\n");
        //     printf("\tTypes were '");
        //     type_print(lt);
        //     printf("', and '");
        //     type_print(rt);
        //     printf("'\n");
        //     typecheck_succeeded = false;
        // }
        result = type_copy(e->symbol->type);
        break;

        // Grouped expressions
        // (something)
    case EXPR_GROUP:
        // Nothing to check here, just return whatever the inner expression is
        result = type_copy(lt);
        break;

        // Argument lists
    case EXPR_ARG:
        struct param_list *p = param_list_create(e->left->name, type_copy(lt), 0);

        if (rt)
            p->next = param_list_copy(rt->params);

        result = type_create(TYPE_VOID, 0, p);
        break;

    case EXPR_INITIALIZER:
        // TODO: Finish this one, might be complex
        type_create(TYPE_VOID, 0, 0);
        break;

        // Function calls
        // f(argument list)
    case EXPR_CALL:
        if (lt->kind != TYPE_FUNCTION)
        {
            printf("ERROR: Attempted to call a non-function symbol\n");
            printf("\tTypes were '");
            type_print(lt);
            printf("', and '");
            type_print(rt);
            printf("'\n");
            typecheck_succeeded = false;
            // As a failsafe, just copy whatever the type of the left item
            result = type_copy(lt);
        }
        else
        {
            // In the case of no params, lt->params should be null and rt should be null
            if (!(!lt->params && !rt) && !param_list_equals(lt->params, rt->params))
            {
                printf("ERROR: Called function '%s' with incompatible argument types\n", e->left->name);
                printf("\tArgument types expected, '");
                param_list_print(lt->params);
                printf("', argument types recieved, '");
                param_list_print(rt->params);
                printf("'\n");
                typecheck_succeeded = false;
            }
            result = type_copy(lt->subtype);
        }
        break;

        // Increment, decrement
        // ++, --
    case EXPR_INC:
    case EXPR_DEC:
        result = type_copy(lt);
        if (lt->kind != TYPE_INTEGER || lt->kind != TYPE_CHARACTER)
        {
            printf("ERROR: Attempted to ++ or -- a non-integer or non-character\n");
            printf("\tType was '");
            type_print(lt);
            printf("'\n");
            typecheck_succeeded = false;

            // In the case that we fail, just return an integer to typecheck the rest of the program
            type_delete(result);
            result = type_create(TYPE_INTEGER, 0, 0);
        }
        break;

        // Negate
        // -1, -x, -f(a, b, c) ...
    case EXPR_NEGATE:
        if (lt->kind != TYPE_INTEGER)
        {
            printf("ERROR: Attempted to negate a non-interger expression\n");
            printf("\tType was '");
            type_print(lt);
            printf("'\n");
            typecheck_succeeded = false;
        }
        result = type_create(TYPE_INTEGER, 0, 0);
        break;

        // Not operator, boolean flip
        // !true, !(A == B), !x ...
    case EXPR_NOT:
        if (lt->kind != TYPE_INTEGER)
        {
            printf("ERROR: Attempted to NOT (!) a non-boolean expression\n");
            printf("\tType was '");
            type_print(lt);
            printf("'\n");
            typecheck_succeeded = false;
        }
        result = type_create(TYPE_BOOLEAN, 0, 0);
        break;

        // Arithmetic
        // + - / * % ^
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_DIV:
    case EXPR_MUL:
    case EXPR_POW:
    case EXPR_MOD:
        if (lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER)
        {
            printf("ERROR: Attempted arithmetic on two non-integer expressions\n");
            printf("\tTypes were '");
            type_print(lt);
            printf("', and '");
            type_print(rt);
            printf("'\n");
            typecheck_succeeded = false;
        }
        result = type_create(TYPE_INTEGER, 0, 0);
        break;

        // Comparisons
        // =, !=, <, <=, >, >=
    case EXPR_EQUALITY:
    case EXPR_NEQUALITY:
    case EXPR_LT:
    case EXPR_LTE:
    case EXPR_GT:
    case EXPR_GTE:
        // Make sure the compared types are comparable types
        if (lt->kind != TYPE_BOOLEAN && lt->kind != TYPE_CHARACTER && lt->kind != TYPE_INTEGER &&
            rt->kind != TYPE_BOOLEAN && rt->kind != TYPE_CHARACTER && rt->kind != TYPE_INTEGER)
        {
            printf("ERROR: Attempted to compare an expression of an uncomparable type\n");
            printf("\tTypes were '");
            type_print(lt);
            printf("', and '");
            type_print(rt);
            printf("'\n");
            typecheck_succeeded = false;
        }
        // Check to make sure types are the same
        if (!type_equals(lt, rt))
        {
            printf("ERROR: Attempted comparison of two expressions of different types\n");
            printf("\tTypes were '");
            type_print(lt);
            printf("', and '");
            type_print(rt);
            printf("'\n");
            typecheck_succeeded = false;
        }
        result = type_create(TYPE_BOOLEAN, 0, 0);
        break;

        // && and || comparisons
    case EXPR_AND:
    case EXPR_OR:
        if (lt->kind != TYPE_BOOLEAN || rt->kind != TYPE_BOOLEAN)
        {
            printf("ERROR: Attempted && or || comparison of non-boolean typed expressions\n");
            printf("\tTypes were '");
            type_print(lt);
            printf("', and '");
            type_print(rt);
            printf("'\n");
            typecheck_succeeded = false;
        }
        result = type_create(TYPE_BOOLEAN, 0, 0);
        break;

        // named symbol = some expression
    case EXPR_ASSIGNMENT:
        if (!type_equals(lt, rt))
        {
            printf("ERROR: Asignee type not in agreement with assignment type\n");
            printf("\tTypes were '");
            type_print(lt);
            printf("', and '");
            type_print(rt);
            printf("'\n");
            typecheck_succeeded = false;
        }
        result = type_copy(rt);
        break;

        // a[index]
    case EXPR_SUBSCRIPT:
        if (lt->kind == TYPE_ARRAY)
        {
            if (rt->kind != TYPE_INTEGER)
            {
                printf("ERROR: Array index is a non-integer value\n");
                printf("\tIndex is of type '");
                type_print(rt);
                printf("'\n");
                typecheck_succeeded = false;
            }
            result = type_copy(lt->subtype);
        }
        else
        {
            printf("ERROR: Attemp to index non-array expression\n");
            printf("\tTypes were '");
            type_print(lt);
            printf("', and '");
            type_print(rt);
            printf("'\n");
            typecheck_succeeded = false;
            result = type_copy(lt);
        }
        break;
    }

    // Clean up copied types from left and right expressions
    type_delete(lt);
    type_delete(rt);

    return result;
}
