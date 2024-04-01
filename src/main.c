
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decl.h"
#include "token.h"

extern int MAX_TOKEN_LENGTH;

extern FILE *yyin;
extern int yylex();
extern int yyparse();
extern char *yytext;

extern struct decl *parser_result;

extern const char *token_name(enum yytokentype t);

int validateScan()
{
    while (1)
    {
        enum yytokentype t = yylex();

        printf("token: %2d\t%-24s\ttext: %s\n", t, token_name(t), yytext);

        if (t == 0) // End of file
            break;

        if (t == TOKEN_ERROR)
        {
            printf("ERROR: TOKEN_ERROR recieved.\n");
            return 2;
        }

        if (strlen(yytext) > MAX_TOKEN_LENGTH)
        {
            printf("ERROR: Max token length (%d characters) exceeded, previous token was %ld characters long.\n",
                   MAX_TOKEN_LENGTH, strlen(yytext));
            return 3;
        }
    }

    return 0;
}

void printUsage(char *argv0)
{
    printf("Usage:\n");
    printf("\t%s -scan        filename.bminor\n", argv0);
    printf("\t%s -parse       filename.bminor\n", argv0);
    printf("\t%s -prettyprint filename.bminor\n", argv0);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Error: not enough input arguments\n");
        printUsage(argv[0]);
        return 1;
    }

    yyin = fopen(argv[2], "r");
    if (!yyin)
    {
        printf("Error: Could not open file %s\n", argv[2]);
        return 1;
    }

    if (strcmp(argv[1], "-scan") == 0)
        return validateScan();

    if (strcmp(argv[1], "-parse") == 0 || strcmp(argv[1], "-prettyprint") == 0)
    {
        int parse_response = yyparse();

        if (parse_response != 0)
            printf("Error during parse: yyparse() returned %d\n", parse_response);
        else if (strcmp(argv[1], "-prettyprint") == 0)
        {
            printf("\n");
            decl_print(parser_result, 0);
        }
        else
            printf("Parse successful\n");

        return parse_response;
    }

    printf("Error: unrecognized input arguments\n");
    printUsage(argv[0]);

    return 1;
}
