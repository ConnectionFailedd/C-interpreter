#include <memory>
#include <set>

#include "value.hpp"
#include "variable.hpp"

namespace CINT {

Variable::GlobalVariableMultiSet Variable::GlobalVariableMultiSet::init() {
    auto res = std::multiset<std::shared_ptr<Variable>, SharedPtrVariableCmp>();

    return res;
}

std::shared_ptr<Variable> Variable::GlobalVariableMultiSet::find(const UnconfirmedName & _key) {
    auto keyVariable = std::make_shared<Variable>(VariableName(Scope::NOSCOPE, _key.base_name()), Value::NOVALUE);

    auto equalRange = __globalVariableMultiSet.equal_range(keyVariable);
    for(auto begin = equalRange.first, end = equalRange.second; begin != end; begin++) {
        if((*begin)->variable_name() == _key) return *begin;
    }

    return NOVARIABLE;
}

}