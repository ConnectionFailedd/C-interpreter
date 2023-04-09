#ifndef __VALUE_HPP__
#define __VALUE_HPP__

#include <map>
#include <memory>

#include "type.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {
namespace Value {

/* ---------------------------------------------------------------------------------------------------- */

class Value {
private:
    const std::shared_ptr<Type::Type> __type;

    void * const __valuePointer;

    const bool __isConst;
    const bool __isLeft;

public:
    inline Value(std::shared_ptr<Type::Type> _type, bool _isConst, bool _isLeft) : __type(_type), __valuePointer(::operator new(_type->size(), _type->align())), __isConst(_isConst), __isLeft(_isLeft) {}
    inline ~Value() { ::operator delete(__valuePointer, __type->size(), __type->align()); }
};

/* ---------------------------------------------------------------------------------------------------- */

using VariableName = Name;
extern std::map<VariableName, Value> variables;

/* ---------------------------------------------------------------------------------------------------- */

}
} // namespace CINT::Value

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __VALUE_HPP__ */