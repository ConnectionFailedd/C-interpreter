#include <map>

#include "value.hpp"

namespace CINT {

auto Value::variables = std::map<Value::VariableName, std::shared_ptr<Value>>();

const auto Value::NOVALUE = Value(nullptr, true, false);

} // namespace CINT::Value