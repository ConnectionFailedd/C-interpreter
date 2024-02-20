#ifndef __FUNCTION_SIGNATURE_HPP__
#define __FUNCTION_SIGNATURE_HPP__

/* ---------------------------------------------------------------------------------------------------- */

#include "type.hpp"
#include <memory>
#include <string>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::IR {

/* ---------------------------------------------------------------------------------------------------- */

class FunctionSignature {
private:
    std::string __functionName;
    std::vector<std::shared_ptr<Type>> __argumentTypes;

public:
    FunctionSignature(const std::string & _functionName, const std::vector<std::shared_ptr<Type>> & _argumentTypes) : __functionName(_functionName), __argumentTypes(_argumentTypes) {}
    FunctionSignature(const FunctionSignature & _other) : __functionName(_other.__functionName), __argumentTypes(_other.__argumentTypes) {}

    inline const std::string & function_name() const noexcept { return __functionName; }
    inline const std::vector<std::shared_ptr<Type>> & argument_types() const noexcept { return __argumentTypes; }

    bool operator<(const FunctionSignature & _rhs) const noexcept;
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::IR

/* ---------------------------------------------------------------------------------------------------- */

#endif