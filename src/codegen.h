
#ifndef SCRATCH_H
#define SCRATCH_H

struct symbol;
struct decl;
struct expr;
struct stmt;

// ========================
// Scratch register control
// ========================

int scratch_alloc();
void scratch_free(int r);
const char *scratch_name(int r);

// ===================
// Labelling functions
// ===================

int label_create();
const char *label_name(int label);

// ===============
// Code generation
// ===============

const char *symbol_codegen(struct symbol *s);
const char *decl_codegen(struct decl *d);
void stmt_codegen(struct stmt *s);
void expr_codegen(struct expr *e);

#endif
