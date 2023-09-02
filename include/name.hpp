#ifndef __NAME_HPP__
#define __NAME_HPP__

#include <_types/_uint8_t.h>
#include <memory>
#include <string>
#include <vector>

#include "scope.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class UnconfirmedName;

class Name {
private:
    std::shared_ptr<Scope> __scope;
    std::string __baseName;

public:
    inline Name(const std::shared_ptr<Scope> & _scope, const std::string & _baseName) : __scope(_scope), __baseName(_baseName) {}
    inline Name(const std::shared_ptr<Scope> & _scope, std::string && _baseName) noexcept : __scope(_scope), __baseName(std::move(_baseName)) {}

    inline const std::shared_ptr<Scope> & scope() const noexcept { return __scope; }
    inline const std::string & base_name() const noexcept { return __baseName; }

    inline bool operator<(const Name & _rhs) const & noexcept { return __baseName < _rhs.__baseName; }
    Match operator==(const UnconfirmedName & _rhs) const & noexcept;

public:
    const static Name NONAME;
};

class UnconfirmedName {
private:
    std::shared_ptr<Scope> __specifiedScope;
    const std::vector<std::shared_ptr<Scope>> & __currentScopeStack;
    std::vector<std::shared_ptr<Scope>> __usingScopes;
    std::string __baseName;

public:
    inline UnconfirmedName(const std::shared_ptr<Scope> & _specifiedScope, const std::vector<std::shared_ptr<Scope>> & _usingScopes, const std::string & _baseName) : __currentScopeStack(Scope::currentScopeStack), __specifiedScope(_specifiedScope), __usingScopes(_usingScopes), __baseName(_baseName) {}
    inline UnconfirmedName(const std::string & _baseName) : __specifiedScope(Scope::NOSCOPE), __currentScopeStack(Scope::currentScopeStack), __usingScopes(), __baseName(_baseName) {}
    inline UnconfirmedName(std::string && _baseName) noexcept : __specifiedScope(Scope::NOSCOPE), __currentScopeStack(Scope::currentScopeStack), __usingScopes(), __baseName(std::move(_baseName)) {}

    inline const std::shared_ptr<Scope> & specified_scope() const noexcept { return __specifiedScope; }
    inline const std::vector<std::shared_ptr<Scope>> current_scope_stack() const noexcept { return __currentScopeStack; }
    inline const std::vector<std::shared_ptr<Scope>> using_scopes() const noexcept { return __usingScopes; }
    inline const std::string & base_name() const noexcept { return __baseName; }

    inline Match operator==(const Name & _rhs) const & noexcept { return _rhs == *this; }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __NAME_HPP__ */