
#include "graph.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "symbol.h"

static int graph_node_id_counter = 0;

void ast_graph(struct decl *ast)
{
    printf("digraph {\n\n");
    printf("node[style=\"filled\", fontname = \"Helvetica,Arial,sans-serif\"]\n\n");

    decl_graph(ast);

    printf("\n}\n");
}

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

int param_list_graph(struct param_list *p)
{
    if (!p)
        return -1;

    int node_id = graph_node_id_counter;
    graph_node_id_counter++;

    // The definition of the node
    printf("\"param_list_%06d\"[\n", node_id);
    printf("\tlabel = \"{ parameter: %s | { <type> type | <symbol> symbol | <next> next }}\"\n", p->name);
    printf("\tfillcolor = \"lightyellow\"\n");
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
        printf("\"param_list_%06d\":symbol -> \"symbol_%06d\" [style=\"dashed\"];\n", node_id, symbol_node_id);

    if (next_node_id != -1)
        printf("\"param_list_%06d\":next -> \"param_list_%06d\";\n", node_id, next_node_id);

    return node_id;
}

int symbol_graph(struct symbol *s)
{
    if (!s)
        return -1;

    // int node_id = graph_node_id_counter;
    // graph_node_id_counter++;
    int node_id = (intptr_t)s;

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
    printf("\tfillcolor = \"lightpink\"\n");
    printf("];\n\n");

    // Graph children nodes
    int type_node_id = type_graph(s->type);

    // Only print edges if a corresponding node exists
    if (type_node_id != -1)
        printf("\"symbol_%06d\":type -> \"type_%06d\";\n", node_id, type_node_id);

    return node_id;
}

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

    printf(" | { <params> params | <subtype> subtype | size: %d }}\"\n", t->size);
    printf("\tfillcolor = \"lightyellow\"\n");
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
