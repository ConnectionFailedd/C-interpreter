#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <utility>
#include <vector>

#include "name.hpp"
#include "type.hpp"
#include "value.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class SyntaxTree;

class Function {
public:
    using FunctionName = Name;

public:
    class Signature {
    private:
        FunctionName __functionName;
        std::shared_ptr<const Type> __returnType;
        std::vector<std::shared_ptr<const Type>> __argumentTypes;

    public:
        Signature(const FunctionName & _functionName) : __functionName(_functionName), __returnType(), __argumentTypes() {}
        Signature(FunctionName && _functionName) : __functionName(std::move(_functionName)), __returnType(), __argumentTypes() {}
        Signature(const FunctionName & _functionName, const std::shared_ptr<const Type> & _returnType, const std::vector<std::shared_ptr<const Type>> & _argumentTypes) : __functionName(_functionName), __returnType(_returnType), __argumentTypes(_argumentTypes) {}
        Signature(FunctionName && _functionName, const std::shared_ptr<const Type> & _returnType, const std::vector<std::shared_ptr<const Type>> & _argumentTypes) : __functionName(std::move(_functionName)), __returnType(_returnType), __argumentTypes(_argumentTypes) {}

        inline const FunctionName & function_name() const noexcept { return __functionName; }
        inline const std::shared_ptr<const Type> & return_type() const noexcept { return __returnType; }
        inline const std::vector<std::shared_ptr<const Type>> & argument_types() const noexcept { return __argumentTypes; }

        inline bool operator<(const Signature & _rhs) const noexcept { return __functionName < _rhs.__functionName; }
    };

private:
    Signature __functionSignature;

public:
    inline Function(const Signature & _functionSignature) : __functionSignature(_functionSignature) {}
    inline Function(Signature && _functionSignature) : __functionSignature(std::move(_functionSignature)) {}

    inline virtual const Signature & function_signature() const noexcept { return __functionSignature; }

    inline virtual std::shared_ptr<const Value> execute() const { return Value::NOVALUE; }

    inline virtual bool operator<(const Function & _rhs) const noexcept { return __functionSignature < _rhs.__functionSignature; }

public:
    const static std::shared_ptr<const Function> NOFUNCTION;

public:
    class FunctionSet {
    public:
        class SharedPtrFunctionCmp {
        public:
            bool operator()(const std::shared_ptr<const Function> & _lhs, const std::shared_ptr<const Function> & _rhs) const noexcept {
                return *_lhs < *_rhs;
            }
        };

    private:
        std::set<std::shared_ptr<const Function>, SharedPtrFunctionCmp> __functionSet;

    public:
        inline FunctionSet() : __functionSet() {}
        inline FunctionSet(std::set<std::shared_ptr<const Function>, SharedPtrFunctionCmp> && _functionSet) : __functionSet(std::move(_functionSet)) {}

        inline bool insert(const std::shared_ptr<const Function> & _src) { return __functionSet.insert(_src).second; }
        inline bool insert(std::shared_ptr<const Function> && _src) { return __functionSet.insert(std::move(_src)).second; }
        inline const std::shared_ptr<const Function> & find(const Signature & _key) const {
            auto iter = __functionSet.find(std::make_shared<const Function>(_key));
            if(iter != __functionSet.end()) return *iter;
            return NOFUNCTION;
        }
    };

public:
    class FunctionStack {
    private:
        class Element {
        private:
            std::vector<std::shared_ptr<const Value>> __arguments;

        public:
            inline Element(const std::vector<std::shared_ptr<const Value>> & _arguments) : __arguments(_arguments) {}
            inline Element(std::vector<std::shared_ptr<const Value>> && _arguments) : __arguments(std::move(_arguments)) {}

            inline std::vector<std::shared_ptr<const Value>> & arguments() { return __arguments; }
        };

        std::stack<Element> __functionStack;

        bool __breakSignal;
        bool __continueSignal;
        bool __returnSignal;

    public:
        inline FunctionStack() : __functionStack() {}

        inline std::vector<std::shared_ptr<const Value>> & arguments() { return __functionStack.top().arguments(); }

        inline bool & break_signal() { return __breakSignal; }
        inline bool & continue_signal() { return __continueSignal; }
        inline bool & return_signal() { return __returnSignal; }

        inline void push(std::vector<std::shared_ptr<const Value>> && _arguments) { return __functionStack.push(Element(std::move(_arguments))); }
        inline void pop() { return __functionStack.pop(); }
    };

    static FunctionStack functionStack;
};

/* ---------------------------------------------------------------------------------------------------- */

class UserDefinedFunction : public Function {
private:
    std::shared_ptr<const SyntaxTree> __functionBody;

public:
    inline UserDefinedFunction(const Signature & _functionSignature, const std::shared_ptr<const SyntaxTree> & _functionBody) : Function(_functionSignature), __functionBody(_functionBody) {}
    inline UserDefinedFunction(Signature && _functionSignature, const std::shared_ptr<const SyntaxTree> & _functionBody) : Function(std::move(_functionSignature)), __functionBody(_functionBody) {}

    virtual std::shared_ptr<const Value> execute() const override final;
};

/* ---------------------------------------------------------------------------------------------------- */

class BuiltInFunction : public Function {
public:
    using BuiltInFunctionType = std::shared_ptr<const Value>();

private:
    std::shared_ptr<const std::function<BuiltInFunctionType>> __functionBody;

public:
    inline BuiltInFunction(const Signature & _functionSignature, BuiltInFunctionType _functionBody) : Function(_functionSignature), __functionBody(std::make_shared<std::function<BuiltInFunctionType>>(_functionBody)) {}
    inline BuiltInFunction(Signature && _functionSignature, BuiltInFunctionType _functionBody) : Function(std::move(_functionSignature)), __functionBody(std::make_shared<std::function<BuiltInFunctionType>>(_functionBody)) {}

    inline virtual std::shared_ptr<const Value>execute() const override final { return __functionBody->operator()(); }

public:
    template<class _T>
    static BuiltInFunctionType add;

    template<class _T>
    static BuiltInFunctionType assign;

};

// namespace BuiltInFunctions {
//
// template<class _T>
// BuiltInFunction increment;
//
// template<class _T>
// BuiltInFunction decrement;
//
// template<class _Tt, class _Ts>
// BuiltInFunction cast;
//
// template<class _T>
// BuiltInFunction minus;
//
// template<class _T>
// BuiltInFunction logic_not;
//
// template<class _T>
// BuiltInFunction bitwise_not;
//
// template<class _T>
// BuiltInFunction dereference;
//
// template<class _T>
// BuiltInFunction assign;
//
// template<class _T>
// BuiltInFunction add;
//
// }

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif