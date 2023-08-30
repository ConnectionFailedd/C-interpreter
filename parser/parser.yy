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
#include "function.hpp"
#include "value.hpp"
#include "syntax_tree.hpp"
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
%token PLUS;

%token <std::shared_ptr<CINT::SyntaxTree::Node>> INTEGER;
%nterm <std::shared_ptr<CINT::SyntaxTree::Node>> expression;

%%
%start result;
result:
  expression { auto value = $1->evaluate(); CINT::SyntaxTree::functionStack.push({}, value); std::cout << CINT::SyntaxTree::functionStack.get_return_value()->get_value<int>() << std::endl;};

expression:
  INTEGER { $$ = $1; }
| expression PLUS INTEGER { $$ = CINT::SyntaxTree::make_built_in_function_node(CINT::Function::builtInFunctionMultiMap.find(CINT::Function::Signature(CINT::Name("+")))->second, {$1, $3}); };

%%
void
CINT::PreProcess::Parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}