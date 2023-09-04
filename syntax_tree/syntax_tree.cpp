#include <memory>
#include <vector>

#include "function.hpp"
#include "syntax_tree.hpp"
#include "type.hpp"
#include "value.hpp"

namespace CINT {

std::shared_ptr<const Value> SyntaxTree::FunctionNode::evaluate() const {
    Function::functionStack.push({});
    for(auto argument : __arguments) {
        Function::functionStack.arguments().push_back(argument->evaluate());
    }

    auto res = __function->execute();
    Function::functionStack.pop();

    return res;
}

std::shared_ptr<const Value> SyntaxTree::IfNode::evaluate() const {
    if(__condition->evaluate()->get_value_as<bool>()) {
        return __trueBranch->evaluate();
    }

    return __falseBranch->evaluate();
}

std::shared_ptr<const Value> SyntaxTree::WhileNode::evaluate() const {
    while(__condition->evaluate()->get_value_as<bool>()) {
        auto res = __trueBranch->evaluate();
        if(Function::functionStack.break_signal()) {
            Function::functionStack.break_signal() = false;
            break;
        }
        if(Function::functionStack.continue_signal()) {
            Function::functionStack.continue_signal() = false;
            continue;
        }
        if(Function::functionStack.return_signal()) {
            return res;
        }
    }

    return __falseBranch->evaluate();
}

std::shared_ptr<const Value> SyntaxTree::ForNode::evaluate() const {
    for(__initialization->evaluate(); __condition->evaluate()->get_value_as<bool>(); __increment->evaluate()) {
        auto res = __trueBranch->evaluate();
        if(Function::functionStack.break_signal()) {
            Function::functionStack.break_signal() = false;
            break;
        }
        if(Function::functionStack.continue_signal()) {
            Function::functionStack.continue_signal() = false;
            continue;
        }
        if(Function::functionStack.return_signal()) {
            return res;
        }
    }

    return __falseBranch->evaluate();
}

std::shared_ptr<const Value> SyntaxTree::SemicolonNode::evaluate() const {
    auto res = __previousExpression->evaluate();
    if(Function::functionStack.break_signal() || Function::functionStack.continue_signal() || Function::functionStack.return_signal() || __nextSemicolon == nullptr) {
        return res;
    }

    return __nextSemicolon->evaluate();
}

} // namespace CINT