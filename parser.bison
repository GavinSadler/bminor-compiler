
// Grammar inspired by the ANSI C Grammar
// See: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html

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
%token TOKEN_NOT
%token TOKEN_PIPEPIPE
%token TOKEN_ANDAND
%token TOKEN_CHARLITERAL
%token TOKEN_STRINGLITERAL
%token TOKEN_NUMBER
%token TOKEN_IDENTIFIER
%token TOKEN_ERROR

%start program

%define parse.error verbose

//%locations %define api.pure

%token-table

%%

// Literals and parenthesized grouping
primary_expression
    : TOKEN_IDENTIFIER
    | TOKEN_CHARLITERAL
    | TOKEN_STRINGLITERAL
    | TOKEN_NUMBER
    | TOKEN_TRUE
    | TOKEN_FALSE
    | TOKEN_LEFTPAREN expression TOKEN_RIGHTPAREN  // Grouped expression (expression in here)
;

// f(something), a[something], c++, c--
postfix_expression
    : primary_expression
    | postfix_expression TOKEN_LEFTSQUAREBRACKET expression TOKEN_RIGHTSQUAREBRACKET  // Array access A[something]
    | postfix_expression TOKEN_LEFTPAREN list_expression TOKEN_RIGHTPAREN             // Funciton call F(something)
    | postfix_expression TOKEN_LEFTPAREN TOKEN_RIGHTPAREN                             // Empty funciton call F()
    | postfix_expression TOKEN_PLUSPLUS
    | postfix_expression TOKEN_MINUSMINUS
;

// -a, !a
unary_expression
    : postfix_expression
    | TOKEN_MINUS unary_expression
    | TOKEN_NOT unary_expression
;

// a ^ b, a * b, a / b, a % b
multiplicative_expression
    : unary_expression
    | multiplicative_expression TOKEN_CARRET unary_expression
    | multiplicative_expression TOKEN_MULTIPLY unary_expression
    | multiplicative_expression TOKEN_DIVIDE unary_expression
    | multiplicative_expression TOKEN_MODULO unary_expression
;

// a + b, a - b
additive_expression
    : multiplicative_expression
    | additive_expression TOKEN_PLUS multiplicative_expression
    | additive_expression TOKEN_MINUS multiplicative_expression
;

// a < b, a <= b, a > b, a >= b, a == b, a != b, a && b, a || b
comparative_expression
    : additive_expression
    | comparative_expression TOKEN_LESSTHAN additive_expression
    | comparative_expression TOKEN_LESSTHANEQUALTO additive_expression
    | comparative_expression TOKEN_GREATERTHAN additive_expression
    | comparative_expression TOKEN_GREATERTHANEQUALTO additive_expression
    | comparative_expression TOKEN_EQUALSEQUALS additive_expression
    | comparative_expression TOKEN_NOTEQUAL additive_expression
    | comparative_expression TOKEN_ANDAND additive_expression
    | comparative_expression TOKEN_PIPEPIPE additive_expression
;

assignment_expression
    : comparative_expression
    | symbol_declaration
    | TOKEN_IDENTIFIER TOKEN_EQUALS assignment_expression
;

expression
    : assignment_expression
;

list_expression
    : list_expression TOKEN_COMMA expression
    | expression
;

statement_if
    : TOKEN_IF TOKEN_LEFTPAREN expression TOKEN_RIGHTPAREN compound_statement
    | TOKEN_IF TOKEN_LEFTPAREN expression TOKEN_RIGHTPAREN compound_statement TOKEN_ELSE compound_statement
;

statement_for
    : TOKEN_FOR TOKEN_LEFTPAREN expression TOKEN_SEMICOLON expression TOKEN_SEMICOLON expression TOKEN_RIGHTPAREN compound_statement
;

statement_expression
    : expression TOKEN_SEMICOLON
;

statement_print
    : TOKEN_PRINT list_expression TOKEN_SEMICOLON
;

statement_return
    : TOKEN_RETURN expression TOKEN_SEMICOLON
;

statement
    : statement_if
    | statement_for
    | statement_expression
    | statement_print
    | statement_return
;

list_statement
    : list_statement statement
    | statement
;

compound_statement
    : TOKEN_LEFTCURLYBRACE TOKEN_RIGHTCURLYBRACE
    | TOKEN_LEFTCURLYBRACE list_statement TOKEN_RIGHTCURLYBRACE
;

concrete_type
    : TOKEN_INTEGER
    | TOKEN_STRING
    | TOKEN_CHAR
    | TOKEN_BOOLEAN
;

// Types are either concrete, or a composition of array modifiers and concrete types
type
    : concrete_type
    | TOKEN_ARRAY TOKEN_LEFTSQUAREBRACKET TOKEN_RIGHTSQUAREBRACKET type
    | TOKEN_ARRAY TOKEN_LEFTSQUAREBRACKET TOKEN_NUMBER TOKEN_RIGHTSQUAREBRACKET type
;

// Return types are the only type allowed to be void
return_type: type | TOKEN_VOID ;

list_initializer
    : TOKEN_LEFTCURLYBRACE TOKEN_RIGHTCURLYBRACE
    | TOKEN_LEFTCURLYBRACE list_expression TOKEN_RIGHTCURLYBRACE
;

symbol_declaration
    : TOKEN_IDENTIFIER TOKEN_COLON type
    | TOKEN_IDENTIFIER TOKEN_COLON type TOKEN_EQUALS expression
    | TOKEN_IDENTIFIER TOKEN_COLON type TOKEN_EQUALS list_initializer
;

identity
    : TOKEN_IDENTIFIER TOKEN_COLON type
;

list_parameter
    : list_parameter TOKEN_COMMA identity
    | identity
;

function_declaration
    : TOKEN_IDENTIFIER TOKEN_COLON TOKEN_FUNCTION return_type TOKEN_LEFTPAREN TOKEN_RIGHTPAREN TOKEN_EQUALS compound_statement
    | TOKEN_IDENTIFIER TOKEN_COLON TOKEN_FUNCTION return_type TOKEN_LEFTPAREN list_parameter TOKEN_RIGHTPAREN TOKEN_EQUALS compound_statement
;

function_prototype
    : TOKEN_IDENTIFIER TOKEN_COLON TOKEN_FUNCTION return_type TOKEN_LEFTPAREN TOKEN_RIGHTPAREN
    | TOKEN_IDENTIFIER TOKEN_COLON TOKEN_FUNCTION return_type TOKEN_LEFTPAREN list_expression TOKEN_RIGHTPAREN
;

toplevel_declaration
    : symbol_declaration TOKEN_SEMICOLON
    | function_prototype TOKEN_SEMICOLON
    | function_declaration 
;

program
    : toplevel_declaration
    | program toplevel_declaration
    | %empty
;

%%

// See: https://stackoverflow.com/a/32448812
// Allows for the token integer representation to be translated to a string literal
const char* token_name(enum yytokentype t) {
    return yytname[YYTRANSLATE(t)];
}

void yyerror (char const *msg) {
    printf("parse error: %s\n", msg);
}

//void yyerror (YYLTYPE *locp, char const *msg)
//{
//    printf("parse error: %s\n", msg);
//    printf("\tfirst_line=%d\n", locp->first_line);
//    printf("\tfirst_column=%d\n", locp->first_column);
//    printf("\tlast_line=%d\n", locp->last_line);
//    printf("\tlast_column=%d\n", locp->last_column);
//}
