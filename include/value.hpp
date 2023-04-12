#ifndef __VALUE_HPP__
#define __VALUE_HPP__

#include <map>
#include <memory>

#include "type.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class Value {
private:
    const std::shared_ptr<Type::Type> __type;

    void * const __valuePointer;

    const bool __isConst;
    const bool __isLeft;

public:
    using VariableName = Name;
    static std::map<VariableName, std::shared_ptr<Value>> variables;

    const static std::shared_ptr<Value> NOVALUE;

public:
    inline Value(std::shared_ptr<Type::Type> _type, bool _isConst, bool _isLeft) : __type(_type), __valuePointer(::operator new(_type->size(), _type->align())), __isConst(_isConst), __isLeft(_isLeft) {}
    inline ~Value() {
        if(__valuePointer != nullptr) ::operator delete(__valuePointer, __type->size(), __type->align());
    }

    inline virtual bool is_signal() { return false; }

    template<class T>
    inline T get_value() const {
        return *(T *)(__valuePointer);
    }

    template<bool>
    inline bool get_value() const {
        return *(int *)(__valuePointer);
    }
};

class SignalValue : public Value {
public:
    const static std::shared_ptr<SignalValue> NORETURNVALUE;
    const static std::shared_ptr<SignalValue> BREAK;
    const static std::shared_ptr<SignalValue> CONTINUE;

public:
    inline SignalValue(const Value & _value, bool _isReturn) : Value(_value) {}
    inline ~SignalValue() = default;

    inline virtual bool is_signal() override final { return true; }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __VALUE_HPP__ */