%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%define api.parser.class {Parser}
%define api.namespace { CINT::Frontend }

%header
%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires {
#include "any.hpp"


#include "instruction.hpp"
#include "type.hpp"
#include "value.hpp"
#include "syntax_tree.hpp"
#include "instruction_flow.hpp"
#include "virtual_machine.hpp"
#include <algorithm>
#include <string>
#include <memory>
#include <vector>

#define YY_DECL CINT::Frontend::Parser::symbol_type yylex()
#define YYDEBUG false

namespace CINT::Frontend {
extern CINT::IR::InstructionFlow program;
}
}

%code {

#include "syntax_tree.hpp"
#include "function_signature.hpp"
#include "symbol_table.hpp"
#include <iostream>

YY_DECL;

namespace CINT::Frontend {
CINT::IR::InstructionFlow program = CINT::IR::InstructionFlow();
}
}

/* 
%locations
%define api.location.file "../include/location.hpp"
%define api.location.include {"location.hpp"} */

%define parse.trace
%define parse.error detailed
%define parse.lac full


%define api.token.prefix {TOK_}

%token <std::string> INT_TYPE;

%token <std::string> IDENTIFIER;

%token <std::string> ADD SUB MUL DIV MOD ASSIGN;
%token <std::string> LPAREN RPAREN LBRACE RBRACE SEMICOLON;

%token <std::string> IF ELSE WHILE FOR DO;

%token <std::string> INT_LITERAL;

%token <std::string> SEP;

%nterm <std::shared_ptr<CINT::IR::Type>> type;

%nterm <std::shared_ptr<CINT::IR::Value>> literal variable;

%nterm <std::shared_ptr<CINT::IR::SyntaxTreeNode>> expr empty_expr non_empty_expr expr1 expr2 expr3 term;
%nterm <std::shared_ptr<CINT::IR::Instruction>> glo_stmt glo_open_stmt glo_closed_stmt glo_simple_stmt glo_var_def glo_aggr_stmt;
%nterm <std::vector<std::shared_ptr<CINT::IR::Instruction>>> glo_stmts;
%nterm <CINT::IR::InstructionFlow> program;

%nterm <int> forward;
/* 
%nterm <std::vector<Any>> test; */

%%
%start forward;
/* 
test:
    test glo_stmts {
        $$ = std::move($1);
        $$.push_back($2);
    }
|   {
        $$ = std::vector<Any>();
    }
; */

forward:
    forward program {
        $$ = 0;
        auto value = CINT::Backend::virtualMachine.execute($2);
        if(value->type() == CINT::SymbolTable::NativeTypes::INT_TYPE) {
            std::cout << value->as<int>() << std::endl;
        }
    }
|   { $$ = 0; }
;

program:
    glo_stmts SEP {
        $$ = CINT::IR::InstructionFlow();
        for(auto instruction : $1) {
            $$.append(instruction);
        }
        $$.append(std::make_shared<CINT::IR::ReturnInstruction>(std::make_shared<CINT::IR::ValueNode>(CINT::SymbolTable::None::NONE_VALUE)));
    }
|   non_empty_expr SEP {
        $$ = CINT::IR::InstructionFlow();
        auto instruction = std::make_shared<CINT::IR::ReturnInstruction>($1);
        $$.append(instruction);
    }
;

glo_stmts:
    glo_stmts glo_stmt {
        $$ = std::move($1);
        $$.push_back($2);
    }
|   { $$ = std::vector<std::shared_ptr<CINT::IR::Instruction>>(); }
;

glo_stmt:
    glo_open_stmt { $$ = $1; }
|   glo_closed_stmt { $$ = $1; }
;

glo_open_stmt:
    IF LPAREN non_empty_expr RPAREN glo_open_stmt {
        auto condition = $3;
        auto trueBranch = $5;
        auto branchInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, 2);
        auto jumpInstruction = std::make_shared<CINT::IR::JumpInstruction>(trueBranch->size() + 1);
        $$ = std::make_shared<CINT::IR::AggregatedInstruction>(CINT::IR::AggregatedInstruction({branchInstruction, jumpInstruction, trueBranch}));
    }
|   IF LPAREN non_empty_expr RPAREN glo_closed_stmt {
        auto condition = $3;
        auto trueBranch = $5;
        auto branchInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, 2);
        auto jumpInstruction = std::make_shared<CINT::IR::JumpInstruction>(trueBranch->size() + 1);
        $$ = std::make_shared<CINT::IR::AggregatedInstruction>(CINT::IR::AggregatedInstruction({branchInstruction, jumpInstruction, trueBranch}));
    }
