#include "type.hpp"
#include <cstddef>
#include <new>

namespace CINT::IR {

/* ---------------------------------------------------------------------------------------------------- */

AggregatedType::AggregatedType(const std::string & _typeName, const std::vector<Member> & _members) : __typeName(_typeName), __size(0), __alignment(std::align_val_t(0)), __members() {
    for(auto & member : _members) {
        __size = (__size + size_t(member.type()->alignment()) - 1) & ~(size_t(member.type()->alignment()) - 1);
        __members.push_back(Member(member.type(), member.name(), __size));
        __size += member.type()->size();
        if(member.type()->alignment() > __alignment) {
            __alignment = member.type()->alignment();
        }
    }
    __size = (__size + size_t(__alignment) - 1) & ~(size_t(__alignment) - 1);
}

size_t AggregatedType::get_member_offset(const std::string & _name) const {
    for(auto & member : __members) {
        if(member.name() == _name) {
            return member.offset();
        }
    }
#warning "Uncaught exception: Member not found"
    throw std::runtime_error("Member not found");
}

const std::shared_ptr<Type> & AggregatedType::get_member_type(const std::string & _name) const {
    for(auto & member : __members) {
        if(member.name() == _name) {
            return member.type();
        }
    }
#warning "Uncaught exception: Member not found"
    throw std::runtime_error("Member not found");
}

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::IR