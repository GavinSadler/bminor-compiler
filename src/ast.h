
#ifndef AST_H
#define AST_H

#include <stdbool.h>

struct expr;
struct stmt;
struct type;
// #include <stdlib.h>

// === decl ===

struct decl
{
    char *name;
    struct type *type;
    struct expr *value;
    struct stmt *code;
    struct symbol *symbol;
    struct decl *next;
};

struct decl *decl_create(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next);

// === expr ===

typedef enum
{
    // Atomics
    EXPR_NAME,
    EXPR_CHARLITERAL,
    EXPR_STRINGLITERAL,
    EXPR_INTEGERLITERAL,
    EXPR_BOOLEANLITERAL,

    EXPR_GROUP,
    EXPR_ARG,
    EXPR_INITIALIZER,
    EXPR_SUBSCRIPT,
    EXPR_CALL,
    EXPR_INC,
    EXPR_DEC,
    EXPR_NEGATE,
    EXPR_NOT,
    EXPR_POW,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_MOD,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_LT,
    EXPR_LTE,
    EXPR_GT,
    EXPR_GTE,
    EXPR_EQUALITY,
    EXPR_NEQUALITY,
    EXPR_AND,
    EXPR_OR,
    EXPR_ASSIGNMENT
} expr_t;

struct expr
{
    /* used by all kinds of exprs */
    expr_t kind;
    struct expr *left;
    struct expr *right;

    /* used by various leaf exprs */
    const char *name;
    int literal_value;
    const char *string_literal;
    struct symbol *symbol;
};

struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right);

struct expr *expr_create_name(const char *n);

struct expr *expr_create_integer_literal(int c);

struct expr *expr_create_boolean_literal(int c);

struct expr *expr_create_char_literal(char c);

struct expr *expr_create_string_literal(const char *str);

// == param_list ===

struct param_list
{
    const char *name;
    struct type *type;
    struct symbol *symbol;
    struct param_list *next;
};

struct param_list *param_list_create(const char *name, struct type *type, struct param_list *next);

struct param_list *param_list_copy(struct param_list *p);

bool param_list_equals(struct param_list *a, struct param_list *b);

void param_list_delete(struct param_list *p);

// === stmt ===

typedef enum
{
    STMT_DECL,
    STMT_EXPR,
    STMT_IF,
    STMT_FOR,
    STMT_PRINT,
    STMT_RETURN,
    STMT_BLOCKSTART,
    STMT_BLOCKEND
} stmt_t;

struct stmt
{
    stmt_t kind;
    struct decl *decl;
    struct expr *init_expr;
    struct expr *expr;
    struct expr *next_expr;
    struct stmt *body;
    struct stmt *else_body;
    struct stmt *next;
};

struct stmt *stmt_create(stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr,
                         struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next);

// === type ===

typedef enum
{
    TYPE_VOID,
    TYPE_BOOLEAN,
    TYPE_CHARACTER,
    TYPE_INTEGER,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_FUNCTION,
} type_t;

struct type
{
    type_t kind;
    struct param_list *params;
    struct type *subtype;

    // If we have an array type, the size might come into play
    unsigned int size;
};

struct type *type_create(type_t kind, struct type *subtype, struct param_list *params);

bool type_equals(struct type *a, struct type *b);

struct type *type_copy(struct type *t);

void type_delete(struct type *t);

#endif
