
%{

#include <stdio.h>

%}

%token TOKEN_ARRAY
%token TOKEN_BOOLEAN
%token TOKEN_CHAR
%token TOKEN_ELSE
%token TOKEN_FALSE
%token TOKEN_FOR
%token TOKEN_FUNCTION
%token TOKEN_IF
%token TOKEN_INTEGER
%token TOKEN_PRINT
%token TOKEN_RETURN
%token TOKEN_STRING
%token TOKEN_TRUE
%token TOKEN_VOID
%token TOKEN_WHILE
%token TOKEN_COLON
%token TOKEN_SEMICOLON
%token TOKEN_COMMA
%token TOKEN_LEFTSQUAREBRACKET
%token TOKEN_RIGHTSQUAREBRACKET
%token TOKEN_LEFTCURLYBRACE
%token TOKEN_RIGHTCURLYBRACE
%token TOKEN_LEFTPAREN
%token TOKEN_RIGHTPAREN
%token TOKEN_EQUALS
%token TOKEN_CARRET
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_PLUSPLUS
%token TOKEN_MINUSMINUS
%token TOKEN_MULTIPLY
%token TOKEN_DIVIDE
%token TOKEN_MODULO
%token TOKEN_EQUALSEQUALS
%token TOKEN_GREATERTHANEQUALTO
%token TOKEN_LESSTHANEQUALTO
%token TOKEN_LESSTHAN
%token TOKEN_GREATERTHAN
%token TOKEN_NOTEQUAL
%token TOKEN_PIPEPIPE
%token TOKEN_ANDAND
%token TOKEN_CHARLITERAL
%token TOKEN_STRINGLITERAL
%token TOKEN_NUMBER
%token TOKEN_IDENTIFIER
%token TOKEN_ERROR

%%

program
    : expr TOKEN_SEMICOLON
;

expr
    : expr TOKEN_PLUS term
    | expr TOKEN_MINUS term
    | term
;

term
    : term TOKEN_MULTIPLY factor
    | term TOKEN_DIVIDE factor
    | factor
;

factor
    : TOKEN_MINUS factor
    | TOKEN_LEFTPAREN expr TOKEN_RIGHTPAREN
    | TOKEN_INTEGER
;

%%

int yyerror( char *s ) {
    printf("parse error: %s\n",s);
    return 1;
}
