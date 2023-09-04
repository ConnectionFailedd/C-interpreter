#ifndef __VARIABLE_HPP__
#define __VARIABLE_HPP__

#include <memory>
#include <set>
#include <utility>

#include "name.hpp"
#include "value.hpp"

namespace CINT {

class Variable {
public:
    using VariableName = Name;

private:
    VariableName __variableName;
    std::shared_ptr<const Value> __value;

public:
    inline Variable(const VariableName & _varibaleName, const std::shared_ptr<const Value> & _value) : __variableName(_varibaleName), __value(_value) {}
    inline Variable(VariableName && _varibaleName, std::shared_ptr<const Value> && _value) noexcept : __variableName(std::move(_varibaleName)), __value(std::move(_value)) {}

    inline const VariableName & variable_name() const noexcept { return __variableName; }
    inline const std::shared_ptr<const Value> & value() const noexcept { return __value; }

    inline bool operator<(const Variable & _rhs) const noexcept { return __variableName < _rhs.__variableName; }

public:
    class VariableSet {
    public:
        class SharedPtrVariableCmp {
        public:
            inline bool operator()(const std::shared_ptr<const Variable> & _lhs, const std::shared_ptr<const Variable> & _rhs) const noexcept {
                return *_lhs < *_rhs;
            }
        };

    private:
        std::set<std::shared_ptr<const Variable>, SharedPtrVariableCmp> __variableSet;

    public:
        inline VariableSet() : __variableSet() {}
        inline VariableSet(std::set<std::shared_ptr<const Variable>, SharedPtrVariableCmp> && _variableSet) : __variableSet(std::move(_variableSet)) {}

        inline bool insert(const std::shared_ptr<const Variable> & _src) { return __variableSet.insert(_src).second; }
        inline bool insert(std::shared_ptr<const Variable> && _src) { return __variableSet.insert(std::move(_src)).second; }
        inline const std::shared_ptr<const Variable> & find(const VariableName & _key) const {
            auto iter = __variableSet.find(std::make_shared<const Variable>(Variable(_key, Value::NOVALUE)));
            if(iter != __variableSet.end()) {
                return *iter;
            }
            return NOVARIABLE;
        }
    };

public:
    static const std::shared_ptr<const Variable> NOVARIABLE;
};

} // namespace CINT

#endif