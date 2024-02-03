
#include "token.h"

#include <stdio.h>

extern FILE *yyin;
extern int yylex();
extern char *yytext;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s -scan filename.bminor\n", argv[0]);
        return 1;
    }

    char *filepath = argv[2];

    yyin = fopen(filepath, "r");
    if (!yyin)
    {
        printf("could not open %s\n", filepath);
        return 1;
    }

    while (1)
    {
        token_t t = yylex();
        if (t == TOKEN_EOF)
            break;
        printf("token: %2d\t%30s\ttext: %s\n", t, token_strings[t], yytext);
    }
}
