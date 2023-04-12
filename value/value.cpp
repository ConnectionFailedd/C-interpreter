#include <map>
#include <memory>

#include "value.hpp"

namespace CINT {

auto Value::variables = std::map<Value::VariableName, std::shared_ptr<Value>>();

const auto Value::NOVALUE = std::make_shared<Value>(Value(nullptr, true, false));

const auto SignalValue::NORETURNVALUE = std::make_shared<SignalValue>(*NOVALUE);
const auto SignalValue::BREAK = std::make_shared<SignalValue>(*NOVALUE);
const auto SignalValue::CONTINUE = std::make_shared<SignalValue>(*NOVALUE);

} // namespace CINT::Value