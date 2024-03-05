
#include "token.h"

#include <stdio.h>
#include <string.h>

extern int MAX_TOKEN_LENGTH;

extern FILE *yyin;
extern int yylex();
extern char *yytext;

extern const char* token_name(enum yytokentype t);

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
        enum yytokentype t = yylex();

        printf("token: %2d\t%-24s\ttext: %s\n", t, token_name(t), yytext);

        if (t == 0) // End of file
            break;
        
        if (t == TOKEN_ERROR){
            printf("ERROR: TOKEN_ERROR recieved.\n");
            return 1;
        }
        
        if (strlen(yytext) > MAX_TOKEN_LENGTH) {
            printf("ERROR: Max token length (%d characters) exceeded, previous token was %ld characters long.\n", MAX_TOKEN_LENGTH, strlen(yytext));
            return 1;
        }

    }

    return 0;
}
