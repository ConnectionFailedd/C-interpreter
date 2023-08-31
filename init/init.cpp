#include <map>
#include <memory>
#include <new>
#include <set>
#include <string>
#include <vector>

#include "function.hpp"
#include "name.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "value.hpp"
#include "variable.hpp"

namespace CINT {

auto Scope::currentScopeStack = std::vector<std::shared_ptr<Scope>>({std::make_shared<Scope>()});
const auto Scope::NOSCOPE = std::make_shared<Scope>(std::vector<std::string>());
const auto Scope::GLOBAL_SCOPE = currentScopeStack[0];
auto Scope::scopeMultiSet = ScopeMultiSet::init();

const auto Name::NONAME = Name(Scope::NOSCOPE, "");

const auto Types::Type::NOTYPE = std::make_shared<Type>();
auto Types::Type::typeMultiSet = Type::TypeMultiSet::init();

const auto Value::NOVALUE = std::make_shared<Value>(Value(Types::Type::NOTYPE, true, false));

const auto Variable::NOVARIABLE = std::make_shared<Variable>(Name::NONAME, Value::NOVALUE);
auto Variable::globalVaribaleMultiSet = Variable::GlobalVariableMultiSet::init();

const auto Function::NOFUNCTION = std::make_shared<Function>();
auto Function::functionStack = FunctionStack();
auto Function::functionMultiSet = Function::FunctionMultiSet::init();

} // namespace CINT