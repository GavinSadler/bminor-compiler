
#ifndef RESOLVE_H
#define RESOLVE_H

struct decl;
struct expr;
struct param_list;
struct stmt;
struct type;

void decl_resolve(struct decl *d);

void expr_resolve(struct expr *e);

void param_list_resolve(struct param_list *p);

void stmt_resolve(struct stmt *s);

void type_resolve(struct type *t);

#endif
