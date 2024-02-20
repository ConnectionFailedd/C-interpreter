#ifndef __VALUE_HPP__
#define __VALUE_HPP__

/* ---------------------------------------------------------------------------------------------------- */

#include "type.hpp"
#include <memory>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::IR {

/* ---------------------------------------------------------------------------------------------------- */

// class Value
// Holds a value of a certain type
class Value {
    friend class SubValue;

private:
    std::shared_ptr<Type> __type;

    void * __value;

public:
    inline Value() : __type(nullptr), __value(nullptr) {}
    inline Value(const std::shared_ptr<Type> & _type) : __type(_type) { __value = __type->size() == 0 ? nullptr : operator new(__type->size(), __type->alignment()); }
    inline Value(Value && _other) : __type(std::move(_other.__type)), __value(_other.__value) { _other.__value = nullptr; }
    inline ~Value() {
        if(__value != nullptr) {
            operator delete(__value, __type->alignment());
            __value = nullptr;
        }
    }

    inline const std::shared_ptr<Type> & type() const { return __type; }

    template<class __TYPE>
    inline __TYPE & as() { return *reinterpret_cast<__TYPE *>(__value); }
    template<class __TYPE>
    inline const __TYPE & as() const { return *reinterpret_cast<const __TYPE *>(__value); }
};

// class SubValue
// Points to a member of an aggregated type
class SubValue : public Value {
public:
    inline SubValue(const std::shared_ptr<Value> & _value, const std::string & _name) {
        __type = dynamic_cast<AggregatedType *>(_value->__type.get())->get_member_type(_name);
    }
    inline SubValue(SubValue && _other) : Value(std::move(_other)) {}
    inline ~SubValue() { __value = nullptr; }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::IR

/* ---------------------------------------------------------------------------------------------------- */

#endif