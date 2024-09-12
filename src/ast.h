
#ifndef AST_H
#define AST_H

#include <stdbool.h>

struct expr;
struct stmt;
struct type;

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

#define EXPRS                \
    X(EXPR_NAME)            \
    X(EXPR_CHARLITERAL)     \
    X(EXPR_STRINGLITERAL)   \
    X(EXPR_INTEGERLITERAL)  \
    X(EXPR_BOOLEANLITERAL)  \
    X(EXPR_GROUP)           \
    X(EXPR_ARG)             \
    X(EXPR_INITIALIZER)    \
    X(EXPR_SUBSCRIPT)  \
    X(EXPR_CALL)   \
    X(EXPR_INC)    \
    X(EXPR_DEC)    \
    X(EXPR_NEGATE) \
    X(EXPR_NOT)    \
    X(EXPR_ADD)    \
    X(EXPR_SUB)    \
    X(EXPR_MUL)    \
    X(EXPR_DIV)    \
    X(EXPR_MOD)    \
    X(EXPR_POW)    \
    X(EXPR_LT) \
    X(EXPR_LTE)    \
    X(EXPR_GT) \
    X(EXPR_GTE)    \
    X(EXPR_EQUALITY)   \
    X(EXPR_NEQUALITY)  \
    X(EXPR_AND)    \
    X(EXPR_OR) \
    X(EXPR_ASSIGNMENT)

typedef enum{
    #define X(e) e,
    EXPRS
    #undef X
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

    // Used during code generation
    int reg;
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

#define STMTS       \
    X(STMT_DECL)    \
    X(STMT_EXPR)    \
    X(STMT_IF)  \
    X(STMT_FOR) \
    X(STMT_PRINT)   \
    X(STMT_RETURN)  \
    X(STMT_BLOCKSTART)  \
    X(STMT_BLOCKEND)

typedef enum
{
    #define X(stmt) stmt,
    STMTS
    #undef X
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

#define TYPES   \
    X(TYPE_VOID)    \
    X(TYPE_BOOLEAN) \
    X(TYPE_CHARACTER)   \
    X(TYPE_INTEGER) \
    X(TYPE_STRING)  \
    X(TYPE_ARRAY)   \
    X(TYPE_FUNCTION)

typedef enum
{
    #define X(type) type,
    TYPES
    #undef X
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
