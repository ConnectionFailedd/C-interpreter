%{
# include <string>
# include "driver.hpp"
# include "parser.hpp"
%}

%option noyywrap nounput noinput batch debug

%{
    // Code run each time a pattern is matched.
    # define YY_USER_ACTION
    // location.columns(yyleng);
%}

%%

%{
    // Code run each time yytex() is called.
    auto & location = _driver.location;
    location.step ();
%}

[ \t\r]+    { location.step(); }
\n+         {
                location.lines(yyleng);
                location.step();
            }

"void"      { return CINT::PreProcess::Parser::make_VOID_TYPE(yytext, location); }
"char"      { return CINT::PreProcess::Parser::make_CHAR_TYPE(yytext, location); }
"short"     { return CINT::PreProcess::Parser::make_SHORT_TYPE(yytext, location); }
"int"       { return CINT::PreProcess::Parser::make_INT_TYPE(yytext, location); }
"float"     { return CINT::PreProcess::Parser::make_FLOAT_TYPE(yytext, location); }
"double"    { return CINT::PreProcess::Parser::make_DOUBLE_TYPE(yytext, location); }

"long"      { return CINT::PreProcess::Parser::make_LONG(yytext, location); }
"unsigned"  { return CINT::PreProcess::Parser::make_UNSIGNED(yytext, location); }

"const"     { return CINT::PreProcess::Parser::make_CONST(yytext, location); }

[_a-zA-Z][_a-zA-Z0-9]* { return CINT::PreProcess::Parser::make_IDENTIFIER(yytext, location); }

"<<="       { return CINT::PreProcess::Parser::make_SHIFT_LEFT_ASSIGN(location); }
">>="       { return CINT::PreProcess::Parser::make_SHIFT_RIGHT_ASSIGN(location); }
"&&"        { return CINT::PreProcess::Parser::make_LOGIC_AND(location); }
"||"        { return CINT::PreProcess::Parser::make_LOGIC_OR(location); }
"::"        { return CINT::PreProcess::Parser::make_SCOPE(location); }
"++"        { return CINT::PreProcess::Parser::make_INCERMENT(location); }
"--"        { return CINT::PreProcess::Parser::make_DECREMENT(location); }
"->"        { return CINT::PreProcess::Parser::make_ARROW(location); }
"<<"        { return CINT::PreProcess::Parser::make_SHIFT_LEFT(location); }
">>"        { return CINT::PreProcess::Parser::make_SHIFT_RIGHT(location); }
"<="        { return CINT::PreProcess::Parser::make_LESS_EQUAL(location); }
">="        { return CINT::PreProcess::Parser::make_GREATER_EQUAL(location); }
"=="        { return CINT::PreProcess::Parser::make_EQUAL(location); }
"!="        { return CINT::PreProcess::Parser::make_NOT_EQUAL(location); }
"+="        { return CINT::PreProcess::Parser::make_ADD_ASSIGN(location); }
"-="        { return CINT::PreProcess::Parser::make_SUB_ASSIGN(location); }
"*="        { return CINT::PreProcess::Parser::make_MUL_ASSIGN(location); }
"/="        { return CINT::PreProcess::Parser::make_DIV_ASSIGN(location); }
"%="        { return CINT::PreProcess::Parser::make_MOD_ASSIGN(location); }
"&="        { return CINT::PreProcess::Parser::make_AND_ASSIGN(location); }
"^="        { return CINT::PreProcess::Parser::make_XOR_ASSIGN(location); }
"|="        { return CINT::PreProcess::Parser::make_OR_ASSIGN(location); }
"("         { return CINT::PreProcess::Parser::make_LPAREN(location); }
")"         { return CINT::PreProcess::Parser::make_RPAREN(location); }
"]"         { return CINT::PreProcess::Parser::make_LBRACKET(location); }
"["         { return CINT::PreProcess::Parser::make_RBRACKET(location); }
"{"         { return CINT::PreProcess::Parser::make_LBRACE(location); }
"}"         { return CINT::PreProcess::Parser::make_RBRACE(location); }
"."         { return CINT::PreProcess::Parser::make_DOT(location); }
"+"         { return CINT::PreProcess::Parser::make_PLUS(location); }
"-"         { return CINT::PreProcess::Parser::make_MINUS(location); }
"!"         { return CINT::PreProcess::Parser::make_LOGIC_NOT(location); }
"~"         { return CINT::PreProcess::Parser::make_BITWISE_NOT(location); }
"*"         { return CINT::PreProcess::Parser::make_MULTIPLY(location); }
"&"         { return CINT::PreProcess::Parser::make_BITWISE_AND(location); }
"/"         { return CINT::PreProcess::Parser::make_DIVIDE(location); }
"%"         { return CINT::PreProcess::Parser::make_MOD(location); }
"<"         { return CINT::PreProcess::Parser::make_LESS(location); }
">"         { return CINT::PreProcess::Parser::make_GREATER(location); }
"^"         { return CINT::PreProcess::Parser::make_BITWISE_XOR(location); }
"|"         { return CINT::PreProcess::Parser::make_BITWISE_OR(location); }
"?"         { return CINT::PreProcess::Parser::make_CONDITION1(location); }
":"         { return CINT::PreProcess::Parser::make_CONDITION2(location); }
"="         { return CINT::PreProcess::Parser::make_ASSIGN(location); }
","         { return CINT::PreProcess::Parser::make_COMMA(location); }

"'"         { return CINT::PreProcess::Parser::make_SINGLE_QUOTATION(location); }
"\""        { return CINT::PreProcess::Parser::make_DOUBLE_QUOTATION(location); }
";"         { return CINT::PreProcess::Parser::make_SEMICOLON(location); }

[1-9][0-9]* |
0[1-9]+ |
0[xX][0-9a-fA-F]+ |
0[bB][01]+ |
0           { return CINT::PreProcess::Parser::make_INT_LITERAL(yytext, location); }

.           { throw CINT::PreProcess::Parser::syntax_error(location, "invalid character: " + std::string(yytext)); }
<<EOF>>     { return CINT::PreProcess::Parser::make_YYEOF(location); }

%%
