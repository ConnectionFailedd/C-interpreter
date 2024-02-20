#include "function.hpp"
#include "virtual_machine.hpp"
#include "instruction.hpp"
#include "instruction_flow.hpp"
#include "syntax_tree.hpp"
#include "value.hpp"
#include <cstddef>
#include <memory>
#include <vector>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::Backend {

/* ---------------------------------------------------------------------------------------------------- */

std::shared_ptr<IR::Value> VirtualMachine::execute(const IR::InstructionFlow & _flow) {
    auto pc = size_t(0);
    while(pc < _flow.__flow.size()) {
        const auto & instruction = _flow.__flow[pc];
        if(instruction->is_basic()) {
            execute(instruction);
            pc++;
            continue;
        }
        if(instruction->is_jump()) {
            pc += std::dynamic_pointer_cast<IR::JumpInstruction>(instruction)->__target;
            continue;
        }
        if(instruction->is_branch()) {
            auto condition = execute(std::dynamic_pointer_cast<IR::BranchInstruction>(instruction)->__condition)->as<bool>();
            if(condition) {
                pc += std::dynamic_pointer_cast<IR::BranchInstruction>(instruction)->__target;
            }
            else {
                pc++;
            }
            continue;
        }
        if(instruction->is_return()) {
            return execute(std::dynamic_pointer_cast<IR::ReturnInstruction>(instruction)->__expression);
        }
    }
#warning "Uncaught exception: No return instruction found"
    throw std::runtime_error("No return instruction found");
}

std::shared_ptr<IR::Value> VirtualMachine::execute(const std::shared_ptr<IR::Instruction> & _instruction) {
    if(_instruction->is_basic()) {
        return execute(std::dynamic_pointer_cast<IR::BasicInstruction>(_instruction)->__expression);
    }
    if(_instruction->is_branch()) {
        return execute(std::dynamic_pointer_cast<IR::BranchInstruction>(_instruction)->__condition);
    }
    if(_instruction->is_return()) {
        return execute(std::dynamic_pointer_cast<IR::ReturnInstruction>(_instruction)->__expression);
    }
#warning "Uncaught exception: Unrecognized instruction"
    throw std::runtime_error("Unrecognized instruction");
}

std::shared_ptr<IR::Value> VirtualMachine::execute(const std::shared_ptr<IR::SyntaxTreeNode> & _expression) {
    if(_expression->is_value_node()) {
        return std::dynamic_pointer_cast<IR::ValueNode>(_expression)->__value;
    }
    if(_expression->is_stack_reference_node()) {
        return __stack.top()[std::dynamic_pointer_cast<IR::StackReferenceNode>(_expression)->__index];
    }
    if(_expression->is_function_node()) {
        auto functionCall = std::dynamic_pointer_cast<IR::FunctionNode>(_expression);
        auto frame = Frame();
        for(const auto & argument : functionCall->__arguments) {
            frame.push_back(execute(argument));
        }
        if(functionCall->__function->is_native_function()) {
            return std::dynamic_pointer_cast<IR::NativeFunction>(functionCall->__function)->__function(frame);
        }
        if(functionCall->__function->is_user_defined_function()) {
            auto function = std::dynamic_pointer_cast<IR::UserDefinedFunction>(functionCall->__function);
            for(const auto & type : function->__localVariableTypes) {
                frame.push_back(std::make_shared<IR::Value>(IR::Value(type)));
            }
            __stack.push(frame);
            auto result = execute(function->__functionBody);
            __stack.pop();
            return result;
        }
    }
    if(_expression->is_sub_value_node()) {
        auto value = execute(std::dynamic_pointer_cast<IR::SubValueNode>(_expression)->__valueNode);
        auto memberName = std::dynamic_pointer_cast<IR::SubValueNode>(_expression)->__memberName;
        return std::make_shared<IR::SubValue>(IR::SubValue(value, memberName));
    }
#warning "Uncaught exception: Unrecognized expression"
    throw std::runtime_error("Unrecognized expression");

}

VirtualMachine virtualMachine = VirtualMachine();

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::Backend

/* ---------------------------------------------------------------------------------------------------- */
