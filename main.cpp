
#include "name.hpp"
#include "scope.hpp"
#include "syntax_tree.hpp"
#include "type.hpp"
#include "value.hpp"
#include <memory>
#include <vector>

auto int_type = std::make_shared<CINT::Type::BuiltInType>(4, std::align_val_t(4), CINT::Name(CINT::Scope::currentScope, "int"));

std::shared_ptr<CINT::Value> int_add(std::vector<std::shared_ptr<CINT::Value>> _arguments) {
    int lhs = _arguments[0]->get_value<int>(), rhs = _arguments[1]->get_value<int>();
    int res = lhs + rhs;
    auto returnValue = std::make_shared<CINT::Value>(int_type, false, false);
    returnValue->set_value(res);
    return returnValue;
}

int main(int argc, char * argv[]) {
    auto value1 = std::make_shared<CINT::Value>(int_type, false, false);
    value1->set_value(5);
    auto value2 = std::make_shared<CINT::Value>(int_type, false, false);
    value2->set_value(10);
    auto fixedValueNode1 = CINT::SyntaxTree::make_fixed_value_node(value1);
    auto fixedValueNode2 = CINT::SyntaxTree::make_fixed_value_node(value2);
    auto arguments = std::vector({fixedValueNode1, fixedValueNode2});
    auto func = std::function<std::shared_ptr<CINT::Value>(std::vector<std::shared_ptr<CINT::Value>>)>(int_add);
    auto funcNode = CINT::SyntaxTree::make_built_in_function_node(func, arguments);
    auto returnNode = CINT::SyntaxTree::make_return_node(funcNode);
    auto tree = CINT::SyntaxTree(returnNode);
    CINT::SyntaxTree::functionStack.push({}, {});
    tree.evaluate();
    int returnValue = CINT::SyntaxTree::functionStack.get_return_value()->get_value<int>();
    return 0;
}