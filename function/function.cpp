#include <memory>

#include "function.hpp"
#include "value.hpp"

namespace CINT::Function::BuiltInFunctions {

template<class _T>
std::shared_ptr<Value> increment(const std::vector<std::shared_ptr<Value>> & _arguments) {
    auto value = _arguments[0]->get_value<_T>();
    value++;
    _arguments[0]->set_value(value);
    return _arguments[0];
}

template<class _Tt, class _Ts>
std::shared_ptr<Value> cast(const std::vector<std::shared_ptr<Value>> & _arguments) {
    auto srcValue = _arguments[1]->get_value<_Ts>();
    _arguments[0]->set_value(_Tt(srcValue));
}

template<class _T>
std::shared_ptr<Value> assign(const std::vector<std::shared_ptr<Value>> & _arguments) {
    auto rhs = _arguments[1]->get_value<_T>();
    _arguments[0]->set_value<_T>(rhs);
    return _arguments[0];
}

template<class _T>
std::shared_ptr<Value> add(const std::vector<std::shared_ptr<Value>> & _arguments) {
    auto lhs = _arguments[0]->get_value<_T>();
    auto rhs = _arguments[1]->get_value<_T>();
    auto res = lhs + rhs;
    auto returnValue = std::make_shared<Value>(Value(_arguments[0]->get_type(), false, false));
    returnValue->set_value<_T>(res);
    return returnValue;
}

} // namespace CINT::Function::BuiltInFunctions