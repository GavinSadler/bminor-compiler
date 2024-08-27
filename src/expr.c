
#include <stdio.h>
#include <stdlib.h>

#include "expr.h"
#include "symbol.h"

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

extern int graph_node_id_counter;

int expr_graph(struct expr *e)
{
    if (!e)
        return -1;

    int node_id = graph_node_id_counter;
    graph_node_id_counter++;

    // The definition of the node
    printf("\"expr_%06d\"[\n", node_id);
    printf("\tlabel = \"{ expr: ");

    switch (e->kind)
    {
    case EXPR_NAME:
        printf("name");
        break;
    case EXPR_CHARLITERAL:
        printf("char literal");
        break;
    case EXPR_STRINGLITERAL:
        printf("string literal");
        break;
    case EXPR_INTEGERLITERAL:
        printf("integer literal");
        break;
    case EXPR_BOOLEANLITERAL:
        printf("boolean literal");
        break;
    case EXPR_GROUP:
        printf("group");
        break;
    case EXPR_ARG:
        printf("arg");
        break;
    case EXPR_INITIALIZER:
        printf("initializer");
        break;
    case EXPR_SUBSCRIPT:
        printf("subscript");
        break;
    case EXPR_CALL:
        printf("call");
        break;
    case EXPR_INC:
        printf("inc");
        break;
    case EXPR_DEC:
        printf("dec");
        break;
    case EXPR_NEGATE:
        printf("negate");
        break;
    case EXPR_NOT:
        printf("not");
        break;
    case EXPR_POW:
        printf("pow");
        break;
    case EXPR_MUL:
        printf("mul");
        break;
    case EXPR_DIV:
        printf("div");
        break;
    case EXPR_MOD:
        printf("mod");
        break;
    case EXPR_ADD:
        printf("add");
        break;
    case EXPR_SUB:
        printf("sub");
        break;
    case EXPR_LT:
        printf("lt");
        break;
    case EXPR_LTE:
        printf("lte");
        break;
    case EXPR_GT:
        printf("gt");
        break;
    case EXPR_GTE:
        printf("gte");
        break;
    case EXPR_EQUALITY:
        printf("equality");
        break;
    case EXPR_NEQUALITY:
        printf("nequality");
        break;
    case EXPR_AND:
        printf("and");
        break;
    case EXPR_OR:
        printf("or");
        break;
    case EXPR_ASSIGNMENT:
        printf("assignment");
        break;
    default:
        printf("ERROR: Unknown enum expr_t encountered when trying to graph expr node: %d\n", e->kind);
        exit(1);
        break;
    }

    printf(
        " | { <left> left | <right> right | <name> name: %s | <literal_value> literal_value: %d | <string_literal> string_literal: '%s' | <symbol> symbol }}\"\n",
        e->name, e->literal_value, e->string_literal);
    printf("\tfillcolor = \"lightblue\"\n");
    printf("\tshape = \"record\"\n");
    printf("];\n\n");

    // Graph children nodes
    int left_node_id = expr_graph(e->left);
    int right_node_id = expr_graph(e->right);
    int symbol_node_id = symbol_graph(e->symbol);

    // Only print edges if a corresponding node exists
    if (left_node_id != -1)
        printf("\"expr_%06d\":left -> \"expr_%06d\";\n", node_id, left_node_id);

    if (right_node_id != -1)
        printf("\"expr_%06d\":right -> \"expr_%06d\";\n", node_id, right_node_id);

    if (symbol_node_id != -1)
        printf("\"expr_%06d\":symbol -> \"symbol_%06d\" [style=\"dashed\"];\n", node_id, symbol_node_id);

    return node_id;
}
