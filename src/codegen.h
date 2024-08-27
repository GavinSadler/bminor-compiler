
#ifndef SCRATCH_H
#define SCRATCH_H

#include "symbol.h"

int scratch_alloc();

void scratch_free(int r);

const char *scratch_name(int r);

extern int g_label_counter;

int label_create();

const char *label_name(int label);

const char *symbol_codegen(struct symbol *s);

#endif
