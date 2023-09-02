#ifndef __TYPE_HPP__
#define __TYPE_HPP__

#include <cstddef>
#include <memory>
#include <new>
#include <set>
#include <vector>

#include "name.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {
namespace Types {

/* ---------------------------------------------------------------------------------------------------- */

class Type {
public:
    using TypeName = Name;

private:
    TypeName __typeName;

public:
    inline Type() : __typeName(Name::NONAME) {}
    inline Type(const TypeName & _typeName) : __typeName(_typeName) {}
    inline Type(TypeName && _typeName) : __typeName(std::move(_typeName)) {}

    inline virtual std::size_t size() const & noexcept { return 0; }
    inline virtual std::align_val_t align() const & noexcept { return std::align_val_t(1); }
    inline virtual const TypeName & type_name() const & noexcept { return __typeName; }

    inline virtual bool is_builtin_type() const noexcept { return false; }
    inline virtual bool is_aggregated_type() const noexcept { return false; }
    inline virtual bool is_pointer_type() const noexcept { return false; }
    inline virtual bool is_left_reference_type() const noexcept { return false; }
    inline virtual bool is_right_reference_type() const noexcept { return false; }
    inline virtual bool is_array_type() const noexcept { return false; }
    inline virtual bool is_type_alias() const noexcept { return false; }

    inline virtual bool operator<(const Type & _rhs) const & noexcept { return type_name() < _rhs.type_name(); }

public:
    class TypeMultiSet {
    public:
        class SharedPtrTypeCmp {
        public:
            bool operator()(const std::shared_ptr<Type> & _lhs, const std::shared_ptr<Type> & _rhs) const & noexcept {
                return *_lhs < *_rhs;
            }
        };

    private:
        std::multiset<std::shared_ptr<Type>, SharedPtrTypeCmp> __typeMultiSet;

    public:
        inline TypeMultiSet(std::multiset<std::shared_ptr<Type>, SharedPtrTypeCmp> && _typeMultiSet) : __typeMultiSet(std::move(_typeMultiSet)) {}

        static TypeMultiSet init();

        inline void insert(const std::shared_ptr<Type> & _src) { __typeMultiSet.insert(_src); }
        inline void insert(std::shared_ptr<Type> && _src) { __typeMultiSet.insert(std::move(_src)); }
        std::shared_ptr<Type> find(const UnconfirmedName &);
    };

public:
    static TypeMultiSet typeMultiSet;

    const static std::shared_ptr<Type> NOTYPE;
};

/* ---------------------------------------------------------------------------------------------------- */

class BuiltInType : public Type {
private:
    const std::size_t __size;
    const std::align_val_t __align;

public:
    inline BuiltInType(const TypeName & _typeName, std::size_t _size, std::align_val_t _align) : Type(_typeName), __size(_size), __align(_align) {}
    inline BuiltInType(TypeName && _typeName, std::size_t _size, std::align_val_t _align) noexcept : Type(std::move(_typeName)), __size(_size), __align(_align) {}

    inline virtual std::size_t size() const & noexcept override final { return __size; }
    inline virtual std::align_val_t align() const & noexcept override final { return __align; }
    using Type::type_name;

    inline virtual bool is_builtin_type() const noexcept override final { return true; }

    using Type::operator<;
};

class AggregatedType : public Type {
private:
    std::size_t __size;
    std::align_val_t __align;

    std::vector<std::shared_ptr<Type>> __subTypes;
    std::shared_ptr<Type> __fatherType;

public:
    AggregatedType(TypeName && _typeName, std::vector<std::shared_ptr<Type>> && _subTypes, const std::shared_ptr<Type> & __fatherType = nullptr) noexcept;

    inline virtual std::size_t size() const & noexcept override final { return __size; }
    inline virtual std::align_val_t align() const & noexcept override final { return __align; }
    using Type::type_name;

    inline virtual bool is_aggregated_type() const noexcept override final { return true; }

    using Type::operator<;
};

class PointerType : public Type {
private:
    std::shared_ptr<Type> __baseType;
    std::shared_ptr<Type> __trueType;

public:
    inline PointerType(const std::shared_ptr<Type> & _baseType, const std::shared_ptr<Type> _trueType) noexcept : Type(), __baseType(_baseType), __trueType(_trueType) {}

    inline virtual std::size_t size() const & noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const & noexcept override final { return std::align_val_t(alignof(void *)); }
    using Type::type_name;

    inline virtual bool is_pointer_type() const noexcept override final { return true; }

    using Type::operator<;
};

class LeftReferenceType : public Type {
private:
    std::shared_ptr<Type> __baseType;
    std::shared_ptr<Type> __trueType;

public:
    inline LeftReferenceType(const std::shared_ptr<Type> & _baseType, const std::shared_ptr<Type> _trueType) noexcept : Type(), __baseType(_baseType), __trueType(_trueType) {}

    inline virtual std::size_t size() const & noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const & noexcept override final { return std::align_val_t(alignof(void *)); }
    using Type::type_name;

    inline virtual bool is_left_reference_type() const noexcept override final { return true; }

    using Type::operator<;
};

class RightReferenceType : public Type {
private:
    std::shared_ptr<Type> __baseType;
    std::shared_ptr<Type> __trueType;

public:
    inline RightReferenceType(const std::shared_ptr<Type> & _baseType, const std::shared_ptr<Type> _trueType) noexcept : Type(), __baseType(_baseType), __trueType(_trueType) {}

    inline virtual std::size_t size() const & noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const & noexcept override final { return std::align_val_t(alignof(void *)); }
    using Type::type_name;

    inline virtual bool is_right_reference_type() const noexcept override final { return true; }

    using Type::operator<;
};

class ArrayType : public Type {
private:
    std::shared_ptr<Type> __baseType;
    std::size_t __arraySize;

public:
    inline ArrayType(const std::shared_ptr<Type> & _baseType, std::size_t _arraySize) noexcept : Type(), __baseType(_baseType), __arraySize(_arraySize) {}

    inline virtual std::size_t size() const & noexcept override final { return __baseType->size() * __arraySize; }
    inline virtual std::align_val_t align() const & noexcept override final { return __baseType->align(); }
    using Type::type_name;

    inline virtual bool is_array_type() const noexcept override final { return true; }

    using Type::operator<;
};

class TypeAlias : public Type {
private:
    std::shared_ptr<Type> __originalType;

public:
    inline TypeAlias(TypeName && _typeName, const std::shared_ptr<Type> _originalType) noexcept : Type(std::move(_typeName)), __originalType(_originalType) {}

    inline virtual std::size_t size() const & noexcept override final { return __originalType->size(); }
    inline virtual std::align_val_t align() const & noexcept override final { return __originalType->align(); }
    using Type::type_name;

    inline virtual bool is_type_alias() const noexcept override final { return true; }

    using Type::operator<;
};

/* ---------------------------------------------------------------------------------------------------- */

}
} // namespace CINT::Type

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __TYPE_HPP__ */