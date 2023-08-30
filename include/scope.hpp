#ifndef __SCOPE_HPP__
#define __SCOPE_HPP__

#include <stack>
#include <string>
#include <vector>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class Scope {
private:
    std::vector<std::string> __scopeNames;

public:
    inline Scope() noexcept : __scopeNames() {}
    inline Scope(const Scope & _src) : __scopeNames(_src.__scopeNames) {}
    inline Scope(Scope && _src) noexcept : __scopeNames(std::move(_src.__scopeNames)) {}

    inline void enter(std::string _newScopeName) { __scopeNames.push_back(_newScopeName); }
    inline void exit() { __scopeNames.pop_back(); }

public:
    static Scope currentScope;
    static std::stack<Scope> scopeStack;
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __SCOPE_HPP__ */