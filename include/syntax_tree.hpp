#ifndef __SYNTAX_TREE_HPP__
#define __SYNTAX_TREE_HPP__

#include <cstddef>
#include <functional>
#include <memory>
#include <stack>
#include <vector>

#include "value.hpp"

namespace CINT {

class SyntaxTree {
public:
    class StackNode {
    private:
        std::vector<std::shared_ptr<Value>> __arguments;
        std::shared_ptr<Value> __returnValue;

    public:
        inline StackNode() {}
        inline StackNode(const std::vector<std::shared_ptr<Value>> & _arguments, const std::shared_ptr<Value> & _returnValue) : __arguments(_arguments), __returnValue(_returnValue) {}

        std::vector<std::shared_ptr<Value>> get_arguments() { return __arguments; }
        std::shared_ptr<Value> & get_return_value() { return __returnValue; }
    };

public:
    inline static auto functionStack = std::stack<StackNode>();

private:
    inline static bool continueSignal = false;
    inline static bool breakSignal = false;
    inline static bool returnSignal = false;

private:
    class Node {
    public:
        virtual std::shared_ptr<Value> evaluate() = 0;
    };

    class FixedValueNode : public Node {
    private:
        std::shared_ptr<Value> __value;

    public:
        inline FixedValueNode(const std::shared_ptr<Value> & _value) : __value(_value) {}

        inline virtual std::shared_ptr<Value> evaluate() override final { return __value; }
    };

    class ArgumentNode : public Node {
    private:
        std::size_t __index;

    public:
        inline virtual std::shared_ptr<Value> evaluate() override final { return functionStack.top().get_arguments()[__index]; }
    };

    class BuiltInFunctionNode : public Node {
    private:
        std::function<std::shared_ptr<Value>(std::vector<std::shared_ptr<Value>>)> __builtInFunction;

        std::vector<std::shared_ptr<Node>> __children;

    public:
        virtual std::shared_ptr<Value> evaluate() override final;
    };

    class UserDefinedFunctionNode : public Node {
    private:
        std::shared_ptr<SyntaxTree> __userDefinedFunction;

        std::vector<std::shared_ptr<Node>> __children;

    public:
        virtual std::shared_ptr<Value> evaluate() override final;
    };

    class IfNode : public Node {
    private:
        std::shared_ptr<Node> __condition;
        std::shared_ptr<Node> __trueBranch, __falseBranch;

    public:
        virtual std::shared_ptr<Value> evaluate() override final;
    };

    class WhileNode : public Node {
    private:
        std::shared_ptr<Node> __condition;
        std::shared_ptr<Node> __trueBranch, __falseBranch;

    public:
        virtual std::shared_ptr<Value> evaluate() override final;
    };

    class ForNode : public Node {
    private:
        std::shared_ptr<Node> __initialization, __condition, __increment;
        std::shared_ptr<Node> __trueBranch, __falseBranch;

    public:
        virtual std::shared_ptr<Value> evaluate() override final;
    };

    class BreakNode : public Node {
    public:
        virtual std::shared_ptr<Value> evaluate() override final {
            breakSignal = true;
            return Value::NOVALUE;
        }
    };

    class ContinueNode : public Node {
    public:
        virtual std::shared_ptr<Value> evaluate() override final {
            continueSignal = true;
            return Value::NOVALUE;
        }
    };

    class ReturnNode : public Node {
    private:
        std::shared_ptr<Node> __returnValue;

    public:
        inline ReturnNode(const std::shared_ptr<Node> & _returnValue) : __returnValue(_returnValue) {}
        virtual std::shared_ptr<Value> evaluate() override final {
            returnSignal = true;
            functionStack.top().get_return_value() = __returnValue->evaluate();
            return __returnValue->evaluate();
        }
    };

    class SemicolonNode : public Node {
    private:
        std::shared_ptr<Node> __previousExpression, __nextSemicolon;

    public:
        virtual std::shared_ptr<Value> evaluate() override final;
    };

private:
    std::shared_ptr<Node> __root;

public:
    inline SyntaxTree(const std::shared_ptr<Node> & _root) : __root(_root) {}

    inline std::shared_ptr<Value> evaluate() { return __root->evaluate(); }

    static std::shared_ptr<Node> make_fixed_value_node(const std::shared_ptr<Value> & _value) {
        return std::make_shared<FixedValueNode>(FixedValueNode(_value));
    }
    static std::shared_ptr<Node> make_return_node(const std::shared_ptr<Node> & _returnValue) {
        return std::make_shared<ReturnNode>(ReturnNode(_returnValue));
    }
};

} // namespace CINT

#endif