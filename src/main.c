
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "arg.h"
#include "graph.h"
#include "print.h"
#include "resolve.h"
#include "scope.h"
#include "token.h"
#include "typecheck.h"

// Scanner internals
extern size_t MAX_TOKEN_LENGTH;
extern FILE *yyin;
extern int yylex();
extern char *yytext;

// Parser externals
extern int yyparse();
extern struct decl *parser_result;
extern const char *token_name(enum yytokentype t);

// Typechecking externals
extern bool typecheck_succeeded;

int main(int argc, char *argv[])
{
    parse_input_arguments(argc, argv);

    yyin = fopen(argv[2], "r");
    if (!yyin)
    {
        printf("ERROR: Could not open file %s\n", argv[2]);
        return 1;
    }

    // Run the scanner in isolation, if requested
    if (input_arguments.scan)
    {
        while (1)
        {
            enum yytokentype t = yylex();

            printf("=== VALIDATING SCAN ===");

            printf("token: %2d\t%-24s\ttext: %s\n", t, token_name(t), yytext);

            if (t == 0) // End of file
                break;

            if (t == TOKEN_ERROR)
            {
                printf("ERROR: TOKEN_ERROR recieved.\n");
                return 1;
            }

            if (strlen(yytext) > MAX_TOKEN_LENGTH)
            {
                printf("ERROR: Max token length (%ld characters) exceeded, previous token was %ld characters long.\n",
                       MAX_TOKEN_LENGTH, strlen(yytext));
                return 1;
            }
        }

        printf("=== INPUT SCANNED SUCCESSFULLY!!! ===");
    }

    // Run the parser
    int parse_response = yyparse();

    // Make sure the parse was successful
    if (parse_response != 0)
    {
        printf("ERROR: yyparse() returned %d\n", parse_response);
        return parse_response;
    }

    // If we parse, we want to make sure we stop before program resolution and
    // typechecking
    if (input_arguments.parse)
        return parse_response;

    scope_initialize();
    decl_resolve(parser_result);

    if (input_arguments.graph)
        ast_graph(parser_result);

    if (input_arguments.format)
        decl_print(parser_result, 0);

    // Typechecking step
    decl_typecheck(parser_result);

    if (!typecheck_succeeded)
        return 1;

    // Stop here if we just want to verify typechecking
    if (input_arguments.typecheck)
        return !typecheck_succeeded;

    return 0;
}
