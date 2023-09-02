#include "value.hpp"
#include <ostream>

std::ostream & CINT::operator<<(std::ostream & _ost, const CINT::Value & _value) {
    if(_value.type()->is_builtin_type()) {
        if(_value.type()->type_name().base_name() == "int") return _ost << _value.get_value<int>();
    }

    return _ost << "Print failed";
}