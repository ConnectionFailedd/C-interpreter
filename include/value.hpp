#ifndef __VALUE_HPP__
#define __VALUE_HPP__

#include <memory>

#include "type.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class Value {
private:
    std::shared_ptr<Type> __type;

    void * __valuePointer;

public:
    inline Value(std::shared_ptr<Type> _type) : __type(_type), __valuePointer(::operator new(_type->size(), _type->align())) {}
    inline ~Value() { ::operator delete(__valuePointer, __type->size(), __type->align()); }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __VALUE_HPP__ */