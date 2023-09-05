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
#include <string>
#include "function.hpp"
#include "value.hpp"
#include "variable.hpp"
#include "scope.hpp"
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
%define api.location.file "../include/location.hpp"
%define api.location.include {"location.hpp"}

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
# include "driver.hpp"
}

%define api.token.prefix {TOK_}

%token <std::string> VOID_TYPE CHAR_TYPE SHORT_TYPE INT_TYPE FLOAT_TYPE DOUBLE_TYPE;
%token <std::string> LONG UNSIGNED CONST;

%token <std::string> IDENTIFIER;

%token SHIFT_LEFT_ASSIGN SHIFT_RIGHT_ASSIGN LOGIC_AND LOGIC_OR SCOPE INCERMENT DECREMENT ARROW SHIFT_LEFT SHIFT_RIGHT LESS_EQUAL GREATER_EQUAL EQUAL NOT_EQUAL ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE DOT PLUS MINUS LOGIC_NOT BITWISE_NOT MULTIPLY BITWISE_AND DIVIDE MOD LESS GREATER BITWISE_XOR BITWISE_OR CONDITION1 CONDITION2 ASSIGN COMMA SINGLE_QUOTATION DOUBLE_QUOTATION SEMICOLON;

%token <std::string> INT_LITERAL;

%nterm <std::string> program;

%%
%start program;
program:
    INT_LITERAL { std::cout << "INT_LITERAL" << std::endl; };
%%

void
CINT::PreProcess::Parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}