%{
# include <string>
# include "driver.hpp"
# include "parser.hpp"
%}

%option noyywrap nounput noinput batch debug

%{
    namespace CINT::PreProcess {
    CINT::PreProcess::Parser::symbol_type make_INTEGER(const char *, const CINT::PreProcess::Parser::location_type &);
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

"+"         { return CINT::PreProcess::Parser::make_PLUS(location); }

[0-9]+      { return CINT::PreProcess::make_INTEGER(yytext, location); }

.           { throw CINT::PreProcess::Parser::syntax_error(location, "invalid character: " + std::string(yytext)); }
<<EOF>>     { return CINT::PreProcess::Parser::make_YYEOF(location); }

%%

namespace CINT::PreProcess {

CINT::PreProcess::Parser::symbol_type make_INTEGER(const char * _str, const CINT::PreProcess::Parser::location_type& location) {
    auto int_type = *CINT::Type::Type::typeMultiSet.find(std::make_shared<CINT::Type::BuiltInType>(0, std::align_val_t(0), CINT::Name("int")));
    auto value = std::make_shared<CINT::Value>(int_type, false, false);
    value->set_value<int>(std::stoi(_str));
    auto fixedValueNode = CINT::SyntaxTree::make_fixed_value_node(value);
    return CINT::PreProcess::Parser::make_INTEGER(fixedValueNode, location);
}

}
