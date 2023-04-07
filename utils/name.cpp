#include "name.hpp"
#include "scope.hpp"

namespace CINT {

const auto Name::noName = Name(Scope(), "");

} // namespace CINT