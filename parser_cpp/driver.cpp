#include "driver.hpp"
#include "parser.hpp"

// defined in lexer.cpp
extern int yy_flex_debug;
extern FILE * yyin;

namespace CINT {
namespace PreProcess {

Driver::Driver() : trace_parsing(false), trace_scanning(false) {}

void Driver::scan_begin() {
    yy_flex_debug = trace_scanning;
    if(file.empty() || file == "-")
        yyin = stdin;
    else if(!(yyin = fopen(file.c_str(), "r"))) {
        std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }
}

void Driver::scan_end() {
    fclose(yyin);
}

int Driver::parse(const std::string & f) {
    file = f;
    location.initialize(&file);
    scan_begin();
    CINT::PreProcess::Parser parse(*this);
    parse.set_debug_level(trace_parsing);
    int res = parse();
    scan_end();
    return res;
}

}
} // namespace CINT::PreProcess