#ifndef __TYPE_HPP__
#define __TYPE_HPP__

#include <cstddef>
#include <memory>
#include <new>
#include <vector>

#include "name.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {
namespace Type {

/* ---------------------------------------------------------------------------------------------------- */

class Type {
public:
    using TypeName = Name;

public:
    virtual std::size_t size() const noexcept = 0;
    virtual std::align_val_t align() const noexcept = 0;
    virtual const TypeName & name() const noexcept = 0;

    inline virtual bool is_builtin_type() const noexcept { return false; }
    inline virtual bool is_aggregated_type() const noexcept { return false; }
    inline virtual bool is_pointer_type() const noexcept { return false; }
    inline virtual bool is_left_reference_type() const noexcept { return false; }
    inline virtual bool is_right_reference_type() const noexcept { return false; }
    inline virtual bool is_array_type() const noexcept { return false; }
    inline virtual bool is_type_alias() const noexcept { return false; }
};

/* ---------------------------------------------------------------------------------------------------- */

class BuiltInType : public Type {
private:
    const std::size_t __size;
    const std::align_val_t __align;

    const TypeName __name;

public:
    inline BuiltInType(std::size_t _size, std::align_val_t _align, TypeName && _name) noexcept : __size(_size), __align(_align), __name(std::move(_name)) {}

    inline virtual std::size_t size() const noexcept override final { return __size; }
    inline virtual std::align_val_t align() const noexcept override final { return __align; }
    inline virtual const TypeName & name() const noexcept override final { return __name; }

    inline virtual bool is_builtin_type() const noexcept override final { return true; }
};

class AggregatedType : public Type {
private:
    const std::size_t __size;
    const std::align_val_t __align;

    const std::vector<std::shared_ptr<Type>> __subTypes;
    const std::shared_ptr<Type> __fatherType;

    const TypeName __name;

public:
    AggregatedType(std::vector<std::shared_ptr<Type>> && _subTypes, TypeName && _name, const std::shared_ptr<Type> & __fatherType = nullptr) noexcept;

    inline virtual std::size_t size() const noexcept override final { return __size; }
    inline virtual std::align_val_t align() const noexcept override final { return __align; }
    inline virtual const TypeName & name() const noexcept override final { return __name; }

    inline virtual bool is_aggregated_type() const noexcept override final { return true; }
};

class PointerType : public Type {
private:
    const std::shared_ptr<Type> __baseType;
    const std::shared_ptr<Type> __trueType;

public:
    inline PointerType(const std::shared_ptr<Type> & _baseType, const std::shared_ptr<Type> _trueType) noexcept : __baseType(_baseType), __trueType(_trueType) {}

    inline virtual std::size_t size() const noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const noexcept override final { return std::align_val_t(alignof(void *)); }
    inline virtual const TypeName & name() const noexcept override final { return Name::noName; }

    inline virtual bool is_pointer_type() const noexcept override final { return true; }
};

class LeftReferenceType : public Type {
private:
    const std::shared_ptr<Type> __baseType;
    const std::shared_ptr<Type> __trueType;

public:
    inline LeftReferenceType(const std::shared_ptr<Type> & _baseType, const std::shared_ptr<Type> _trueType) noexcept : __baseType(_baseType), __trueType(_trueType) {}

    inline virtual std::size_t size() const noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const noexcept override final { return std::align_val_t(alignof(void *)); }
    inline virtual const TypeName & name() const noexcept override final { return Name::noName; }

    inline virtual bool is_left_reference_type() const noexcept override final { return true; }
};

class RightReferenceType : public Type {
private:
    const std::shared_ptr<Type> __baseType;
    const std::shared_ptr<Type> __trueType;

public:
    inline RightReferenceType(const std::shared_ptr<Type> & _baseType, const std::shared_ptr<Type> _trueType) noexcept : __baseType(_baseType), __trueType(_trueType) {}

    inline virtual std::size_t size() const noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const noexcept override final { return std::align_val_t(alignof(void *)); }
    inline virtual const TypeName & name() const noexcept override final { return Name::noName; }

    inline virtual bool is_right_reference_type() const noexcept override final { return true; }
};

class ArrayType : public Type {
private:
    const std::shared_ptr<Type> __baseType;
    const std::size_t __arraySize;

public:
    inline ArrayType(const std::shared_ptr<Type> & _baseType, std::size_t _arraySize) noexcept : __baseType(_baseType), __arraySize(_arraySize) {}

    inline virtual std::size_t size() const noexcept override final { return __baseType->size() * __arraySize; }
    inline virtual std::align_val_t align() const noexcept override final { return __baseType->align(); }
    inline virtual const TypeName & name() const noexcept override final { return Name::noName; }

    inline virtual bool is_array_type() const noexcept override final { return true; }
};

class TypeAlias : public Type {
private:
    const std::shared_ptr<Type> __originalType;

    const TypeName __name;

public:
    inline TypeAlias(const std::shared_ptr<Type> _originalType, TypeName && _name) noexcept : __originalType(_originalType), __name(std::move(_name)) {}

    inline virtual std::size_t size() const noexcept override final { return __originalType->size(); }
    inline virtual std::align_val_t align() const noexcept override final { return __originalType->align(); }
    inline virtual const TypeName & name() const noexcept override final { return __name; }

    inline virtual bool is_type_alias() const noexcept override final { return true; }
};

/* ---------------------------------------------------------------------------------------------------- */

}
} // namespace CINT::Type

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __TYPE_HPP__ */