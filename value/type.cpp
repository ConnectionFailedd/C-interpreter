#include <memory>
#include <new>

#include "type.hpp"
#include "scope.hpp"

namespace CINT {

std::multiset<std::shared_ptr<Type::Type>, Type::Type::SharedPtrTypeCmp> Type::Type::type_multi_set_init() {
    auto res = std::multiset<std::shared_ptr<Type>, Type::Type::SharedPtrTypeCmp>();

    res.insert(std::make_shared<BuiltInType>(1, std::align_val_t(1), Name(Scope(), "char")));
    res.insert(std::make_shared<BuiltInType>(4, std::align_val_t(4), Name(Scope(), "int")));

    return res;
}

}