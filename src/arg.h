
#ifndef ARG_H
#define ARG_H

#include <stdbool.h>

// Used by main to communicate with parse_opt
struct arguments
{
    char *input_file;
    char *output_file;
    bool verbose;
    bool scan;
    bool parse;
    bool format;
    bool graph;
    bool typecheck;
};

extern struct arguments input_arguments;

void parse_input_arguments(int argc, char *argv[]);

#endif