|   IF LPAREN non_empty_expr RPAREN glo_closed_stmt ELSE glo_open_stmt {
        auto condition = $3;
        auto trueBranch = $5;
        auto falseBranch = $7;
        auto branchInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, falseBranch->size() + 2);
        auto jumpInstruction = std::make_shared<CINT::IR::JumpInstruction>(trueBranch->size() + 1);
        $$ = std::make_shared<CINT::IR::AggregatedInstruction>(CINT::IR::AggregatedInstruction({branchInstruction, falseBranch, jumpInstruction, trueBranch}));
    }
|   WHILE LPAREN non_empty_expr RPAREN glo_open_stmt {
        auto condition = $3;
        auto body = $5;
        auto branchInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, 2);
        auto jumpInstruction = std::make_shared<CINT::IR::JumpInstruction>(body->size() + 2);
        auto returnInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, -body->size());
        $$ = std::make_shared<CINT::IR::AggregatedInstruction>(CINT::IR::AggregatedInstruction({branchInstruction, jumpInstruction, body, returnInstruction}));
    }
|   FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN glo_open_stmt {
        auto init = std::make_shared<CINT::IR::BasicInstruction>($3);
        auto condition = $5;
        auto update = std::make_shared<CINT::IR::BasicInstruction>($7);
        auto body = $9;
        auto branchInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, 2);
        auto jumpInstruction = std::make_shared<CINT::IR::JumpInstruction>(body->size() + 3);
        auto returnInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, -body->size() - 1);
        $$ = std::make_shared<CINT::IR::AggregatedInstruction>(CINT::IR::AggregatedInstruction({init, branchInstruction, jumpInstruction, body, update, returnInstruction}));
    }
;

glo_closed_stmt:
    glo_simple_stmt { $$ = $1; }
|   IF LPAREN non_empty_expr RPAREN glo_closed_stmt ELSE glo_closed_stmt {
        auto condition = $3;
        auto trueBranch = $5;
        auto falseBranch = $7;
        auto branchInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, falseBranch->size() + 2);
        auto jumpInstruction = std::make_shared<CINT::IR::JumpInstruction>(trueBranch->size() + 1);
        $$ = std::make_shared<CINT::IR::AggregatedInstruction>(CINT::IR::AggregatedInstruction({branchInstruction, falseBranch, jumpInstruction, trueBranch}));
    }
|   WHILE LPAREN non_empty_expr RPAREN glo_closed_stmt {
        auto condition = $3;
        auto body = $5;
        auto branchInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, 2);
        auto jumpInstruction = std::make_shared<CINT::IR::JumpInstruction>(body->size() + 2);
        auto returnInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, -body->size());
        $$ = std::make_shared<CINT::IR::AggregatedInstruction>(CINT::IR::AggregatedInstruction({branchInstruction, jumpInstruction, body, returnInstruction}));
    }
|   FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN glo_closed_stmt {
        auto init = std::make_shared<CINT::IR::BasicInstruction>($3);
        auto condition = $5;
        auto update = std::make_shared<CINT::IR::BasicInstruction>($7);
        auto body = $9;
        auto branchInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, 2);
        auto jumpInstruction = std::make_shared<CINT::IR::JumpInstruction>(body->size() + 3);
        auto returnInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, -body->size() - 1);
        $$ = std::make_shared<CINT::IR::AggregatedInstruction>(CINT::IR::AggregatedInstruction({init, branchInstruction, jumpInstruction, body, update, returnInstruction}));
    }
;

glo_simple_stmt:
    glo_var_def { $$ = $1; }
|   expr SEMICOLON { $$ = std::make_shared<CINT::IR::BasicInstruction>($1); }
|   glo_aggr_stmt { $$ = $1; }
|   DO glo_stmt WHILE LPAREN non_empty_expr RPAREN SEMICOLON {
        auto body = $2;
        auto condition = $5;
        auto returnInstruction = std::make_shared<CINT::IR::BranchInstruction>(condition, -body->size());
        $$ = std::make_shared<CINT::IR::AggregatedInstruction>(CINT::IR::AggregatedInstruction({body, returnInstruction}));
    }
;

glo_var_def:
    type IDENTIFIER SEMICOLON {
        auto value = std::make_shared<CINT::IR::Value>($1);
        CINT::SymbolTable::variableMap[$2] = value;
        auto expression = std::make_shared<CINT::IR::ValueNode>(value);
        $$ = std::make_shared<CINT::IR::BasicInstruction>(expression);
    }
|   type IDENTIFIER ASSIGN non_empty_expr SEMICOLON {
        auto value = std::make_shared<CINT::IR::Value>($1);
        CINT::SymbolTable::variableMap[$2] = value;
        auto signature = CINT::IR::FunctionSignature("=", {value->type(), $4->type()});
        auto key = std::make_shared<CINT::IR::FunctionKey>(signature);
        auto iter = CINT::SymbolTable::functionSet.find(key);
        auto function = iter == CINT::SymbolTable::functionSet.end() ? CINT::SymbolTable::None::NONE_FUNCTION : *iter;
        auto lhs = std::make_shared<CINT::IR::ValueNode>(value);
        auto rhs = $4;
        auto expression = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(function, {lhs, rhs}));
        $$ = std::make_shared<CINT::IR::BasicInstruction>(expression);
    }
