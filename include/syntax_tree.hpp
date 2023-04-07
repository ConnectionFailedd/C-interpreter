#ifndef __SYNTAX_TREE_HPP__
#define __SYNTAX_TREE_HPP__

#include <memory>
#include <vector>

#ifndef __VALUE_HPP__
#define VALUE_TYPE int
#endif

namespace CINT {

class SyntaxTree {
protected:
    class Node {
    private:
        VALUE_TYPE __value;

    public:
        virtual VALUE_TYPE evaluate() = 0;
    };

private:
    std::shared_ptr<Node> root;

public:
    inline virtual VALUE_TYPE evaluate() { return root->evaluate(); }
};

} // namespace CINT

#endif