
#ifndef EXPR_H
#define EXPR_H

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

#endif
