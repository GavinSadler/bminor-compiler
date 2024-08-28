
#ifndef TYPECHECK_H
#define TYPECHECK_H

struct decl;
struct expr;
struct stmt;

void decl_typecheck(struct decl *d);

void stmt_typecheck(struct stmt *s);

struct type *expr_typecheck(struct expr *e);

#endif
