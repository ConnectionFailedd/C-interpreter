#include "name.hpp"
#include "scope.hpp"

namespace CINT {

Match Name::operator==(const UnconfirmedName & _rhs) const & noexcept {
    if(__baseName != _rhs.base_name()) return Match::Status::NOT_MATCHED;
    if(_rhs.specified_scope() != Scope::NOSCOPE && _rhs.specified_scope() == this->__scope) return Match::Status::SPECIFIED_MATCHED;
    if(_rhs.specified_scope() != Scope::NOSCOPE) return Match::Status::NOT_MATCHED;

    for(auto iter = Scope::currentScopeStack.rbegin(); iter != Scope::currentScopeStack.rend(); iter ++) {
        if(*iter == this->__scope) return Match::Status::LOCAL_MATCHED;
    }

    for(auto usingScope : _rhs.using_scopes()) {
        if(usingScope == this->__scope) return Match::Status::USING_MATCHED;
    }

    return Match::Status::NOT_MATCHED;
}

}