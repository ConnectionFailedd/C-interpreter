#ifndef __SYNTAX_TREE_HPP__
#define __SYNTAX_TREE_HPP__

#include <cstddef>
#include <functional>
#include <memory>
#include <stack>
#include <vector>

#include "value.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class SyntaxTree {
    /* ---------------------------------------------------------------------------------------------------- */

private:
    class FunctionStack {
    private:
        class Element {
        private:
            std::vector<std::shared_ptr<Value>> __arguments;
            std::shared_ptr<Value> __returnValue;

        public:
            inline Element(const std::vector<std::shared_ptr<Value>> & _arguments, const std::shared_ptr<Value> & _returnValue) : __arguments(_arguments), __returnValue(_returnValue) {}

            inline const std::vector<std::shared_ptr<Value>> & get_arguments() { return __arguments; }
            inline std::shared_ptr<Value> & get_return_value() { return __returnValue; }
        };

        std::stack<Element> __functionStack;

    public:
        inline FunctionStack() : __functionStack() {}

        inline const std::vector<std::shared_ptr<Value>> & get_arguments() { return __functionStack.top().get_arguments(); }
        inline std::shared_ptr<Value> & get_return_value() { return __functionStack.top().get_return_value(); }

        inline void push(const std::vector<std::shared_ptr<Value>> & _arguments, const std::shared_ptr<Value> & _returnValue) { return __functionStack.push(Element(_arguments, _returnValue)); }
        inline void pop() { return __functionStack.pop(); }
    };

public:
    inline static auto functionStack = FunctionStack();

private:
    inline static bool continueSignal = false;
    inline static bool breakSignal = false;
    inline static bool returnSignal = false;

    /* ---------------------------------------------------------------------------------------------------- */

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
        inline ArgumentNode(std::size_t _index) : __index(_index) {}

        inline virtual std::shared_ptr<Value> evaluate() override final { return functionStack.get_arguments()[__index]; }
    };

    class BuiltInFunctionNode : public Node {
    private:
        std::function<std::shared_ptr<Value>(std::vector<std::shared_ptr<Value>>)> __builtInFunction;

        std::vector<std::shared_ptr<Node>> __arguments;

    public:
        inline BuiltInFunctionNode(const std::function<std::shared_ptr<Value>(std::vector<std::shared_ptr<Value>>)> & _builtInFunction, const std::vector<std::shared_ptr<Node>> & _arguments) : __builtInFunction(_builtInFunction), __arguments(_arguments) {}

        virtual std::shared_ptr<Value> evaluate() override final;
    };

    class UserDefinedFunctionNode : public Node {
    private:
        std::shared_ptr<SyntaxTree> __userDefinedFunction;

        std::vector<std::shared_ptr<Node>> __arguments;

    public:
        inline UserDefinedFunctionNode(const std::shared_ptr<SyntaxTree> & _userDefinedFunction, const std::vector<std::shared_ptr<Node>> & _arguments) : __userDefinedFunction(_userDefinedFunction), __arguments(_arguments) {}

        virtual std::shared_ptr<Value> evaluate() override final;
    };

    class IfNode : public Node {
    private:
        std::shared_ptr<Node> __condition;
        std::shared_ptr<Node> __trueBranch, __falseBranch;

    public:
        inline IfNode(const std::shared_ptr<Node> & _condition, const std::shared_ptr<Node> & _trueBranch, const std::shared_ptr<Node> & _falseBranch) : __condition(_condition), __trueBranch(_trueBranch), __falseBranch(_falseBranch) {}

        virtual std::shared_ptr<Value> evaluate() override final;
    };

    class WhileNode : public Node {
    private:
        std::shared_ptr<Node> __condition;
        std::shared_ptr<Node> __trueBranch, __falseBranch;

    public:
        inline WhileNode(const std::shared_ptr<Node> & _condition, const std::shared_ptr<Node> & _trueBranch, const std::shared_ptr<Node> & _falseBranch) : __condition(_condition), __trueBranch(_trueBranch), __falseBranch(_falseBranch) {}

        virtual std::shared_ptr<Value> evaluate() override final;
    };

    class ForNode : public Node {
    private:
        std::shared_ptr<Node> __initialization, __condition, __increment;
        std::shared_ptr<Node> __trueBranch, __falseBranch;

    public:
        inline ForNode(const std::shared_ptr<Node> & _initialization, const std::shared_ptr<Node> & _condition, const std::shared_ptr<Node> & _increment, const std::shared_ptr<Node> & _trueBranch, const std::shared_ptr<Node> & _falseBranch) : __initialization(_initialization), __condition(_condition), __increment(_increment), __trueBranch(_trueBranch), __falseBranch(_falseBranch) {}

        virtual std::shared_ptr<Value> evaluate() override final;
    };

    class BreakNode : public Node {
    public:
        inline BreakNode() {}

        virtual std::shared_ptr<Value> evaluate() override final {
            breakSignal = true;
            return Value::NOVALUE;
        }
    };

    class ContinueNode : public Node {
    public:
        inline ContinueNode() {}

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
            functionStack.get_return_value() = __returnValue->evaluate();
            return __returnValue->evaluate();
        }
    };

    class SemicolonNode : public Node {
    private:
        std::shared_ptr<Node> __previousExpression, __nextSemicolon;

    public:
        inline SemicolonNode(const std::shared_ptr<Node> & _previousExpression, const std::shared_ptr<Node> & _nextSemicolon) : __previousExpression(_previousExpression), __nextSemicolon(_nextSemicolon) {}

        virtual std::shared_ptr<Value> evaluate() override final;
    };

