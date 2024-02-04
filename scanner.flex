%{
#include "token.h"
%}
DIGIT         [0-9]
LETTER        [a-zA-Z]
ALPHANUM      [a-zA-Z0-9]
%%
(" "|\t|\n)     /* skip whitespace */
"//"[^\n]*\n    /* skip single-line comments */
"/*"[^"*"]*"*/" /* skip c-style comments */
array                           { return TOKEN_ARRAY; }
boolean                         { return TOKEN_BOOLEAN; }
char                            { return TOKEN_CHAR; }
else                            { return TOKEN_ELSE; }
false                           { return TOKEN_FALSE; }
for                             { return TOKEN_FOR; }
function                        { return TOKEN_FUNCTION; }
if                              { return TOKEN_IF; }
integer                         { return TOKEN_INTEGER; }
print                           { return TOKEN_PRINT; }
return                          { return TOKEN_RETURN; }
string                          { return TOKEN_STRING; }
true                            { return TOKEN_TRUE; }
void                            { return TOKEN_VOID; }
while                           { return TOKEN_WHILE; }
:                               { return TOKEN_COLON; }
;                               { return TOKEN_SEMICOLON; }
,                               { return TOKEN_COMMA; }
\[                              { return TOKEN_LEFTSQUAREBRACKET; }
\]                              { return TOKEN_RIGHTSQUAREBRACKET; }
\{                              { return TOKEN_LEFTCURLYBRACE; }
\}                              { return TOKEN_RIGHTCURLYBRACE; }
\(                              { return TOKEN_LEFTPAREN; }
\)                              { return TOKEN_RIGHTPAREN; }
=                               { return TOKEN_EQUALS; }
\^                              { return TOKEN_CARRET; }
\+                              { return TOKEN_PLUS; }
-                               { return TOKEN_MINUS; }
\+\+                            { return TOKEN_PLUSPLUS; }
--                              { return TOKEN_MINUSMINUS; }
\*                              { return TOKEN_MULTIPLY; }
\/                              { return TOKEN_DIVIDE; }
%                               { return TOKEN_MODULO; }
==                              { return TOKEN_EQUALSEQUALS; }
\>=                             { return TOKEN_GREATERTHANEQUALTO; }
\<=                             { return TOKEN_LESSTHANEQUALTO; }
\<                              { return TOKEN_LESSTHAN; }
\>                              { return TOKEN_GREATERTHAN; }
!=                              { return TOKEN_NOTEQUAL; }
\|\|                            { return TOKEN_PIPEPIPE; }
&&                              { return TOKEN_ANDAND; }
'(\\.|[^'\\])'                  { return TOKEN_CHARLITERAL; }
\"(\\.|[^"\\\n])*\"             { return TOKEN_STRINGLITERAL; }
[0-9]+                          { return TOKEN_NUMBER; }
[a-zA-Z0-9_]+                   { return TOKEN_IDENTIFIER; }
.                               { return TOKEN_ERROR; }
%%
int yywrap() { return 1; }
