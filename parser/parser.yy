%skeleton "lalr1.cc"
%require "3.8.2"

%define api.namespace { CINT::PreProcess }
%define api.parser.class { Parser }
%define api.value.type variant
%define api.token.constructor
%defines

%nterm <int> term

%token <int> INTEGER
%token YYEOF 0

%%

term:
    %empty {}
    | INTEGER { $$ = $1; }
;

%%