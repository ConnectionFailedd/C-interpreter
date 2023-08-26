#include <memory>
#include <vector>

#include "syntax_tree.hpp"
#include "type.hpp"
#include "value.hpp"

namespace CINT {

std::shared_ptr<Value> SyntaxTree::BuiltInFunctionNode::evaluate() {
    auto arguments = std::vector<std::shared_ptr<Value>>();
    for(auto argument : __arguments) {
        arguments.push_back(argument->evaluate());
    }
    return __builtInFunction(arguments);
}

std::shared_ptr<Value> SyntaxTree::UserDefinedFunctionNode::evaluate() {
    auto arguments = std::vector<std::shared_ptr<Value>>();
    for(auto argument : __arguments) {
        arguments.push_back(argument->evaluate());
    }

    functionStack.push(arguments, CINT::Value::NOVALUE);
    __userDefinedFunction->evaluate();
    return functionStack.get_return_value();
}

std::shared_ptr<Value> SyntaxTree::IfNode::evaluate() {
    if(__condition->evaluate()->get_value<bool>()) {
        return __trueBranch->evaluate();
    }

    return __falseBranch->evaluate();
}

std::shared_ptr<Value> SyntaxTree::WhileNode::evaluate() {
    while(__condition->evaluate()->get_value<bool>()) {
        auto res = __trueBranch->evaluate();
        if(breakSignal) {
            breakSignal = false;
            break;
        }
        if(continueSignal) {
            continueSignal = false;
            continue;
        }
        if(returnSignal) {
            return res;
        }
    }

    return __falseBranch->evaluate();
}

std::shared_ptr<Value> SyntaxTree::ForNode::evaluate() {
    for(__initialization->evaluate(); __condition->evaluate()->get_value<bool>(); __increment->evaluate()) {
        auto res = __trueBranch->evaluate();
        if(breakSignal) {
            breakSignal = false;
            break;
        }
        if(continueSignal) {
            continueSignal = false;
            continue;
        }
        if(returnSignal) {
            return res;
        }
    }

    return __falseBranch->evaluate();
}

std::shared_ptr<Value> SyntaxTree::SemicolonNode::evaluate() {
    auto res = __previousExpression->evaluate();
    if(breakSignal || continueSignal || returnSignal) {
        return res;
    }

    return __nextSemicolon->evaluate();
}

} // namespace CINT