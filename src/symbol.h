
#ifndef SYMBOL_H
#define SYMBOL_H

struct type;

#define SYMBOLS                                                                                                        \
    X(SYMBOL_LOCAL)                                                                                                    \
    X(SYMBOL_PARAM)                                                                                                    \
    X(SYMBOL_GLOBAL)

typedef enum
{
#define X(symbol) symbol,
    SYMBOLS
#undef X
} symbol_t;

struct symbol
{
    symbol_t kind;
    struct type *type;
    const char *name;
    int which;
};

struct symbol *symbol_create(symbol_t kind, struct type *type, const char *name);

#endif
