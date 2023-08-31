#include "scope.hpp"
#include <memory>

namespace CINT {

Scope::ScopeMultiSet Scope::ScopeMultiSet::init() {
    auto res = std::multiset<std::shared_ptr<Scope>, SharedPtrScopeCmp>();

    res.insert(currentScopeStack[0]);

    return res;
}

Scope Scope::operator+(const Scope & _rhs) const & {
    auto res = Scope(*this);
    for(auto scopeName : _rhs.__scopeNames) {
        res.__scopeNames.push_back(scopeName);
    }
    return res;
}

Scope Scope::operator+(const std::string & _rhs) const & {
    auto res = Scope(*this);
    res.__scopeNames.push_back(_rhs);
    return res;
}

Match Scope::operator==(const UnconfirmedScope & _rhs) const & {
    if(this->__scopeNames.back() != _rhs.base_name()) return Match::Status::NOT_MATCHED;
    if(_rhs.specified_scope() != NOSCOPE && *_rhs.specified_scope() + _rhs.base_name() == *this) return Match::Status::SPECIFIED_MATCHED;

    for(auto currentScope : _rhs.current_scope_stack()) {
        if(*currentScope + _rhs.base_name() == *this) return Match::Status::LOCAL_MATCHED;
    }

    for(auto usingScope : _rhs.using_scopes()) {
        if(*usingScope + _rhs.base_name() == *this) return Match::Status::USING_MATCHED;
    }

    return Match::Status::NOT_MATCHED;
}

std::shared_ptr<Scope> Scope::ScopeMultiSet::find(const UnconfirmedScope & _key) const & {
    auto equalRange = __scopeMultiSet.equal_range(std::make_shared<Scope>(Scope({_key.base_name()})));
    for(auto begin = equalRange.first, end = equalRange.second; begin != end; begin++) {
        if(_key == **begin) return *begin;
    }
    return NOSCOPE;
}

} // namespace CINT