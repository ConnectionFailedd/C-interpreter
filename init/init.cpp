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

const auto Name::NONAME = Name(Scope(), "");

const std::shared_ptr<Type::Type> Type::Type::NOTYPE = std::make_shared<BuiltInType>(BuiltInType(0, std::align_val_t(alignof(std::max_align_t)), Name::NONAME));
auto Type::Type::typeMultiSet = std::multiset<std::shared_ptr<Type>, Type::Type::SharedPtrTypeCmp>();

auto Value::variables = std::map<Value::VariableName, std::shared_ptr<Value>>();
const std::shared_ptr<Value> Value::NOVALUE = std::make_shared<Value>(Value(Type::Type::NOTYPE, true, false));

auto Function::builtInFunctionMultiMap = std::multimap<Function::Signature, std::function<Function::BuiltInFunction>>();
auto Function::userDefinedFunctionMultiMap = std::multimap<Function::Signature, std::shared_ptr<SyntaxTree>>();

} // namespace CINT