private:
    std::shared_ptr<Node> __root;

public:
    inline SyntaxTree(const std::shared_ptr<Node> & _root) : __root(_root) {}

    inline std::shared_ptr<Value> evaluate() { return __root->evaluate(); }

    inline static std::shared_ptr<Node> make_fixed_value_node(const std::shared_ptr<Value> & _value) {
        return std::make_shared<FixedValueNode>(FixedValueNode(_value));
    }
    inline static std::shared_ptr<Node> make_argument_node(std::size_t _index) {
        return std::make_shared<ArgumentNode>(ArgumentNode(_index));
    }
    inline static std::shared_ptr<Node> make_built_in_function_node(const std::function<std::shared_ptr<Value>(std::vector<std::shared_ptr<Value>>)> & _builtInFunction, const std::vector<std::shared_ptr<Node>> & _arguments) {
        return std::make_shared<BuiltInFunctionNode>(BuiltInFunctionNode(_builtInFunction, _arguments));
    }
    inline static std::shared_ptr<Node> make_user_defined_function_node(const std::shared_ptr<SyntaxTree> & _userDefinedFunction, const std::vector<std::shared_ptr<Node>> & _arguments) {
        return std::make_shared<UserDefinedFunctionNode>(UserDefinedFunctionNode(_userDefinedFunction, _arguments));
    }
    inline static std::shared_ptr<Node> make_if_node(const std::shared_ptr<Node> & _condition, const std::shared_ptr<Node> & _trueBranch, const std::shared_ptr<Node> & _falseBranch) {
        return std::make_shared<IfNode>(IfNode(_condition, _trueBranch, _falseBranch));
    }
    inline static std::shared_ptr<Node> make_while_node(const std::shared_ptr<Node> & _condition, const std::shared_ptr<Node> & _trueBranch, const std::shared_ptr<Node> & _falseBranch) {
        return std::make_shared<WhileNode>(WhileNode(_condition, _trueBranch, _falseBranch));
    }
    inline static std::shared_ptr<Node> make_for_node(const std::shared_ptr<Node> & _initialization, const std::shared_ptr<Node> & _condition, const std::shared_ptr<Node> & _increment, const std::shared_ptr<Node> & _trueBranch, const std::shared_ptr<Node> & _falseBranch) {
        return std::make_shared<ForNode>(ForNode(_initialization, _condition, _increment, _trueBranch, _falseBranch));
    }
    inline static std::shared_ptr<Node> make_break_node() {
        return std::make_shared<BreakNode>(BreakNode());
    }
    inline static std::shared_ptr<Node> make_continue_node() {
        return std::make_shared<ContinueNode>(ContinueNode());
    }
    inline static std::shared_ptr<Node> make_return_node(const std::shared_ptr<Node> & _returnValue) {
        return std::make_shared<ReturnNode>(ReturnNode(_returnValue));
    }
    inline static std::shared_ptr<Node> make_semicolon_node(const std::shared_ptr<Node> & _previousExpression, const std::shared_ptr<Node> & _nextSemicolon) {
        return std::make_shared<SemicolonNode>(SemicolonNode(_previousExpression, _nextSemicolon));
    }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif