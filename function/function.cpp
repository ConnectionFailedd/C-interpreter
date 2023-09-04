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

// namespace CINT::Function {
//
// std::multimap<Signature, std::function<BuiltInFunction>> built_in_function_multi_map_init() {
//     auto res = std::multimap<Signature, std::function<BuiltInFunction>>();
//
//     auto int_type = Types::Type::typeMultiSet.find("int");
//     auto intAddIntIntSignature = Signature(Name("+"), int_type, std::vector({int_type, int_type}));
//     auto intAddIntIntFunction = std::function(BuiltInFunctions::add<int>);
//     res.insert({intAddIntIntSignature, intAddIntIntFunction});
//
//     return res;
// }
//
// std::multimap<Signature, std::shared_ptr<SyntaxTree>> user_defined_function_multi_map_init() {
//     return std::multimap<Signature, std::shared_ptr<SyntaxTree>>();
// }
//
// }
//
// namespace CINT::Function::BuiltInFunctions {
//
// template<class _T>
// std::shared_ptr<Value> increment(const std::vector<std::shared_ptr<Value>> & _arguments) {
//     auto value = _arguments[0]->get_value<_T>();
//     value++;
//     _arguments[0]->set_value(value);
//     return _arguments[0];
// }
//
// template<class _Tt, class _Ts>
// std::shared_ptr<Value> cast(const std::vector<std::shared_ptr<Value>> & _arguments) {
//     auto srcValue = _arguments[1]->get_value<_Ts>();
//     _arguments[0]->set_value(_Tt(srcValue));
// }
//
// template<class _T>
// std::shared_ptr<Value> assign(const std::vector<std::shared_ptr<Value>> & _arguments) {
//     auto rhs = _arguments[1]->get_value<_T>();
//     _arguments[0]->set_value<_T>(rhs);
//     return _arguments[0];
// }
//
// template<class _T>
// std::shared_ptr<Value> add(const std::vector<std::shared_ptr<Value>> & _arguments) {
//     auto lhs = _arguments[0]->get_value<_T>();
//     auto rhs = _arguments[1]->get_value<_T>();
//     auto res = lhs + rhs;
//     auto returnValue = std::make_shared<Value>(Value(_arguments[0]->type(), false, false));
//     returnValue->set_value<_T>(res);
//     return returnValue;
// }
//
// } // namespace CINT::Function::BuiltInFunctions