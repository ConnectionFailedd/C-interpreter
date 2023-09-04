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

const auto Type::NOTYPE = std::make_shared<const Type>(Type::TypeName(""));

const auto Value::NOVALUE = std::make_shared<const Value>(Value(Type::NOTYPE, true, false));

const auto Variable::NOVARIABLE = std::make_shared<const Variable>(VariableName(""), Value::NOVALUE);

const auto Function::NOFUNCTION = std::make_shared<const Function>(Function::Signature(""));
auto Function::functionStack = FunctionStack();

auto Scope::currentScopeStack = std::vector<std::shared_ptr<Scope>>({std::make_shared<Scope>(Scope::ScopeName(__GLOBAL_SCOPE_NAME__))});
const auto Scope::NOSCOPE = std::make_shared<Scope>("");
} // namespace CINT