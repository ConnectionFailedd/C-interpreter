#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__

/* ---------------------------------------------------------------------------------------------------- */

#include "function_signature.hpp"
#include "instruction_flow.hpp"
#include "type.hpp"
#include "value.hpp"
#include <functional>
#include <memory>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::Backend {
class VirtualMachine;
}

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::IR {

/* ---------------------------------------------------------------------------------------------------- */

class Function {
    friend Backend::VirtualMachine;

public:
    inline virtual const std::shared_ptr<Type> & return_type() const = 0;
    inline virtual const FunctionSignature & function_signature() const noexcept = 0;

    inline virtual bool is_function_key() const noexcept { return false; }
    inline virtual bool is_native_function() const noexcept { return false; }
    inline virtual bool is_user_defined_function() const noexcept { return false; }

    inline virtual bool operator<(const Function & _other) const noexcept { return function_signature() < _other.function_signature(); }
};

class FunctionKey final : public Function {
    friend Backend::VirtualMachine;

private:
    FunctionSignature __signature;

public:
    inline FunctionKey(const FunctionSignature & _signature) : __signature(_signature) {}
    inline FunctionKey(const FunctionKey & _other) : __signature(_other.__signature) {}

    virtual const std::shared_ptr<Type> & return_type() const override final;
    inline virtual const FunctionSignature & function_signature() const noexcept override final { return __signature; }

    inline virtual bool is_function_key() const noexcept override final { return true; }

    using Function::operator<;
};

class NativeFunction final : public Function {
    friend Backend::VirtualMachine;

public:
    using NativeFunctionType = std::shared_ptr<Value>(std::vector<std::shared_ptr<Value>>);

private:
    std::shared_ptr<Type> __returnType;
    FunctionSignature __signature;
    std::function<NativeFunctionType> __function;

public:
    inline NativeFunction(const std::shared_ptr<Type> & _returnType, const FunctionSignature & _signature, const std::function<NativeFunctionType> & _function) : __returnType(_returnType), __signature(_signature), __function(_function) {}
    inline NativeFunction(const NativeFunction & _other) : __returnType(_other.__returnType), __signature(_other.__signature), __function(_other.__function) {}

    inline virtual const std::shared_ptr<Type> & return_type() const override final { return __returnType; }
    inline virtual const FunctionSignature & function_signature() const noexcept override final { return __signature; }

    inline virtual bool is_native_function() const noexcept override final { return true; }

    using Function::operator<;
};

class UserDefinedFunction final : public Function {
    friend Backend::VirtualMachine;

private:
    std::shared_ptr<Type> __returnType;
    FunctionSignature __signature;
    InstructionFlow __functionBody;
    std::vector<std::shared_ptr<Type>> __localVariableTypes;

public:
    inline UserDefinedFunction(const std::shared_ptr<Type> & _returnType, const FunctionSignature & _signature, const InstructionFlow & _functionBody, const std::vector<std::shared_ptr<Type>> & _localVariableTypes) : __returnType(_returnType), __signature(_signature), __functionBody(_functionBody), __localVariableTypes(_localVariableTypes) {}
    inline UserDefinedFunction(const UserDefinedFunction & _other) : __returnType(_other.__returnType), __signature(_other.__signature), __functionBody(_other.__functionBody), __localVariableTypes(_other.__localVariableTypes) {}

    inline virtual const std::shared_ptr<Type> & return_type() const override final { return __returnType; }
    inline virtual const FunctionSignature & function_signature() const noexcept override final { return __signature; }

    inline virtual bool is_user_defined_function() const noexcept override final { return true; }

    using Function::operator<;
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::IR

/* ---------------------------------------------------------------------------------------------------- */

#endif