#include "function_signature.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::IR {

/* ---------------------------------------------------------------------------------------------------- */

bool FunctionSignature::operator<(const FunctionSignature & _rhs) const noexcept {
    if(__functionName < _rhs.__functionName) return true;
    if(_rhs.__functionName < __functionName) return false;
    for (size_t i = 0; i < __argumentTypes.size() && i < _rhs.__argumentTypes.size(); i++) {
        if(*__argumentTypes[i] < *_rhs.__argumentTypes[i]) return true;
        if(*_rhs.__argumentTypes[i] < *__argumentTypes[i]) return false;
    }
    return __argumentTypes.size() < _rhs.__argumentTypes.size();
}

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::IR