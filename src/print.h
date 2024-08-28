
#ifndef PRINT_H
#define PRINT_H

struct decl;
struct expr;
struct param_list;
struct stmt;
struct symbol;
struct type;

void expr_print(struct expr *e);

void stmt_print(struct stmt *s, int indent);

void decl_print(struct decl *d, int indent);

void param_list_print(struct param_list *p);

void symbol_print(struct symbol *s);

void type_print(struct type *t);

void param_list_print(struct param_list *p);

#endif