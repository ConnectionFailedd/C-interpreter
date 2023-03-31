%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hpp"
# include "parser.hpp"
%}
%option noyywrap nounput noinput batch debug
%{
  // A number symbol corresponding to the value in S.
  CINT::PreProcess::Parser::symbol_type
  make_NUMBER (const std::string &s, const CINT::PreProcess::Parser::location_type& loc);
%}
id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]
%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  CINT::PreProcess::location& loc = _driver.location;
  // Code run each time yylex is called.
  loc.step ();
%}
{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();
"-"        return CINT::PreProcess::Parser::make_MINUS  (loc);
"+"        return CINT::PreProcess::Parser::make_PLUS   (loc);
"*"        return CINT::PreProcess::Parser::make_STAR   (loc);
"/"        return CINT::PreProcess::Parser::make_SLASH  (loc);
"("        return CINT::PreProcess::Parser::make_LPAREN (loc);
")"        return CINT::PreProcess::Parser::make_RPAREN (loc);
":="       return CINT::PreProcess::Parser::make_ASSIGN (loc);

{int}      return make_NUMBER (yytext, loc);
{id}       return CINT::PreProcess::Parser::make_IDENTIFIER (yytext, loc);
.          {
             throw CINT::PreProcess::Parser::syntax_error
               (loc, "invalid character: " + std::string(yytext));
}
<<EOF>>    return CINT::PreProcess::Parser::make_YYEOF (loc);
%%
namespace CINT{
namespace PreProcess {

CINT::PreProcess::Parser::symbol_type
make_NUMBER (const std::string &s, const CINT::PreProcess::Parser::location_type& loc)
{
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw CINT::PreProcess::Parser::syntax_error (loc, "integer is out of range: " + s);
  return CINT::PreProcess::Parser::make_NUMBER ((int) n, loc);
}
void
Driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void
Driver::scan_end ()
{
  fclose (yyin);
}

}
}