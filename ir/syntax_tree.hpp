#ifndef __SYNTAX_TREE_HPP__
#define __SYNTAX_TREE_HPP__

/* ---------------------------------------------------------------------------------------------------- */

#include "type.hpp"
#include "value.hpp"
#include <memory>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::Backend {
class VirtualMachine;
}

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::IR {

/* ---------------------------------------------------------------------------------------------------- */

// class SyntaxTreeNode
// The syntax tree of a C++ expression
class SyntaxTreeNode {
    friend Backend::VirtualMachine;

public:
    inline virtual bool is_value_node() const noexcept { return false; }
    inline virtual bool is_stack_reference_node() const noexcept { return false; }
    inline virtual bool is_function_node() const noexcept { return false; }
    inline virtual bool is_sub_value_node() const noexcept { return false; }

    inline virtual const std::shared_ptr<Type> & type() const = 0;
};

// class ValueNode
// A node binded to a certain value
class ValueNode final : public SyntaxTreeNode {
    friend Backend::VirtualMachine;

private:
    std::shared_ptr<Value> __value;

public:
    inline ValueNode(const std::shared_ptr<Value> & _value) : __value(_value) {}
    inline ValueNode(const ValueNode & _other) : __value(_other.__value) {}

    inline virtual bool is_value_node() const noexcept override final { return true; }

    inline virtual const std::shared_ptr<Type> & type() const override final { return __value->type(); }
};

// class StackReferenceNode
// A node referencing a value in the stack
class StackReferenceNode final : public SyntaxTreeNode {
    friend Backend::VirtualMachine;

private:
    std::size_t __index;

    std::shared_ptr<Type> __type;

public:
    inline StackReferenceNode(std::size_t _index, const std::shared_ptr<Type> & _type) : __index(_index), __type(_type) {}
    inline StackReferenceNode(const StackReferenceNode & _other) : __index(_other.__index), __type(_other.__type) {}

    inline virtual bool is_stack_reference_node() const noexcept override final { return true; }

    inline virtual const std::shared_ptr<Type> & type() const override final { return __type; }
};

// class FunctionNode
// A node representing a function call
class FunctionNode final : public SyntaxTreeNode {
    friend Backend::VirtualMachine;

private:
    std::shared_ptr<class Function> __function;

    std::vector<std::shared_ptr<SyntaxTreeNode>> __arguments;

public:
    inline FunctionNode(const std::shared_ptr<Function> & _function, const std::vector<std::shared_ptr<SyntaxTreeNode>> & _arguments) : __function(_function), __arguments(_arguments) {}
    inline FunctionNode(const FunctionNode & _other) : __function(_other.__function), __arguments(_other.__arguments) {}

    inline virtual bool is_function_node() const noexcept override final { return true; }

    virtual const std::shared_ptr<Type> & type() const override final;
};

// class SubValueNode
// A node getting a member of an aggregated type
class SubValueNode final : public SyntaxTreeNode {
    friend Backend::VirtualMachine;

private:
    std::shared_ptr<SyntaxTreeNode> __valueNode;
    std::string __memberName;

public:
    inline SubValueNode(const std::shared_ptr<SyntaxTreeNode> & _valueNode, const std::string & _memberName) : __valueNode(_valueNode), __memberName(_memberName) {}
    inline SubValueNode(const SubValueNode & _other) : __valueNode(_other.__valueNode), __memberName(_other.__memberName) {}

    inline virtual bool is_sub_value_node() const noexcept override final { return true; }

    inline virtual const std::shared_ptr<Type> & type() const override final { return std::dynamic_pointer_cast<AggregatedType>(__valueNode->type())->get_member_type(__memberName); }
};

/* ---------------------------------------------------------------------------------------------------- */

}

/* ---------------------------------------------------------------------------------------------------- */

#endif