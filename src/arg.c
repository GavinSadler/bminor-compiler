
#include <argp.h>
#include <stdbool.h>
#include <stddef.h>

#include "arg.h"

// Used by main to communicate with parse_opt
struct arguments input_arguments = {"", "a.out", false, false, false, false, false, false};

// The options we understand
static struct argp_option options[] = {
    {"scan", 's', 0, 0, "Validates that the file scans correctly", 0},
    {"parse", 'p', 0, 0, "Validates that the input file parses correctly", 0},
    {"typecheck", 't', 0, 0, "Validates that the input file typechecks correctly", 0},
    {"format", 'f', 0, 0, "Outputs a formatted version of the input source", 1},
    {"graph", 'g', 0, 0, "Outputs a Graphviz .dot file of the AST of the input source", 1},
    {"output", 'o', "FILE", 0, "Output to FILE instead of standard output", 2},
    {"verbose", 'v', 0, 0, "Produce verbose output", 2},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    // Get the input argument from argp_parse, which we know is a pointer to our
    // arguments structure.
    struct arguments *arguments = (struct arguments *)state->input;

    switch (key)
    {
    case 'v':
        arguments->verbose = true;
        break;
    case 's':
        arguments->scan = true;
        break;
    case 'p':
        arguments->parse = true;
        break;
    case 'g':
        arguments->graph = true;
        break;
    case 't':
        arguments->typecheck = true;
        break;
    case 'f':
        arguments->format = true;
        break;
    case 'o':
        arguments->output_file = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 1)
            argp_usage(state); // Too many arguments
        arguments->input_file = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 1)
            argp_usage(state); // Not enough arguments
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

// The arg parser
static struct argp argp = {options, parse_opt, "INPUT_SOURCE", NULL, NULL, NULL, NULL};

// Helper function so we don't have to jugle as many input arguments
void parse_input_arguments(int argc, char *argv[])
{
    argp_parse(&argp, argc, argv, 0, 0, &input_arguments);
}
