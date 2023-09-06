#include <functional>
#include <memory>
#include <set>
#include <vector>

#include "function.hpp"
#include "name.hpp"
#include "scope.hpp"
#include "syntax_tree.hpp"
#include "type.hpp"
#include "value.hpp"

namespace CINT {

Function::FunctionStack::Element::Element() : __arguments(), __localScopeStack(), __localUsingScopesStack() {
    __localScopeStack.push_back(std::make_shared<Scope>(Scope::ScopeName("")));
}

std::shared_ptr<const Value> UserDefinedFunction::execute() const { return __functionBody->evaluate(); }

template<class _T>
std::shared_ptr<const Value> BuiltInFunction::add() {
    auto lhs = functionStack.arguments()[0]->get_value_as<_T>();
    auto rhs = functionStack.arguments()[1]->get_value_as<_T>();
    auto res = lhs + rhs;
    auto returnValue = std::make_shared<Value>(Value(functionStack.arguments()[0]->type(), false, false));
    returnValue->set_value_as<_T>(res);
    return returnValue;
}

template<class _T>
std::shared_ptr<const Value> BuiltInFunction::assign() {
    auto rhs = functionStack.arguments()[1]->get_value_as<_T>();
    functionStack.arguments()[0]->set_value_as<_T>(rhs);
    return functionStack.arguments()[0];
}

} // namespace CINT