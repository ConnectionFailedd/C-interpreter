#ifndef __TYPE_HPP__
#define __TYPE_HPP__

#include <memory>
#include <new>
#include <set>
#include <utility>
#include <vector>

#include "name.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class Type {
public:
    using TypeName = Name;

private:
    TypeName __typeName;

public:
    inline Type(const TypeName & _typeName) : __typeName(_typeName) {}
    inline Type(TypeName && _typeName) : __typeName(std::move(_typeName)) {}

    inline virtual std::size_t size() const noexcept { return 0; }
    inline virtual std::align_val_t align() const noexcept { return std::align_val_t(1); }
    inline virtual const TypeName & type_name() const noexcept { return __typeName; }

    inline virtual bool is_builtin_type() const noexcept { return false; }
    inline virtual bool is_aggregated_type() const noexcept { return false; }
    inline virtual bool is_pointer_type() const noexcept { return false; }
    inline virtual bool is_left_reference_type() const noexcept { return false; }
    inline virtual bool is_right_reference_type() const noexcept { return false; }
    inline virtual bool is_array_type() const noexcept { return false; }
    inline virtual bool is_type_alias() const noexcept { return false; }

    inline virtual bool operator<(const Type & _rhs) const noexcept { return type_name() < _rhs.type_name(); }

public:

    class TypeSet {
    public:
        class SharedPtrTypeCmp {
        public:
            bool operator()(const std::shared_ptr<const Type> & _lhs, const std::shared_ptr<const Type> & _rhs) const noexcept {
                return *_lhs < *_rhs;
            }
        };

    private:
        // types should never be modified once they are created
        std::set<std::shared_ptr<const Type>, SharedPtrTypeCmp> __typeSet;

    public:
        inline TypeSet() : __typeSet() {}
        inline TypeSet(std::set<std::shared_ptr<const Type>, SharedPtrTypeCmp> && _typeSet) : __typeSet(std::move(_typeSet)) {}

        inline bool insert(const std::shared_ptr<const Type> & _src) { return __typeSet.insert(_src).second; }
        inline bool insert(std::shared_ptr<const Type> && _src) { return __typeSet.insert(std::move(_src)).second; }
        inline const std::shared_ptr<const Type> & find(const TypeName & _key) const {
            auto iter = __typeSet.find(std::make_shared<const Type>(_key));
            if(iter != __typeSet.end()) return *iter;
            return NOTYPE;
        }
    };

public:
    const static std::shared_ptr<const Type> NOTYPE;
};

/* ---------------------------------------------------------------------------------------------------- */

class BuiltInType : public Type {
private:
    std::size_t __size;
    std::align_val_t __align;

public:
    inline BuiltInType(const TypeName & _typeName, std::size_t _size, std::align_val_t _align) : Type(_typeName), __size(_size), __align(_align) {}
    inline BuiltInType(TypeName && _typeName, std::size_t _size, std::align_val_t _align) noexcept : Type(std::move(_typeName)), __size(_size), __align(_align) {}

    inline virtual std::size_t size() const noexcept override final { return __size; }
    inline virtual std::align_val_t align() const noexcept override final { return __align; }

    inline virtual bool is_builtin_type() const noexcept override final { return true; }
};

class AggregatedType : public Type {
public:
    class Domain {
    private:
        std::shared_ptr<const Type> __type;
        Name __name;
        std::size_t __offset;
    public:
        inline Domain(const std::shared_ptr<const Type> & _type, const Name & _name) : __type(_type), __name(_name), __offset(0) {}
        inline Domain(const std::shared_ptr<const Type> & _type, Name && _name) : __type(_type), __name(std::move(_name)), __offset(0) {}
        inline Domain(const std::shared_ptr<const Type> & _type, const Name & _name, std::size_t _offset) : __type(_type), __name(_name), __offset(_offset) {}
        inline Domain(const std::shared_ptr<const Type> & _type, Name && _name, std::size_t _offset) noexcept : __type(_type), __name(std::move(_name)), __offset(_offset) {}

        inline const std::shared_ptr<const Type> & type() const noexcept { return __type; }
        inline const Name & name() const noexcept { return __name; }
        inline std::size_t offset() const noexcept { return __offset; }
    };

private:
    std::size_t __size;
    std::align_val_t __align;

    std::vector<Domain> __domains;

    // std::shared_ptr<const Type> __fatherType;

public:
    AggregatedType(const TypeName & _typeName, const std::vector<Domain> & _domains);

    inline virtual std::size_t size() const noexcept override final { return __size; }
    inline virtual std::align_val_t align() const noexcept override final { return __align; }

    std::size_t get_offset_of(const Name &) const;

    inline virtual bool is_aggregated_type() const noexcept override final { return true; }
};

class PointerType : public Type {
private:
    std::shared_ptr<const Type> __baseType;

public:
    inline PointerType(const std::shared_ptr<const Type> & _baseType) noexcept : Type(""), __baseType(_baseType) {}

    inline virtual std::size_t size() const noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const noexcept override final { return std::align_val_t(alignof(void *)); }

    inline virtual bool is_pointer_type() const noexcept override final { return true; }
};

class LeftReferenceType : public Type {
private:
    std::shared_ptr<const Type> __baseType;

public:
    inline LeftReferenceType(const std::shared_ptr<const Type> & _baseType) noexcept : Type(""), __baseType(_baseType) {}

    inline virtual std::size_t size() const noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const noexcept override final { return std::align_val_t(alignof(void *)); }

    inline virtual bool is_left_reference_type() const noexcept override final { return true; }
};

class RightReferenceType : public Type {
private:
    std::shared_ptr<const Type> __baseType;

public:
    inline RightReferenceType(const std::shared_ptr<const Type> & _baseType) noexcept : Type(""), __baseType(_baseType) {}

    inline virtual std::size_t size() const noexcept override final { return sizeof(void *); }
    inline virtual std::align_val_t align() const noexcept override final { return std::align_val_t(alignof(void *)); }

    inline virtual bool is_right_reference_type() const noexcept override final { return true; }
};

class ArrayType : public Type {
private:
    std::shared_ptr<const Type> __baseType;
    std::size_t __arraySize;

public:
    inline ArrayType(const std::shared_ptr<const Type> & _baseType, std::size_t _arraySize) noexcept : Type(""), __baseType(_baseType), __arraySize(_arraySize) {}

    inline virtual std::size_t size() const noexcept override final { return __baseType->size() * __arraySize; }
    inline virtual std::align_val_t align() const noexcept override final { return __baseType->align(); }

    inline virtual bool is_array_type() const noexcept override final { return true; }
};

class TypeAlias : public Type {
private:
    std::shared_ptr<const Type> __originalType;

public:
    inline TypeAlias(TypeName && _typeName, const std::shared_ptr<const Type> _originalType) noexcept : Type(std::move(_typeName)), __originalType(_originalType) {}

    inline virtual std::size_t size() const noexcept override final { return __originalType->size(); }
    inline virtual std::align_val_t align() const noexcept override final { return __originalType->align(); }

    inline virtual bool is_type_alias() const noexcept override final { return true; }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::Type

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __TYPE_HPP__ */