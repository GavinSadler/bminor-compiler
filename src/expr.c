
#include <stdio.h>
#include <stdlib.h>

#include "expr.h"

struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = kind;
    e->left = left;
    e->right = right;

    return e;
}

struct expr *expr_create_name(const char *n)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = EXPR_NAME;
    e->name = n;

    return e;
}

struct expr *expr_create_integer_literal(int c)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = EXPR_INTEGERLITERAL;
    e->literal_value = c;

    return e;
}

struct expr *expr_create_boolean_literal(int c)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = EXPR_BOOLEANLITERAL;
    e->literal_value = c;

    return e;
}

struct expr *expr_create_char_literal(char c)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = EXPR_CHARLITERAL;
    e->literal_value = c;

    return e;
}

struct expr *expr_create_string_literal(const char *str)
{
    struct expr *e = malloc(sizeof(struct expr));

    e->kind = EXPR_STRINGLITERAL;
    e->string_literal = str;

    return e;
}

void expr_print(struct expr *e)
{
    if (!e)
        return;

    switch (e->kind)
    {
    case EXPR_NAME:
        printf("%s", e->name);
        break;
    case EXPR_CHARLITERAL:
        printf("'%c'", (char)e->literal_value);
        break;
    case EXPR_STRINGLITERAL:
        printf("%s", e->string_literal);
        break;
    case EXPR_INTEGERLITERAL:
        printf("%d", e->literal_value);
        break;
    case EXPR_BOOLEANLITERAL:
        if (e->literal_value)
            printf("true");
        else
            printf("false");
        break;
    case EXPR_SUBSCRIPT:
        expr_print(e->left);
        printf("[");
        expr_print(e->right);
        printf("]");
        break;
    case EXPR_CALL:
        expr_print(e->left);
        printf("(");
        expr_print(e->right);
        printf(")");
        break;
    case EXPR_INC:
        expr_print(e->left);
        printf("++");
        break;
    case EXPR_DEC:
        expr_print(e->left);
        printf("--");
        break;
    case EXPR_NEGATE:
        printf("-");
        expr_print(e->left);
        break;
    case EXPR_NOT:
        printf("!");
        expr_print(e->left);
        break;
    case EXPR_POW:
        expr_print(e->left);
        printf(" ^ ");
        expr_print(e->right);
        break;
    case EXPR_MUL:
        expr_print(e->left);
        printf(" * ");
        expr_print(e->right);
        break;
    case EXPR_DIV:
        expr_print(e->left);
        printf(" / ");
        expr_print(e->right);
        break;
    case EXPR_MOD:
        expr_print(e->left);
        printf(" %% ");
        expr_print(e->right);
        break;
    case EXPR_ADD:
        expr_print(e->left);
        printf(" + ");
        expr_print(e->right);
        break;
    case EXPR_SUB:
        expr_print(e->left);
        printf(" - ");
        expr_print(e->right);
        break;
    case EXPR_LT:
        expr_print(e->left);
        printf(" < ");
        expr_print(e->right);
        break;
    case EXPR_LTE:
        expr_print(e->left);
        printf(" <= ");
        expr_print(e->right);
        break;
    case EXPR_GT:
        expr_print(e->left);
        printf(" > ");
        expr_print(e->right);
        break;
    case EXPR_GTE:
        expr_print(e->left);
        printf(" >= ");
        expr_print(e->right);
        break;
    case EXPR_EQUALITY:
        expr_print(e->left);
        printf(" == ");
        expr_print(e->right);
        break;
    case EXPR_NEQUALITY:
        expr_print(e->left);
        printf(" != ");
        expr_print(e->right);
        break;
    case EXPR_AND:
        expr_print(e->left);
        printf(" && ");
        expr_print(e->right);
        break;
    case EXPR_OR:
        expr_print(e->left);
        printf(" || ");
        expr_print(e->right);
        break;
    case EXPR_ASSIGNMENT:
        expr_print(e->left);
        printf(" = ");
        expr_print(e->right);
        break;
    case EXPR_ARG:
        expr_print(e->left);
        if (e->right)
        {
            printf(", ");
            expr_print(e->right);
        }
        break;
    case EXPR_INITIALIZER:
        printf("{ ");
        expr_print(e->left);
        printf(" }");
        if (e->right)
        {
            printf(", ");
            expr_print(e->right);
        }
        fflush(stdout);
        break;
    case EXPR_GROUP:
        printf("( ");
        expr_print(e->left);
        printf(" )");
        break;
    default:
        printf("\nERROR: No print case for expr of kind: %d\n", e->kind);
        exit(1);
        break;
    }
}
