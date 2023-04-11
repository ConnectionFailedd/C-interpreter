#include <memory>
#include <vector>

#include "function.hpp"
#include "syntax_tree.hpp"
#include "type.hpp"
#include "value.hpp"

namespace CINT {

std::shared_ptr<Value> SyntaxTree::BuiltInFunctionNode::evaluate() {
    auto arguments = std::vector<std::shared_ptr<Value>>();
    for(auto child : __children) {
        arguments.push_back(child->evaluate());
    }
    return __builtInFunction(arguments);
}

std::shared_ptr<Value> SyntaxTree::UserDefinedFunctionNode::evaluate() {
    auto arguments = std::vector<std::shared_ptr<Value>>();
    for(auto child : __children) {
        arguments.push_back(child->evaluate());
    }
    return __userDefinedFunction->evaluate(arguments);
}

std::shared_ptr<Value> SyntaxTree::IfNode::evaluate() {
    if(__condition->evaluate()->get_value<bool>()) __trueBranch->evaluate();
    else __falseBranch->evaluate();
    return Value::NOVALUE;
}

std::shared_ptr<Value> SyntaxTree::WhileNode::evaluate() {
    // break & continue ?
    while(__condition->evaluate()->get_value<bool>()) __trueBranch->evaluate();
    __falseBranch->evaluate();
    return Value::NOVALUE;
}

std::shared_ptr<Value> SyntaxTree::ForNode::evaluate() {
    // break & continue ?
    for(__initialization->evaluate(); __condition->evaluate()->get_value<bool>(); __increment->evaluate()) {
        __trueBranch->evaluate();
    }
    __falseBranch->evaluate();
    return Value::NOVALUE;
}

} // namespace CINT