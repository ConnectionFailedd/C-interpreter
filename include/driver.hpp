
#include "parser.hpp"
#define YY_DECL CINT::PreProcess::Parser::symbol_type yylex()
YY_DECL;