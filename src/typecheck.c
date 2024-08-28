
#include "typecheck.h"

#include <stdbool.h>
#include <stdio.h>

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "print.h"
#include "scope.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

// Global typechecking variable that gets set to false if a typechecking error was found
bool typecheck_succeeded = true;

// A function used to make sure that array literals fit into the container
bool array_fits(struct symbol *storage, struct type *value)
{
    struct type *storage_type = storage->type;
    struct type *value_type = value;

    while (storage_type != NULL && storage_type->kind == TYPE_ARRAY)
    {
        printf("Storage size: %d\tValue size: %d\n", storage_type->size, value_type->size);
        if (storage_type->size != 0 && storage_type->size < value_type->size)
            return false;

        storage_type = storage_type->subtype;
        value_type = value_type->subtype;
    }

    return true;
}

// Global variable that we use to verify the return type of functions
struct type *function_return_type;

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

        printf("Symbol name: %s\n", d->symbol->name);
        printf("Value type: ");
        type_print(t);
        printf("\n");

        // If the declared type is an array, we need to make sure the declared variable has enough space for the
        // if (!array_fits(d->symbol, t))
        // {
        //     printf("ERROR: Symbol declaration for '%s' does not fufill storage required by literal assignment\n",
        //     d->name); typecheck_succeeded = false;
        // }
    }

    // Function parameter and return type verification
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

        // Make sure to mark the funciton return type
        // This will be compared to return value's return types
        function_return_type = d->type->subtype;
    }

    // Function body verification
    if (d->code)
        stmt_typecheck(d->code);

    decl_typecheck(d->next);
}

void stmt_typecheck(struct stmt *s)
{
    if (!s)
        return;

    struct type *t;

    switch (s->kind)
    {
    case STMT_DECL:
        decl_typecheck(s->decl);
        break;
    case STMT_EXPR:
        t = expr_typecheck(s->expr);
        type_delete(t);
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

        type_delete(t);

        stmt_typecheck(s->body);
        stmt_typecheck(s->else_body);
        break;
    case STMT_FOR:
        t = expr_typecheck(s->init_expr);
        type_delete(t);
        t = expr_typecheck(s->expr);
        type_delete(t);
        t = expr_typecheck(s->next_expr);
        type_delete(t);
        stmt_typecheck(s->body);
        break;
    case STMT_PRINT:
        t = expr_typecheck(s->expr);
        type_delete(t);
        break;
    case STMT_RETURN:
        t = expr_typecheck(s->expr);

        // Verify that return statement's type matches the global function return type variable
        if (!type_equals(t, function_return_type))
        {
            printf("ERROR: return statment expression should be of type '");
            type_print(function_return_type);
            printf("', but expression type evaluated to type '");
            type_print(t);
            printf("'\n");
            typecheck_succeeded = false;
        }

        type_delete(t);
        break;
    case STMT_BLOCKSTART:
        break;
    case STMT_BLOCKEND:
        break;
    default:
        printf("ERROR: Unknown statement type encountered, %d\n", s->kind);
        break;
    }

    stmt_typecheck(s->next);
}

// Note: This function will always return a new type struct on the heap
//       The returned struct type will need to be destroyed after use
struct type *expr_typecheck(struct expr *e)
{
    if (!e)
        return 0;

    struct type *lt = expr_typecheck(e->left);
    struct type *rt = expr_typecheck(e->right);
    struct type *result = NULL;

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
    case EXPR_ARG: {
        struct param_list *p = param_list_create(e->left->name, type_copy(lt), 0);

        if (rt)
            p->next = param_list_copy(rt->params);

        result = type_create(TYPE_VOID, 0, p);
        break;
    }

    case EXPR_INITIALIZER: {

        // Since initializers are just a wrapper around arg chains, we just have
        // to check that the types of the param lists all match, and that if
        // there is a number associated with the size of the array, we have the
        // correct number of elements

        struct param_list *pl = lt->params;
        struct type *subtype = pl->type;

        // An array literal cannot be empty, there must be at least 1 element
        if (!pl || !lt)
        {
            printf("ERROR: array literal has no elements\n");
            typecheck_succeeded = false;
            return type_create(TYPE_ARRAY, 0, 0);
        }

        // Ensure all types from the array correspond to one another
        int size = 1;

        while (pl->next != NULL)
        {
            if (!type_equals(subtype, pl->next->type))
            {
                printf("ERROR: array literal has conflicting types. Encountered type'");
                type_print(subtype);
                printf("' beside type '");
                type_print(pl->next->type);
                printf("'.\n");

                typecheck_succeeded = false;
                break;
            }

            size++;
            pl = pl->next;
        }

        // Also encode size infoermation for the array literal. This will be
        // used to ensure that the array literal's size corresponds to the size
        // of its declaration
        result = type_create(TYPE_ARRAY, subtype, 0);
        result->size = size;
        break;
    }

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

        if (!(lt->kind == TYPE_INTEGER || lt->kind == TYPE_CHARACTER))
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
