#include <map>
#include <memory>

#include "value.hpp"

namespace CINT {

auto Value::variables = std::map<Value::VariableName, std::shared_ptr<Value>>();

const auto Value::NOVALUE = std::make_shared<Value>(Value(nullptr, true, false));

} // namespace CINT::Value