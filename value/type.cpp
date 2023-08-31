#include <memory>
#include <new>

#include "type.hpp"
#include "scope.hpp"

namespace CINT {

Types::Type::TypeMultiSet Types::Type::TypeMultiSet::init() {
    auto res = std::multiset<std::shared_ptr<Type>, Types::Type::TypeMultiSet::SharedPtrTypeCmp>();

    res.insert(std::make_shared<BuiltInType>(TypeName(Scope::GLOBAL_SCOPE, "char"), 1, std::align_val_t(1)));
    res.insert(std::make_shared<BuiltInType>(TypeName(Scope::GLOBAL_SCOPE, "int"), 4, std::align_val_t(4)));

    return res;
}

std::shared_ptr<Types::Type> Types::Type::TypeMultiSet::find(const UnconfirmedName & _key) {
    auto keyType = std::make_shared<Type>(Scope::NOSCOPE, _key.base_name());

    auto equalRange = __typeMultiSet.equal_range(keyType);
    for(auto begin = equalRange.first, end = equalRange.second; begin != end; begin++) {
        if((*begin)->name() == _key) return *begin;
    }

    return NOTYPE;
}

}