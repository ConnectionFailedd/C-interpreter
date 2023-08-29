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
  PLUS    "+"
;

%token <int> NUMBER "number"
%nterm <int> expression;

%%
%start result;
result:
  expression { std::cout << $1 << std::endl;};

expression:
  NUMBER {$$ = $1;}
| NUMBER PLUS NUMBER {$$ = $1 + $3;};

%%
void
CINT::PreProcess::Parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}