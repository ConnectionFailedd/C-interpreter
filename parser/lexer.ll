%option c++
%option noyywrap

%{
    #include "parser.hpp"
    #include "driver.hpp"
%}

%%

[0-9] { return CINT::PreProcess::Parser::make_INTEGER(std::stoi(yytext)); }

%%