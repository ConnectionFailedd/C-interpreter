%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%define api.parser.class {Parser}
%define api.namespace { CINT::PreProcess }

%header
%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires {
# include <string>
namespace CINT {
namespace PreProcess {

class Driver;

}
}
}
// The parsing context.
%param { CINT::PreProcess::Driver & _driver }
%locations
%define parse.trace
%define parse.error detailed
%define parse.lac full
%code {
# include "driver.hpp"
}
%define api.token.prefix {TOK_}
%token
  ASSIGN  ":="
  MINUS   "-"
  PLUS    "+"
  STAR    "*"
  SLASH   "/"
  LPAREN  "("
  RPAREN  ")"
;
%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%nterm <int> exp
%printer { yyo << $$; } <*>;
%%
%start unit;
unit: assignments exp  { _driver.result = $2; };

assignments:
  %empty                 {}
| assignments assignment {};

assignment:
  "identifier" ":=" exp { _driver.variables[$1] = $3; };

%left "+" "-";
%left "*" "/";
exp:
  "number"
| "identifier"  { $$ = _driver.variables[$1]; }
| exp "+" exp   { $$ = $1 + $3; }
| exp "-" exp   { $$ = $1 - $3; }
| exp "*" exp   { $$ = $1 * $3; }
| exp "/" exp   { $$ = $1 / $3; }
| "(" exp ")"   { $$ = $2; }
%%
void
CINT::PreProcess::Parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}