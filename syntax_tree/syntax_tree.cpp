#include <memory>
#include <vector>

#include "function.hpp"
#include "syntax_tree.hpp"
#include "type.hpp"
#include "value.hpp"

namespace CINT {

std::shared_ptr<Value> SyntaxTree::FunctionNode::evaluate() {
    Function::functionStack.push();
    for(auto argument : __arguments) {
        Function::functionStack.arguments().push_back(argument->evaluate());
    }

    __function->execute();

    auto res = Function::functionStack.return_value();
    Function::functionStack.pop();

    return res;
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