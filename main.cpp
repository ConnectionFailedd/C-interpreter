
#include "function.hpp"
#include "name.hpp"
#include "scope.hpp"
#include "syntax_tree.hpp"
#include "type.hpp"
#include "value.hpp"
#include <memory>
#include <vector>

int main(int argc, char * argv[]) {
    auto value1 = std::make_shared<CINT::Value>(CINT::Type::Type::types[CINT::Name(CINT::Scope::currentScope, "int")], false, false);
    value1->set_value(5);
    auto value2 = std::make_shared<CINT::Value>(CINT::Type::Type::types[CINT::Name(CINT::Scope::currentScope, "int")], false, false);
    value2->set_value(10);
    auto fixedValueNode1 = CINT::SyntaxTree::make_fixed_value_node(value1);
    auto fixedValueNode2 = CINT::SyntaxTree::make_fixed_value_node(value2);
    auto arguments = std::vector({fixedValueNode1, fixedValueNode2});
    auto func = CINT::Function::builtInFunctionMap[CINT::Function::Signature(CINT::Name(CINT::Scope::currentScope, "+"), CINT::Type::Type::types[CINT::Name(CINT::Scope::currentScope, "int")], std::vector({CINT::Type::Type::types[CINT::Name(CINT::Scope::currentScope, "int")], CINT::Type::Type::types[CINT::Name(CINT::Scope::currentScope, "int")]}))];
    auto funcNode = CINT::SyntaxTree::make_built_in_function_node(func, arguments);
    auto returnNode = CINT::SyntaxTree::make_return_node(funcNode);
    auto tree = CINT::SyntaxTree(returnNode);
    CINT::SyntaxTree::functionStack.push({}, {});
    tree.evaluate();
    int returnValue = CINT::SyntaxTree::functionStack.get_return_value()->get_value<int>();
    return 0;
}