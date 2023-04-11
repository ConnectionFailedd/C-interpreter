#ifndef __SYNTAX_TREE_HPP__
#define __SYNTAX_TREE_HPP__

#include <functional>
#include <memory>
#include <vector>

#include "value.hpp"

namespace CINT {

class SyntaxTree {
private:
    class Node {
    public:
        virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) = 0;

        inline virtual bool is_return() { return false; }
    };

    class BindedValueNode : Node {
    private:
        std::shared_ptr<Value> __value;

    public:
        inline virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) override final {
            return __value;
        }
    };

    class ArgumentNode : Node {
    private:
        std::size_t __index;

    public:
        inline virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) override final {
            return _arguments[__index];
        }
    };

    class EmbeddedFunctionNode : Node {
    private:
        std::function<std::shared_ptr<Value>(std::vector<std::shared_ptr<Value>>)> __embeddedFunction;

        std::vector<std::shared_ptr<Node>> __children;

    public:
        inline virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) override final {
            auto functionArguments = std::vector<std::shared_ptr<Value>>();
            for(auto child : __children) {
                functionArguments.push_back(child->evaluate(_arguments));
            }
            return __embeddedFunction(functionArguments);
        }
    };

    class UserDefinedFunctionNode : Node {
    private:
        std::shared_ptr<SyntaxTree> __userDefinedFunction;

        std::vector<std::shared_ptr<Node>> __children;

    public:
        inline virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) override final {
            auto functionArguments = std::vector<std::shared_ptr<Value>>();
            for(auto child : __children) {
                functionArguments.push_back(child->evaluate(_arguments));
            }
            return __userDefinedFunction->evaluate(functionArguments);
        }
    };

    class IfNode : Node {
    private:
        std::shared_ptr<Node> __condition;
        std::shared_ptr<Node> __trueBranch, falseBranch;

    public:
        virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) override final;
    };

    class WhileNode : Node {
    private:
        std::shared_ptr<Node> __condition;
        std::shared_ptr<Node> __trueBranch, falseBranch;

    public:
        virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) override final;
    };

    class ForNode : Node {
    private:
        std::shared_ptr<Node> __initialization, __condition, __increment;
        std::shared_ptr<Node> __trueBranch, falseBranch;

    public:
        virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) override final;
    };

    class ReturnNode : Node {
    private:
        std::shared_ptr<Node> __returnValue;

    public:
        virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) override final;

        inline virtual bool is_return() override final { return true; }
    };

    class SemicolonNode : Node {
    private:
        std::shared_ptr<Node> __Expression, __nextColon;

    public:
        inline virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) override final {
            if(__Expression->is_return()) return __Expression->evaluate(_arguments);
            else return __nextColon->evaluate(_arguments);
        }
    };

private:
    std::shared_ptr<Node> __root;

    std::vector<std::shared_ptr<Value>> __localVariables;

public:
    inline virtual std::shared_ptr<Value> evaluate(const std::vector<std::shared_ptr<Value>> & _arguments) { return __root->evaluate(_arguments); }
};

} // namespace CINT

#endif