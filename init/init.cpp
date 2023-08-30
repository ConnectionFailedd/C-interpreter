#include <map>
#include <memory>
#include <new>
#include <set>
#include <vector>

#include "function.hpp"
#include "name.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "value.hpp"

namespace CINT {

auto Scope::currentScope = Scope();
auto Scope::scopeStack = std::stack<Scope>();

const auto Name::NONAME = Name("");

const auto Types::Type::NOTYPE = std::make_shared<Type>();
auto Types::Type::typeMultiSet = Type::TypeMultiSet::init();

const auto Value::NOVALUE = std::make_shared<Value>(Value(Types::Type::NOTYPE, true, false));
auto Value::variables = Value::VariableMultiMap();

const auto Function::NOFUNCTION = std::make_shared<Function>(FunctionName(""));
auto Function::functionStack = FunctionStack();
auto Function::functionMultiSet = Function::FunctionMultiSet::init();

} // namespace CINT