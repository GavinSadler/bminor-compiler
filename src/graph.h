
#ifndef GRAPH_H
#define GRAPH_H

struct decl;
struct expr;
struct param_list;
struct stmt;
struct symbol;
struct type;

int expr_graph(struct expr *e);

int stmt_graph(struct stmt *s);

int decl_graph(struct decl *d);

int param_list_graph(struct param_list *p);

int symbol_graph(struct symbol *s);

int type_graph(struct type *t);

#endif
