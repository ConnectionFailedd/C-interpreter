#include <memory>
#include <set>

#include "variable.hpp"

namespace CINT {

Variable::GlobalVariableMultiSet Variable::GlobalVariableMultiSet::init() {
    auto res = std::multiset<std::shared_ptr<Variable>, SharedPtrVariableCmp>();

    return res;
}

}