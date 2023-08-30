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
    const TypeName __name;

public:
    inline Type() : __name(Name::NONAME) {}
    inline Type(const TypeName & _name) : __name(_name) {}
    inline Type(TypeName && _name) : __name(std::move(_name)) {}

    inline virtual std::size_t size() const noexcept { return 0; }
    inline virtual std::align_val_t align() const noexcept { return std::align_val_t(1); }
    inline virtual const TypeName & name() const noexcept { return __name; }

    inline virtual bool is_builtin_type() const noexcept { return false; }
    inline virtual bool is_aggregated_type() const noexcept { return false; }
    inline virtual bool is_pointer_type() const noexcept { return false; }
    inline virtual bool is_left_reference_type() const noexcept { return false; }
    inline virtual bool is_right_reference_type() const noexcept { return false; }
    inline virtual bool is_array_type() const noexcept { return false; }
    inline virtual bool is_type_alias() const noexcept { return false; }

    inline virtual bool operator<(const Type & _rhs) const & noexcept { return name() < _rhs.name(); }

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
        inline void insert(std::shared_ptr<Type> && _src) { __typeMultiSet.insert(_src); }
        inline std::shared_ptr<Type> find(std::string && _typeName) {
            // should be rewritten
            auto iter = __typeMultiSet.find(std::make_shared<Type>(TypeName(std::move(_typeName))));
            if(iter != __typeMultiSet.end()) {
                return * iter;
            }
            return Type::NOTYPE;
        }
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
    inline BuiltInType(const TypeName & _name, std::size_t _size, std::align_val_t _align) : Type(_name), __size(_size), __align(_align) {}
    inline BuiltInType(TypeName && _name, std::size_t _size, std::align_val_t _align) noexcept : Type(std::move(_name)), __size(_size), __align(_align) {}

    inline virtual std::size_t size() const noexcept override final { return __size; }
    inline virtual std::align_val_t align() const noexcept override final { return __align; }
    using Type::name;

    inline virtual bool is_builtin_type() const noexcept override final { return true; }

    using Type::operator<;
};

class AggregatedType : public Type {
private:
    const std::size_t __size;
    const std::align_val_t __align;

    const std::vector<std::shared_ptr<Type>> __subTypes;
    const std::shared_ptr<Type> __fatherType;

public:
    AggregatedType(TypeName && _name, std::vector<std::shared_ptr<Type>> && _subTypes, const std::shared_ptr<Type> & __fatherType = nullptr) noexcept;

    inline virtual std::size_t size() const noexcept override final { return __size; }
    inline virtual std::align_val_t align() const noexcept override final { return __align; }
    using Type::name;

    inline virtual bool is_aggregated_type() const noexcept override final { return true; }

    using Type::operator<;
};

class PointerType : public Type {
private:
    const std::shared_ptr<Type> __baseType;
    const std::shared_ptr<Type> __trueType;

public:
    inline PointerType(const std::shared_ptr<Type> & _baseType, const std::shared_ptr<Type> _trueType) noexcept : Type(), __baseType(_baseType), __trueType(_trueType) {}

    inline virtual std::size_t size() const noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const noexcept override final { return std::align_val_t(alignof(void *)); }
    using Type::name;

    inline virtual bool is_pointer_type() const noexcept override final { return true; }

    using Type::operator<;
};

class LeftReferenceType : public Type {
private:
    const std::shared_ptr<Type> __baseType;
    const std::shared_ptr<Type> __trueType;

public:
    inline LeftReferenceType(const std::shared_ptr<Type> & _baseType, const std::shared_ptr<Type> _trueType) noexcept : Type(), __baseType(_baseType), __trueType(_trueType) {}

    inline virtual std::size_t size() const noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const noexcept override final { return std::align_val_t(alignof(void *)); }
    using Type::name;

    inline virtual bool is_left_reference_type() const noexcept override final { return true; }

    using Type::operator<;
};

class RightReferenceType : public Type {
private:
    const std::shared_ptr<Type> __baseType;
    const std::shared_ptr<Type> __trueType;

public:
    inline RightReferenceType(const std::shared_ptr<Type> & _baseType, const std::shared_ptr<Type> _trueType) noexcept : Type(), __baseType(_baseType), __trueType(_trueType) {}

    inline virtual std::size_t size() const noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const noexcept override final { return std::align_val_t(alignof(void *)); }
    using Type::name;

    inline virtual bool is_right_reference_type() const noexcept override final { return true; }

    using Type::operator<;
};

class ArrayType : public Type {
private:
    const std::shared_ptr<Type> __baseType;
    const std::size_t __arraySize;

public:
    inline ArrayType(const std::shared_ptr<Type> & _baseType, std::size_t _arraySize) noexcept : Type(), __baseType(_baseType), __arraySize(_arraySize) {}

    inline virtual std::size_t size() const noexcept override final { return __baseType->size() * __arraySize; }
    inline virtual std::align_val_t align() const noexcept override final { return __baseType->align(); }
    using Type::name;

    inline virtual bool is_array_type() const noexcept override final { return true; }

    using Type::operator<;
};

class TypeAlias : public Type {
private:
    const std::shared_ptr<Type> __originalType;

public:
    inline TypeAlias(TypeName && _name, const std::shared_ptr<Type> _originalType) noexcept : Type(std::move(_name)), __originalType(_originalType) {}

    inline virtual std::size_t size() const noexcept override final { return __originalType->size(); }
    inline virtual std::align_val_t align() const noexcept override final { return __originalType->align(); }
    using Type::name;

    inline virtual bool is_type_alias() const noexcept override final { return true; }

    using Type::operator<;
};

/* ---------------------------------------------------------------------------------------------------- */

}
} // namespace CINT::Type

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __TYPE_HPP__ */