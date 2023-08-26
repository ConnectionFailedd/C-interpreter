#ifndef __VALUE_HPP__
#define __VALUE_HPP__

#include <map>
#include <memory>
#include <type_traits>

#include "type.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class Value {
private:
    const std::shared_ptr<Type::Type> __type;

    void * __valuePointer;

    const bool __isConst;
    const bool __isLeft;

public:
    using VariableName = Name;
    static std::map<VariableName, std::shared_ptr<Value>> variables;

    const static std::shared_ptr<Value> NOVALUE;

public:
    inline Value(const std::shared_ptr<Type::Type> & _type, bool _isConst, bool _isLeft) : __type(_type), __valuePointer(::operator new(_type->size(), _type->align())), __isConst(_isConst), __isLeft(_isLeft) {}
    inline Value(Value && _src) : __type(_src.__type), __isConst(_src.__isConst), __isLeft(_src.__isLeft) {
        __valuePointer = _src.__valuePointer;
        _src.__valuePointer = nullptr;
    }
    inline ~Value() {
        if(__valuePointer != nullptr) {
            ::operator delete(__valuePointer, __type->size(), __type->align());
            __valuePointer = nullptr;
        }
    }

    template<class T>
    inline T get_value() const {
        return *(T *)(__valuePointer);
    }
    template<class T>
    inline void set_value(T _src) const {
        *(T *)(__valuePointer) = _src;
    }

    template<bool>
    inline bool get_value() const {
        return *(int *)(__valuePointer);
    }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __VALUE_HPP__ */