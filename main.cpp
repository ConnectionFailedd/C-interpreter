
#include "name.hpp"
#include "scope.hpp"
#include "syntax_tree.hpp"
#include "type.hpp"
#include "value.hpp"
#include <memory>

int main(int argc, char * argv[]) {
    auto int_type = std::make_shared<CINT::Type::BuiltInType>(4, std::align_val_t(4), CINT::Name(CINT::Scope::currentScope, "int"));
    auto value = std::make_shared<CINT::Value>(int_type, false, false);
    auto fixedValueNode = CINT::SyntaxTree::make_fixed_value_node(value);
    auto returnNode = CINT::SyntaxTree::make_return_node(fixedValueNode);
    auto tree = CINT::SyntaxTree(returnNode);
    CINT::SyntaxTree::functionStack.push(CINT::SyntaxTree::StackNode());
    tree.evaluate();
    return 0;
}