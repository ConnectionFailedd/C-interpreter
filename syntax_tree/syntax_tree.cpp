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
    if(__condition->evaluate()->get_value<bool>()) {
        auto res = __trueBranch->evaluate();
        if(res->is_signal()) return res;
    }

    auto res = __falseBranch->evaluate();
    if(res->is_signal()) return res;

    return Value::NOVALUE;
}

std::shared_ptr<Value> SyntaxTree::WhileNode::evaluate() {
    while(__condition->evaluate()->get_value<bool>()) {
        auto res = __trueBranch->evaluate();
        if(res->is_signal()) {
            if(res == SignalValue::BREAK) break;
            if(res == SignalValue::CONTINUE)
                continue;
            else
                return res;
        }
    }

    auto res = __falseBranch->evaluate();
    if(res->is_signal()) return res;

    return Value::NOVALUE;
}

std::shared_ptr<Value> SyntaxTree::ForNode::evaluate() {
    for(__initialization->evaluate(); __condition->evaluate()->get_value<bool>(); __increment->evaluate()) {
        auto res = __trueBranch->evaluate();
        if(res->is_signal()) {
            if(res == SignalValue::BREAK) break;
            if(res == SignalValue::CONTINUE)
                continue;
            else
                return res;
        }
    }

    auto res = __falseBranch->evaluate();
    if(res->is_signal()) return res;

    return Value::NOVALUE;
}

std::shared_ptr<Value> SyntaxTree::SemicolonNode::evaluate() {
    auto res = __previousExpression->evaluate();
    if(res->is_signal()) return res;

    res = __nextSemicolon->evaluate();
    if(res->is_signal()) return res;

    return Value::NOVALUE;
}

} // namespace CINT