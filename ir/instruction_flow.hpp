#ifndef __INSTRUCTION_FLOW_HPP__
#define __INSTRUCTION_FLOW_HPP__

/* ---------------------------------------------------------------------------------------------------- */

#include "instruction.hpp"
#include <memory>
#include <vector>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::Backend {
class VirtualMachine;
}

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::IR {

/* ---------------------------------------------------------------------------------------------------- */

class InstructionFlow {
    friend Backend::VirtualMachine;

private:
    std::vector<std::shared_ptr<Instruction>> __flow;

public:
    inline InstructionFlow() = default;
    inline InstructionFlow(const InstructionFlow & _other) : __flow(_other.__flow) {}

    inline void append(const std::shared_ptr<Instruction> & _instruction) {
        if(_instruction->is_aggregated()) {
            for(const auto & _sub_instruction : std::dynamic_pointer_cast<AggregatedInstruction>(_instruction)->instructions()) {
                append(_sub_instruction);
            }
        }
        else {
            __flow.push_back(_instruction);
        }
    }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::IR

/* ---------------------------------------------------------------------------------------------------- */

#endif