#include "scope.hpp"

namespace CINT {

auto Scope::currentScope = Scope();
auto Scope::scopeStack = std::stack<Scope>();

} // namespace CINT