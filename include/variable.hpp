#ifndef __VARIABLE_HPP__
#define __VARIABLE_HPP__

#include "name.hpp"
#include "value.hpp"
#include <memory>
#include <set>
#include <utility>

namespace CINT {

class Variable {
public:
    using VariableName = Name;

private:
    VariableName __variableName;
    std::shared_ptr<Value> __value;

public:
    inline Variable(const VariableName & _varibaleName, const std::shared_ptr<Value> & _value) : __variableName(_varibaleName), __value(_value) {}
    inline Variable(VariableName && _varibaleName, std::shared_ptr<Value> && _value) noexcept : __variableName(std::move(_varibaleName)), __value(std::move(_value)) {}

    inline const VariableName & variable_name() const noexcept { return __variableName; }
    inline const std::shared_ptr<Value> & value() const noexcept { return __value; }

    inline bool operator<(const Variable & _rhs) const & noexcept { return __variableName < _rhs.__variableName; }

public:
    class GlobalVariableMultiSet {
    public:
        class SharedPtrVariableCmp {
        public:
            inline bool operator()(const std::shared_ptr<Variable> & _lhs, const std::shared_ptr<Variable> & _rhs) const noexcept {
                return *_lhs < *_rhs;
            }
        };

    private:
        std::multiset<std::shared_ptr<Variable>, SharedPtrVariableCmp> __globalVaribaleMultiSet;

    public:
        inline GlobalVariableMultiSet(std::multiset<std::shared_ptr<Variable>, SharedPtrVariableCmp> && _globalVaribaleMultiSet) : __globalVaribaleMultiSet(std::move(_globalVaribaleMultiSet)) {}

        static GlobalVariableMultiSet init();

        inline void insert(const std::shared_ptr<Variable> & _src) { __globalVaribaleMultiSet.insert(_src); }
        inline void insert(std::shared_ptr<Variable> && _src) { __globalVaribaleMultiSet.insert(std::move(_src)); }
    };

public:
    static const std::shared_ptr<Variable> NOVARIABLE;

    static GlobalVariableMultiSet globalVaribaleMultiSet;
};

} // namespace CINT

#endif