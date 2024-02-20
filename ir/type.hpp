#ifndef __TYPE_HPP__
#define __TYPE_HPP__

/* ---------------------------------------------------------------------------------------------------- */

#include <cstddef>
#include <memory>
#include <new>
#include <string>
#include <vector>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::IR {

/* ---------------------------------------------------------------------------------------------------- */

// class Type
// Abstract class for all types
class Type {
public:
    virtual const std::string & type_name() const = 0;
    virtual size_t size() const = 0;
    virtual std::align_val_t alignment() const = 0;

    inline virtual bool is_type_key() const { return false; }
    inline virtual bool is_built_in_type() const { return false; }
    inline virtual bool is_aggregated_type() const { return false; }

    inline virtual bool operator<(const Type & _rhs) const noexcept { return this->type_name() < _rhs.type_name(); }
};

/* ---------------------------------------------------------------------------------------------------- */

// class TypeKey
// Used as key of type set
class TypeKey final : public Type {
private:
    std::string __typeName;

public:
    inline TypeKey(const std::string & _typeName) : __typeName(_typeName) {}
    inline TypeKey(const TypeKey & _other) : __typeName(_other.__typeName) {}

    inline virtual const std::string & type_name() const override final { return __typeName; }
    inline virtual size_t size() const override final { return 0; }
    inline virtual std::align_val_t alignment() const override final { return std::align_val_t(0); }

    inline virtual bool is_type_key() const override final { return true; }

    using Type::operator<;
};

// class NativeType
// Built-in types
class NativeType final : public Type {
private:
    std::string __typeName;
    size_t __size;
    std::align_val_t __alignment;

public:
    inline NativeType(const std::string & _typeName, size_t _size, std::align_val_t _alignment) : __typeName(_typeName), __size(_size), __alignment(_alignment) {}
    inline NativeType(const NativeType & _other) : __typeName(_other.__typeName), __size(_other.__size), __alignment(_other.__alignment) {}

    inline virtual const std::string & type_name() const override final { return __typeName; }
    inline virtual size_t size() const override final { return __size; }
    inline virtual std::align_val_t alignment() const override final { return __alignment; }

    inline virtual bool is_built_in_type() const override final { return true; }

    using Type::operator<;
};

// class AggregatedType
// Aggregated types
class AggregatedType final : public Type {
public:
    class Member {
    private:
        std::shared_ptr<Type> __type;
        std::string __name;
        size_t __offset;

    public:
        inline Member(const std::shared_ptr<Type> & _type, const std::string & _name, size_t _offset = 0) : __type(_type), __name(_name), __offset(_offset) {}

        inline const std::shared_ptr<Type> & type() const { return __type; }
        inline const std::string & name() const { return __name; }
        inline size_t offset() const { return __offset; }
    };

private:
    std::string __typeName;
    size_t __size;
    std::align_val_t __alignment;

    std::vector<Member> __members;

public:
    AggregatedType(const std::string & _typeName, const std::vector<Member> & _members);
    inline AggregatedType(const AggregatedType & _other) : __typeName(_other.__typeName), __size(_other.__size), __alignment(_other.__alignment), __members(_other.__members) {}

    inline virtual const std::string & type_name() const override final { return __typeName; }
    inline virtual size_t size() const override final { return __size; }
    inline virtual std::align_val_t alignment() const override final { return __alignment; }

    inline virtual bool is_aggregated_type() const override final { return true; }

    using Type::operator<;

    size_t get_member_offset(const std::string & _name) const;
    const std::shared_ptr<Type> & get_member_type(const std::string & _name) const;
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::IR

/* ---------------------------------------------------------------------------------------------------- */

#endif // __TYPE_HPP__