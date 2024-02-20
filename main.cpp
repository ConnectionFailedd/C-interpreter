#include "parser.hpp"
#include "symbol_table.hpp"
#include "virtual_machine.hpp"
#include <cstdio>
#include <iostream>
#include <unistd.h>

int main() {
    auto parser = CINT::Frontend::Parser();
    parser();
    // CINT::Backend::VirtualMachine virtualMachine;
    // while(true) {
    //     auto parser = new CINT::Frontend::Parser();
    //     parser->parse();
    //     auto value = virtualMachine.execute(CINT::Frontend::program);
    //     if(value->type() == CINT::SymbolTable::NativeTypes::INT_TYPE) {
    //         std::cout << value->as<int>() << std::endl;
    //     }
    //     delete parser;
    //     fflush(stdin);
    // }
    // while(true) {
    //     auto c = char();
    //     std::cin.get(c);
    //     std::cout << int(c);
    //     sleep(1);
    // }
}