#ifndef __VIRTUAL_MACHINE_HPP__
#define __VIRTUAL_MACHINE_HPP__

/* ---------------------------------------------------------------------------------------------------- */

#include "instruction_flow.hpp"
#include "syntax_tree.hpp"
#include "value.hpp"
#include <stack>
#include <vector>
#include <memory>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::Backend {

/* ---------------------------------------------------------------------------------------------------- */

class VirtualMachine {
private:
    using Frame = std::vector<std::shared_ptr<IR::Value>>;
    std::stack<Frame> __stack;

public:
    std::shared_ptr<IR::Value> execute(const IR::InstructionFlow & _flow);
    std::shared_ptr<IR::Value> execute(const std::shared_ptr<IR::Instruction> & _instruction);
    std::shared_ptr<IR::Value> execute(const std::shared_ptr<IR::SyntaxTreeNode> & _expression);
};

extern VirtualMachine virtualMachine;

/* ---------------------------------------------------------------------------------------------------- */

}

/* ---------------------------------------------------------------------------------------------------- */

#endif