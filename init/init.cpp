#include "name.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "value.hpp"
#include <memory>

namespace CINT {

auto Scope::currentScope = Scope();
auto Scope::scopeStack = std::stack<Scope>();

const auto Name::NONAME = Name(Scope(), "");

const std::shared_ptr<Type::Type> Type::Type::NOTYPE = std::make_shared<BuiltInType>(BuiltInType(0, std::align_val_t(alignof(std::max_align_t)), Name::NONAME));

auto Value::variables = std::map<Value::VariableName, std::shared_ptr<Value>>();
const std::shared_ptr<Value> Value::NOVALUE = std::make_shared<Value>(Value(Type::Type::NOTYPE, true, false));

} // namespace CINT