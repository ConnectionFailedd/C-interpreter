%{
# include <string>
# include "driver.hpp"
# include "parser.hpp"
%}

%option noyywrap nounput noinput batch debug

%{
    namespace CINT::PreProcess {
    CINT::PreProcess::Parser::symbol_type make_INT_LITERAL(const char *, const CINT::PreProcess::Parser::location_type &);
    }
%}


%{
    // Code run each time a pattern is matched.
    # define YY_USER_ACTION location.columns(yyleng);
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

"::"
"++"
"--"
"("
")"
"{"
"}"
"["
"]"
"."
"->"
"+"         { return CINT::PreProcess::Parser::make_PLUS(location); }
"-"         { return CINT::PreProcess::Parser::make_MINUS(location); }
"!"
"~"
"*"
"&"
"/"
"%"
"+"
"-"
"<<"
">>"
"<"
"<="
">"
">="
"=="
"!="
"^"
"|"
"&&"
"||"
"?"
":"
"="         { return CINT::PreProcess::Parser::make_ASSIGN(location); }
"+="
"-="
"*="
"/="
"%="
"<<="
">>="
"&="
"^="
"|="
","

"'"
"\""

";"         { return CINT::PreProcess::Parser::make_SEMICOLON(location); }

0
[1-9][0-9]*
0[1-9]+
0[xX][0-9a-fA-F]+
0[bB][01]+

0[uU]
[1-9][0-9]*[uU]
0[1-9]+[uU]
0[xX][0-9a-fA-F]+[uU]
0[bB][01]+[uU]

0[lL]
[1-9][0-9]*[lL]
0[1-9]+[lL]
0[xX][0-9a-fA-F]+[lL]
0[bB][01]+[lL]

0[uU][lL]|0[lL][uU]
[1-9][0-9]*[uU][lL]|[1-9][0-9]*[lL][uU]
0[1-9]+[uU][lL]|[1-9][0-9]*[lL][uU]
0[xX][0-9a-fA-F]+[uU][lL]|[1-9][0-9]*[lL][uU]
0[bB][01]+[uU][lL]|[1-9][0-9]*[lL][uU]

'[^'\\\n]'
'\\['"\\abfnrtv]'
'\\[0-7]+'
'\\x[0-9a-fA-F]+'

\"([^\"\\\n]|(\\['\"\\abfnrtv])|\\[0-7]+|\\x[0-9a-fA-F]+)+\"

.           { throw CINT::PreProcess::Parser::syntax_error(location, "invalid character: " + std::string(yytext)); }
<<EOF>>     { return CINT::PreProcess::Parser::make_YYEOF(location); }

%%

namespace CINT::PreProcess {

CINT::PreProcess::Parser::symbol_type make_INT_LITERAL(const char * _str, const CINT::PreProcess::Parser::location_type& location) {
    auto int_type = CINT::Types::Type::typeMultiSet.find(UnconfirmedName("int"));
    auto value = std::make_shared<CINT::Value>(int_type, false, false);
    value->set_value<int>(std::stol(_str));
    auto fixedValueNode = CINT::SyntaxTree::make_fixed_value_node(value);
    return CINT::PreProcess::Parser::make_INT_LITERAL(fixedValueNode, location);
}

}
