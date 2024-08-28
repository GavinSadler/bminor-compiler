
#include <stdio.h>
#include <stdlib.h>

#include "print.h"

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "stmt.h"
#include "type.h"

// Quick macro for doing indentation
#define INDENT(n)                                                                                                      \
    for (int i = 0; i < n; i++)                                                                                        \
        printf(" ");

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

void param_list_print(struct param_list *p)
{
    if (!p)
        return;

    printf("%s: ", p->name);
    type_print(p->type);

    if (p->next)
    {
        printf(", ");
        param_list_print(p->next);
    }
}
