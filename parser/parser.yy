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

%token INT_TYPE;

%token PLUS ASSIGN SEMICOLON;

%token <std::string> IDENTIFIER;
%token <std::shared_ptr<CINT::SyntaxTree::Node>> INT_LITERAL;
%nterm <std::shared_ptr<CINT::SyntaxTree::Node>> program statements statement expression declaration definition term;

%%
// %start result;
// result:
//   expression { std::cout << $1->evaluate()->get_value<int>() << std::endl;};
// 
// expression:
//   INT_LITERAL { $$ = $1; }
// | expression PLUS INT_LITERAL {
//   auto int_type = Types::Type::typeMultiSet.find(UnconfirmedName("int"));
//   $$ = CINT::SyntaxTree::make_function_node(CINT::Function::functionMultiSet.find(Function::UnconfirmedSignature(UnconfirmedName("+"), int_type, {int_type, int_type})), {$1, $3}); };

%start program;
program:
    expression { std::cout << * $1->evaluate() << std::endl; }
|   declaration { std::cout << * $1->evaluate() << std::endl; }
|   definition { std::cout << * $1->evaluate() << std::endl; }
|   statements { std::cout << * $1->evaluate() << std::endl; };

statements:
    statement statements {
        $$ = $1;
        dynamic_cast<CINT::SyntaxTree::SemicolonNode *>($1.get())->set_next_semicolon($2);
    }
|   statement { $$ = $1; };

statement:
    expression SEMICOLON { $$ = CINT::SyntaxTree::make_semicolon_node($1, nullptr); }
|   declaration SEMICOLON { $$ = CINT::SyntaxTree::make_semicolon_node($1, nullptr); }
|   definition SEMICOLON { $$ = CINT::SyntaxTree::make_semicolon_node($1, nullptr); };

expression:
    term { $$ = $1; }
|   expression PLUS term {
        auto int_type = Types::Type::typeMultiSet.find(UnconfirmedName("int"));
        $$ = CINT::SyntaxTree::make_function_node(CINT::Function::functionMultiSet.find(Function::UnconfirmedSignature(UnconfirmedName("+"), int_type, {int_type, int_type})), {$1, $3});
    };

term:
    INT_LITERAL { $$ = $1; }
|   IDENTIFIER { $$ = CINT::SyntaxTree::make_fixed_value_node(Variable::globalVaribaleMultiSet.find(UnconfirmedName($1))->value()); };

declaration:
    INT_TYPE IDENTIFIER {
        auto int_type = Types::Type::typeMultiSet.find(UnconfirmedName("int"));
        auto value = std::make_shared<Value>(int_type, false, true);
        Variable::globalVaribaleMultiSet.insert(std::make_shared<Variable>(Variable::VariableName(Scope::currentScopeStack.back(), $2), value));
        $$ = CINT::SyntaxTree::make_fixed_value_node(value);
    };

definition:
    INT_TYPE IDENTIFIER ASSIGN expression {
        auto int_type = Types::Type::typeMultiSet.find(UnconfirmedName("int"));
        auto value = std::make_shared<Value>(int_type, false, true);
        Variable::globalVaribaleMultiSet.insert(std::make_shared<Variable>(Variable::VariableName(Scope::currentScopeStack.back(), $2), value));
        auto valueNode = CINT::SyntaxTree::make_fixed_value_node(value);
        $$ = CINT::SyntaxTree::make_function_node(CINT::Function::functionMultiSet.find(Function::UnconfirmedSignature(UnconfirmedName("="), int_type, {int_type, int_type})), {valueNode, $4});
    };

%%
void
CINT::PreProcess::Parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}