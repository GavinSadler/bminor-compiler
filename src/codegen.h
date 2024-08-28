
#ifndef SCRATCH_H
#define SCRATCH_H

struct symbol;
struct decl;
struct expr;
struct stmt;

int scratch_alloc();

void scratch_free(int r);

const char *scratch_name(int r);

extern int label_counter;

int label_create();

const char *label_name(int label);

const char *symbol_codegen(struct symbol *s);

const char *decl_codegen(struct decl *d);

const char *stmt_codegen(struct stmt *s);

const char *expr_codegen(struct expr *e);

#endif