;

type:
    INT_TYPE { $$ = CINT::SymbolTable::NativeTypes::INT_TYPE; }
;

glo_aggr_stmt:
    LBRACE glo_stmts RBRACE { $$ = std::make_shared<CINT::IR::AggregatedInstruction>($2); }
;

expr:
    empty_expr { $$ = $1; }
|   non_empty_expr { $$ = $1; }

empty_expr:
    { $$ = std::make_shared<CINT::IR::ValueNode>(CINT::SymbolTable::None::NONE_VALUE); }
;

non_empty_expr:
    expr1 { $$ = $1; }
;

expr1:
    expr2 ASSIGN expr1 {
        auto signature = CINT::IR::FunctionSignature("=", {$1->type(), $3->type()});
        auto key = std::make_shared<CINT::IR::FunctionKey>(signature);
        auto iter = CINT::SymbolTable::functionSet.find(key);
        if(iter == CINT::SymbolTable::functionSet.end()) {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(CINT::SymbolTable::None::NONE_FUNCTION, {$1, $3}));
        }
        else {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(*iter, {$1, $3}));
        }
    }
|   expr2 { $$ = $1; }
;

expr2:
    expr2 ADD expr3 {
        auto signature = CINT::IR::FunctionSignature("+", {$1->type(), $3->type()});
        auto key = std::make_shared<CINT::IR::FunctionKey>(signature);
        auto iter = CINT::SymbolTable::functionSet.find(key);
        if(iter == CINT::SymbolTable::functionSet.end()) {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(CINT::SymbolTable::None::NONE_FUNCTION, {$1, $3}));
        }
        else {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(*iter, {$1, $3}));
        }
    }
|   expr2 SUB expr3 {
        auto signature = CINT::IR::FunctionSignature("-", {$1->type(), $3->type()});
        auto key = std::make_shared<CINT::IR::FunctionKey>(signature);
        auto iter = CINT::SymbolTable::functionSet.find(key);
        if(iter == CINT::SymbolTable::functionSet.end()) {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(CINT::SymbolTable::None::NONE_FUNCTION, {$1, $3}));
        }
        else {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(*iter, {$1, $3}));
        }
    }
|   expr3 { $$ = $1; }
;

expr3:
    expr3 MUL term {
        auto signature = CINT::IR::FunctionSignature("*", {$1->type(), $3->type()});
        auto key = std::make_shared<CINT::IR::FunctionKey>(signature);
        auto iter = CINT::SymbolTable::functionSet.find(key);
        if(iter == CINT::SymbolTable::functionSet.end()) {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(CINT::SymbolTable::None::NONE_FUNCTION, {$1, $3}));
        }
        else {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(*iter, {$1, $3}));
        }
    }
|   expr3 DIV term {
        auto signature = CINT::IR::FunctionSignature("/", {$1->type(), $3->type()});
        auto key = std::make_shared<CINT::IR::FunctionKey>(signature);
        auto iter = CINT::SymbolTable::functionSet.find(key);
        if(iter == CINT::SymbolTable::functionSet.end()) {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(CINT::SymbolTable::None::NONE_FUNCTION, {$1, $3}));
        }
        else {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(*iter, {$1, $3}));
        }
    }
|   expr3 MOD term {
        auto signature = CINT::IR::FunctionSignature("%", {$1->type(), $3->type()});
        auto key = std::make_shared<CINT::IR::FunctionKey>(signature);
        auto iter = CINT::SymbolTable::functionSet.find(key);
        if(iter == CINT::SymbolTable::functionSet.end()) {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(CINT::SymbolTable::None::NONE_FUNCTION, {$1, $3}));
        }
        else {
            $$ = std::make_shared<CINT::IR::FunctionNode>(CINT::IR::FunctionNode(*iter, {$1, $3}));
        }
    }
|   term { $$ = $1; }
;

term:
    variable { $$ = std::make_shared<CINT::IR::ValueNode>($1); }
|   literal { $$ = std::make_shared<CINT::IR::ValueNode>($1); }
|   LPAREN expr RPAREN { $$ = $2; }
;

literal:
    INT_LITERAL {
        $$ = std::make_shared<CINT::IR::Value>(CINT::SymbolTable::NativeTypes::INT_TYPE);
        $$->as<int>() = std::stoi($1);
    }
;

variable:
    IDENTIFIER {
        auto iter = CINT::SymbolTable::variableMap.find($1);
        if(iter == CINT::SymbolTable::variableMap.end()) {
            $$ = CINT::SymbolTable::None::NONE_VALUE;
        }
        else {
            $$ = iter->second;
        }
    }
;

%%

void
CINT::Frontend::Parser::error (const std::string& m)
{
  std::cerr << m << '\n';
}