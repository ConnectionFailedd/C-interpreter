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
    friend class SubValue;
private:
    std::shared_ptr<Types::Type> __type;

    void * __valuePointer;

    bool __isConst;
    bool __isLeft;

public:
    inline Value(const std::shared_ptr<Types::Type> & _type, bool _isConst, bool _isLeft) : __type(_type), __valuePointer(::operator new(_type->size(), _type->align())), __isConst(_isConst), __isLeft(_isLeft) {}
    inline Value(Value && _src) noexcept : __type(_src.__type), __isConst(_src.__isConst), __isLeft(_src.__isLeft) {
        __valuePointer = _src.__valuePointer;
        _src.__valuePointer = nullptr;
    }
    inline virtual ~Value() {
        if(__valuePointer != nullptr) {
            ::operator delete(__valuePointer, __type->size(), __type->align());
            __valuePointer = nullptr;
        }
    }

    inline const std::shared_ptr<Types::Type> & type() const & noexcept { return __type; }
    inline bool is_const() const noexcept { return __isConst; }
    inline bool is_left() const noexcept { return __isLeft; }

    template<class T>
    inline T get_value() const {
        return *(T *)(__valuePointer);
    }
    template<class T>
    inline void set_value(T _src) const {
        *(T *)(__valuePointer) = _src;
    }

public:
    const static std::shared_ptr<Value> NOVALUE;
};

class SubValue : public Value {
public:
    inline virtual ~SubValue() override {
        __valuePointer = nullptr;
    }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __VALUE_HPP__ */