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

const std::shared_ptr<Types::Type> Types::Type::NOTYPE = std::make_shared<Type>();
auto Types::Type::typeMultiSet = Type::TypeMultiSet::init();

auto Value::variables = Value::VariableMultiMap();
const std::shared_ptr<Value> Value::NOVALUE = std::make_shared<Value>(Value(Types::Type::NOTYPE, true, false));

auto Function::builtInFunctionMultiMap = Function::built_in_function_multi_map_init();
auto Function::userDefinedFunctionMultiMap = Function::user_defined_function_multi_map_init();

} // namespace CINT