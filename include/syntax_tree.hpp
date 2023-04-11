#ifndef __SYNTAX_TREE_HPP__
#define __SYNTAX_TREE_HPP__

#include <functional>
#include <memory>
#include <vector>

#include "value.hpp"

namespace CINT {

class FunctionTree;

class SyntaxTree {
private:
    class Node {
    public:
        virtual std::shared_ptr<Value> evaluate() = 0;
    };

    class ValueNode : public Node {
    private:
        std::shared_ptr<Value> __value;

    public:
        inline virtual std::shared_ptr<Value> evaluate() override final { return __value; }
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
        std::shared_ptr<FunctionTree> __userDefinedFunction;

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

    class SemicolonNode : public Node {
    private:
        std::shared_ptr<Node> __previousExpression, __nextSemicolon;

    public:
        inline virtual std::shared_ptr<Value> evaluate() override final {
            __previousExpression->evaluate();
            __nextSemicolon->evaluate();
            return Value::NOVALUE;
        }
    };

private:
    std::shared_ptr<Node>
        __root;

public:
    inline std::shared_ptr<Value> evaluate() { return __root->evaluate(); }
};

/*

*/
} // namespace CINT

#endif