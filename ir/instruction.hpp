#ifndef __INSTRUCTION_HPP__
#define __INSTRUCTION_HPP__

/* ---------------------------------------------------------------------------------------------------- */

#include "syntax_tree.hpp"
#include <cstddef>
#include <memory>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::Backend {
class VirtualMachine;
}

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::IR {

/* ---------------------------------------------------------------------------------------------------- */

class Instruction {
    friend Backend::VirtualMachine;

public:
    virtual size_t size() const noexcept = 0;

    inline virtual bool is_basic() const noexcept { return false; }
    inline virtual bool is_jump() const noexcept { return false; }
    inline virtual bool is_branch() const noexcept { return false; }
    inline virtual bool is_return() const noexcept { return false; }
    inline virtual bool is_aggregated() const noexcept { return false; }
};

class BasicInstruction final : public Instruction {
    friend Backend::VirtualMachine;

private:
    std::shared_ptr<SyntaxTreeNode> __expression;

public:
    virtual size_t size() const noexcept override final { return 1; }

    inline BasicInstruction(const std::shared_ptr<SyntaxTreeNode> & _expression) : __expression(_expression) {}
    inline BasicInstruction(const BasicInstruction & _other) : __expression(_other.__expression) {}

    inline virtual bool is_basic() const noexcept override final { return true; }
};

class JumpInstruction final : public Instruction {
    friend Backend::VirtualMachine;

private:
    size_t __target;

public:
    inline virtual size_t size() const noexcept override final { return 1; }

    inline JumpInstruction(size_t _target) : __target(_target) {}
    inline JumpInstruction(const JumpInstruction & _other) : __target(_other.__target) {}

    inline virtual bool is_jump() const noexcept override final { return true; }
};

class BranchInstruction final : public Instruction {
    friend Backend::VirtualMachine;

private:
    std::shared_ptr<SyntaxTreeNode> __condition;
    size_t __target;

public:
    inline virtual size_t size() const noexcept override final { return 1; }

    inline BranchInstruction(const std::shared_ptr<SyntaxTreeNode> & _condition, size_t _target) : __condition(_condition), __target(_target) {}
    inline BranchInstruction(const BranchInstruction & _other) : __condition(_other.__condition), __target(_other.__target) {}

    inline virtual bool is_branch() const noexcept override final { return true; }
};

class ReturnInstruction final : public Instruction {
    friend Backend::VirtualMachine;

private:
    std::shared_ptr<SyntaxTreeNode> __expression;

public:
    inline virtual size_t size() const noexcept override final { return 1; }

    inline ReturnInstruction(const std::shared_ptr<SyntaxTreeNode> & _expression) : __expression(_expression) {}
    inline ReturnInstruction(const ReturnInstruction & _other) : __expression(_other.__expression) {}

    inline virtual bool is_return() const noexcept override final { return true; }
};

class AggregatedInstruction final : public Instruction {
    friend Backend::VirtualMachine;

private:
    std::vector<std::shared_ptr<Instruction>> __instructions;

public:
    inline virtual size_t size() const noexcept override final {
        size_t _size = 0;
        for (const auto & _instruction : __instructions) {
            _size += _instruction->size();
        }
        return _size;
    }

    inline AggregatedInstruction(const std::vector<std::shared_ptr<Instruction>> & _instructions) : __instructions(_instructions) {}
    inline AggregatedInstruction(const AggregatedInstruction & _other) : __instructions(_other.__instructions) {}

    inline virtual bool is_aggregated() const noexcept override final { return true; }

    inline const std::vector<std::shared_ptr<Instruction>> & instructions() const noexcept { return __instructions; }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::IR

/* ---------------------------------------------------------------------------------------------------- */

#endif // __INSTRUCTION_HPP__