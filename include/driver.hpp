#ifndef DRIVER_HH
#define DRIVER_HH
#include "parser.hpp"
#include <map>
#include <string>

// Give Flex the prototype of yylex we want ...
#define YY_DECL \
    CINT::PreProcess::Parser::symbol_type yylex(CINT::PreProcess::Driver & _driver)
// ... and declare it for the parser's sake.
// Conducting the whole scanning and parsing of Calc++.

namespace CINT {
namespace PreProcess {

class Driver {
public:
    Driver();

    int result; // Run the parser on file F.  Return 0 on success.
    int parse(const std::string & f);
    // The name of the file being parsed.
    std::string file;
    // Whether to generate parser debug traces.
    bool trace_parsing;
    // Handling the scanner.
    void scan_begin();
    void scan_end();
    // Whether to generate scanner debug traces.
    bool trace_scanning;
    // The token's location used by the scanner.
    CINT::PreProcess::location location;
};

}
} // namespace CINT::PreProcess

YY_DECL;
#endif // ! DRIVER_HH