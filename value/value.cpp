#include <map>

#include "value.hpp"

namespace CINT {
namespace Value {

auto variables = std::map<CINT::Value::VariableName, CINT::Value::Value>();

}
} // namespace CINT::Value