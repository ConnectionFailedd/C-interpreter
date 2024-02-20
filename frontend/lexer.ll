%{
# include <string>
//# include "driver.hpp"
# include "parser.hpp"
%}

%option noyywrap nounput noinput batch
/* %option debug */

%{
    // Code run each time a pattern is matched.
    # define YY_USER_ACTION
    // location.columns(yyleng);
%}

/* "int"       { return CINT::Frontend::Parser::make_INT_TYPE(yytext, location); } */

%%

%{
    // Code run each time yytex() is called.
    // auto & location = _driver.location;
    // location.step ();
%}

[ \t\r]+    {
                // location.step();
            }
\n          {
                // location.lines(yyleng);
                // location.step();
            }



"int"       { return CINT::Frontend::Parser::make_INT_TYPE(yytext); }

"if"        { return CINT::Frontend::Parser::make_IF(yytext); }
"else"      { return CINT::Frontend::Parser::make_ELSE(yytext); }
"while"     { return CINT::Frontend::Parser::make_WHILE(yytext); }
"for"       { return CINT::Frontend::Parser::make_FOR(yytext); }
"do"        { return CINT::Frontend::Parser::make_DO(yytext); }

[_a-zA-Z][_a-zA-Z0-9]* { return CINT::Frontend::Parser::make_IDENTIFIER(yytext); }

"+"         { return CINT::Frontend::Parser::make_ADD(yytext); }
"-"         { return CINT::Frontend::Parser::make_SUB(yytext); }
"*"         { return CINT::Frontend::Parser::make_MUL(yytext); }
"/"         { return CINT::Frontend::Parser::make_DIV(yytext); }
"%"         { return CINT::Frontend::Parser::make_MOD(yytext); }
"="         { return CINT::Frontend::Parser::make_ASSIGN(yytext); }
"("         { return CINT::Frontend::Parser::make_LPAREN(yytext); }
")"         { return CINT::Frontend::Parser::make_RPAREN(yytext); }
"{"         { return CINT::Frontend::Parser::make_LBRACE(yytext); }
"}"         { return CINT::Frontend::Parser::make_RBRACE(yytext); }
";"         { return CINT::Frontend::Parser::make_SEMICOLON(yytext); }

[1-9][0-9]* |
0[1-9]+ |
0[xX][0-9a-fA-F]+ |
0[bB][01]+ |
0           { return CINT::Frontend::Parser::make_INT_LITERAL(yytext); }

.           { return CINT::Frontend::Parser::make_SEP(yytext); }
<<EOF>>     { return CINT::Frontend::Parser::make_YYEOF(); }

%%
