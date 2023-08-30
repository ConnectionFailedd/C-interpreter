#include <memory>
#include <new>

#include "type.hpp"
#include "scope.hpp"

namespace CINT {

Types::Type::TypeMultiSet Types::Type::TypeMultiSet::init() {
    auto res = std::multiset<std::shared_ptr<Type>, Types::Type::TypeMultiSet::SharedPtrTypeCmp>();

    res.insert(std::make_shared<BuiltInType>(Name(Scope(), "char"), 1, std::align_val_t(1)));
    res.insert(std::make_shared<BuiltInType>(Name(Scope(), "int"), 4, std::align_val_t(4)));

    return res;
}

}